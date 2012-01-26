//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "config.hpp"

#include <soci.h>
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
#include <sqlite3/soci-sqlite3.h> 
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
#include <postgresql/soci-postgresql.h> 
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
#include <mysql/soci-mysql.h> 
#endif

#include <saga/saga/util.hpp>

#if defined(BOOST_HAS_UNISTD_H)
#include <unistd.h>
#endif

#include <boost/assert.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>
#include <saga/impl/exception.hpp>

#include <boost/scoped_array.hpp>

#include "common_helpers.hpp"
#include "replica_database_connection.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace replica
{
    using namespace SOCI;

    namespace detail 
    {    
        inline BackEndFactory const*
        get_backend_factory(database_connection::db_type type)
        {
            switch(type) {
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
            case database_connection::db_sqlite:
                return &sqlite3;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
            case database_connection::db_postgresql:
                return &postgresql;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
            case database_connection::db_mysql:
                return &mysql;
#endif
            default:
                break;
            }
            return NULL;
        }

        // helper class to install/un-install user defined function
        class user_function
        {
        public:
            user_function(database_connection& db, char const* name, void *data = NULL)
            :   db_(db), name_(name)
            {
                db_.install_function(name, data);
            }
            ~user_function()
            {
                db_.uninstall_function(name_.c_str());
            }

        private:
            database_connection& db_;
            std::string name_;
        };
    }
    
    ////////////////////////////////////////////////////////////////////////////
    database_connection::database_connection(database_connection::db_type type, 
        std::string const& connect)
    try
      : sql_(*detail::get_backend_factory(type), connect),   // create db connection
        type_(type), connect_string_(connect), opened(false), 
        rand_((boost::int32_t)std::time(NULL))
    {
        init_database(connect);
    }
    catch (/*SOCI::SOCIError*/ std::runtime_error const &e) {
        SAGA_ADAPTOR_THROW_NO_CONTEXT(
             "Unable to establish connection to database_connection: " + connect + e.what(), 
             saga::NoSuccess);
    }

    void database_connection::init_database(std::string const& connect)
    {
        try {
        // additional back-end specific initialization
            init_backend();
            
        // get db version and type
            std::string version;
            eIndicator ind = eOK;
            sql_ << "select value from main where key = 'version'", 
                into(version, ind);
            if (ind != eOK) {
                SAGA_ADAPTOR_THROW_NO_CONTEXT(
                    "Unknown database_connection structure", saga::NoSuccess);
            }
            
            std::string type;
            sql_ << "select value from main where key = 'type'", 
                into(type, ind);
            if (ind != eOK) {
                SAGA_ADAPTOR_THROW_NO_CONTEXT(
                    "Unknown database_connection structure", saga::NoSuccess);
            }

        // verify version and type
            if (type != "logicalfile" ||
                boost::lexical_cast<int>(version) > SAGA_REPLICADB_LASTVERSION) 
            {
            // wrong database_connection type or db version too new for us
                SAGA_OSSTREAM strm;
                strm << "replica::init_database: " 
                      << "Unknown database_connection version (got: " << type << ", "
                      << version << "), connectstring: " << connect;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
            opened = true;
        }
        catch (/*SOCI::SOCIError*/ std::runtime_error const& e) {   // FIXME!
        // database_connection does not exist, try to create a new one
            if (!init_tables()) {
                SAGA_ADAPTOR_THROW_NO_CONTEXT(
                     "Unable to establish connection to database_connection: " + e.what(), 
                     saga::NoSuccess);
            }
            opened = true;
        }
    }

    void database_connection::close(bool verify)
    {
        if (verify)
            verify_status();
        opened = false;
    }
    
    void database_connection::verify_status() 
    {
        if (!is_open()) {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                std::string("Close has been called for this object."),
                saga::IncorrectState);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // generic wrapper for the transaction handling
    template <typename F>
    inline bool 
    database_connection::execute(F f, std::string const& name)
    {
        bool result = false;
        std::size_t count = 0;                  // counter for retries
        static const saga::uint8_t delays[] =   // delays between retries
        { 
            1, 2, 5, 10, 15, 20, 25, 25, 25, 50, 50, 100 // [ms]
        };
        std::size_t const num_delays = sizeof(delays)/sizeof(delays[0]);

        do {
            try {
            // wrap everything into a transaction
                sql_.begin();
                result = f();
                sql_.commit();
                break;  // everything went well, we can return to the caller
            }
            catch (/*SOCI::SOCIError*/ std::runtime_error const& e) {   // FIXME!
            // an error occurred in the underlying database_connection
            // some of these can be resolved by rolling back the transaction 
            // and starting over
                bool resolvable = error_is_resolvable();

                try {
                // rollback may throw an exception as well...
                    sql_.rollback();
                }
                catch (/*SOCI::SOCIError*/ std::runtime_error const& e1) {   // FIXME!
                // if this happens we're out of luck
                    SAGA_OSSTREAM strm;
                    strm << "replica::logical_file_cpi_impl::" << name 
                         << ": unexpected error during transaction rollback: " 
                         << e1.what();
                    strm << " (the error causing the rollback was: " 
                         << e.what() << ")";
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                        saga::NoSuccess);
                }

                if (!resolvable) {
                // if it's not resolvable, just rethrow as a saga exception
                    SAGA_OSSTREAM strm;
                    strm << "replica::logical_file_cpi_impl::" << name 
                         << ": unexpected error: " << e.what();
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                        saga::NoSuccess);
                }
            }
            catch (...) {
            // just roll back and rethrow the exception
                sql_.rollback();
                throw;
            }

        // sleep for a while...
            boost::xtime xt;
            boost::xtime_get(&xt, boost::TIME_UTC);
            if (count < num_delays)
                xt.nsec += delays[count] * 1000000;
            else
                xt.nsec += delays[num_delays-1] * 1000000;
            boost::thread::sleep(xt);
            ++count;

        } while (true);
        return result;
    }

    ///////////////////////////////////////////////////////////////////////////
    // detect, whether replica entry already exists, create, if necessary
    bool 
    database_connection::get_node_id(std::string entry, int parent_id, int& node_id,
        bool& is_dir)
    {
        eIndicator ind1 = eOK,ind2 = eOK;
        node_id = 0;
        int is_dir_data = 0;

        sql_ << "select node_id, is_directory from nodes "
                    "where node_name = :name and parent_id = :id", 
                into(node_id, ind1), into(is_dir_data, ind2), 
                use(entry), use(parent_id);
                
        is_dir = is_dir_data ? true : false;              
        return ind1 == eOK && ind2 == eOK;
    }
    
    namespace detail
    {
        // test if the iterator points to the last logical part of a path
        template <typename Iterator>
        inline bool is_last(Iterator it, Iterator const& end)
        {
            if (++it == end)
                return true;
            
            return (*it == "." && ++it == end) ? true : false;
        }
    }
        
    int 
    database_connection::find(std::string path, bool& is_dir, 
        bool create, bool create_parents, bool create_dir)
    {
        verify_status();
        
        if (path[0] != '/') {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                std::string("Invalid logical file name "
                    "(should be an absolute name): ") + path,
                saga::IncorrectURL);
        }
        
        typedef boost::char_separator<char> char_separator_type;
        typedef boost::tokenizer<char_separator_type> tokenizer_type;
        
        int parent_id = 0;
        int current_node_id = 1;    // the root has the node_id == 1

        tokenizer_type tok(path, char_separator_type("/"));
        tokenizer_type::iterator end = tok.end();
        for (tokenizer_type::iterator it = tok.begin(); it != end; ++it)
        {
        // re-initialization 
            parent_id = current_node_id;
            current_node_id = 0;

        // retrieve next element from path
            std::string entry(*it);
            if (entry == ".") {
            // if the node_name is '.' simply continue
                current_node_id = parent_id;
            }
            else if (entry == "..") {
            // we need to step up one level in the hierarchy 
                if (1 == parent_id) {
                // there is no way to step up at root level
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(
                        std::string("Invalid logical file name "
                            "('..' should not be used on root level): ") + path,
                        saga::IncorrectURL);
                }
                
                eIndicator ind = eOK;
                sql_ << "select parent_id from nodes where node_id = :id",
                    into(current_node_id, ind), use(parent_id);
                if (ind != eOK) {
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(
                        "Corrupt/inconsistent database_connection structure", 
                        saga::NoSuccess);
                }
            }
            else {
            // try to find this node
                if (!get_node_id(entry, parent_id, current_node_id, is_dir)) {
                    if (create) {
                        bool is_last = detail::is_last(it, end);
                        if (!create_parents && !is_last) {
                            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                                "Could not insert new replica entry "
                                    "(directory does not exist): " + path, 
                                saga::NoSuccess);
                        }
                        
                    // this node does not exist, insert it into the nodes table
                        int is_dir_data = create_dir || !is_last;
                        sql_ << "insert into nodes(node_name, parent_id, is_directory) "
                                "values(:name, :id, :dir)", 
                                use(entry), use(parent_id), use(is_dir_data);
                                
                    // now retrieve the new node_id
                        if (!get_node_id(entry, parent_id, current_node_id, is_dir)) 
                        {
                            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                                "Could not insert new replica entry "
                                    "(insertion failed): " + path, 
                                saga::NoSuccess);
                        }
                    }
                    else {
                        break;   // lfn not found
                    }
                }
            }
        }
        
        if (1 == current_node_id)   // __root__ is always a dir
            is_dir  = true;
            
        return current_node_id;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    //  Find a leaf node
    int database_connection::find_entry(std::string lfn, bool throw_on_error)
    {
        bool found_dir = false;
        int node = find(lfn, found_dir);
        if (0 != node && found_dir) {
            if (throw_on_error) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::find_entry: "
                        "could not find replica entry: " << lfn <<
                        " logical directory with the same name already exists.";
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::AlreadyExists);
            }
            return 0;
        }
        return node;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  Find a directory node
    int database_connection::find_directory(std::string lfn, bool throw_on_error)
    {
        bool found_dir = false;
        int node = find(lfn, found_dir);
        if (0 != node && !found_dir) {
            if (throw_on_error) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::find_directory: "
                        "could not find replica directory node: " << lfn <<
                        " logical file with the same name already exists.";
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::AlreadyExists);
            }
            return 0;
        }
        return node;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  Create a leaf node
    bool database_connection::create_entry(std::string const& lfn, bool create_parents) 
    {
        return execute(
            TR1::bind(&database_connection::create_entry_db, this, lfn, create_parents), 
            "create_entry");
    }
    
    bool database_connection::create_entry_db(std::string const& lfn, bool create_parents) 
    { 
        bool found_dir = false;
        int node = find(lfn, found_dir, true, create_parents); 
        if (0 == node || found_dir) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::create_entry: "
                    "could not create replica entry: " << lfn <<
                    " logical directory with the same name exists";
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::AlreadyExists);
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  Create a leaf node
    bool database_connection::create_directory(std::string const& lfn, bool create_parents) 
    {
        return execute(
            TR1::bind(&database_connection::create_directory_db, this, lfn, create_parents), 
            "create_directory");
    }
    
    bool database_connection::create_directory_db(std::string const& lfn, bool create_parents) 
    { 
        bool found_dir = false;
        int node = find(lfn, found_dir, true, create_parents, true); 
        if (0 == node || !found_dir) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::create_directory: "
                    "could not create replica directory node: " << lfn <<
                    " logical file with the same name already exists.";
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::AlreadyExists);
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        inline bool
        verify_indicators(std::vector<eIndicator> const& inds)
        {
            std::vector<eIndicator>::const_iterator end = inds.end();
            for (std::vector<eIndicator>::const_iterator it = inds.begin(); 
                it != end; ++it)
            {
                if (*it != eOK)
                    return false;
            }
            return true;
        }
    }

    bool 
    database_connection::get_entries(std::string const& lfn, std::vector<std::string>& locations)
    {
        return execute(TR1::bind(&database_connection::get_entries_db, this, lfn, 
                TR1::ref(locations)), "get_entries");
    }

    bool 
    database_connection::get_entries_db(std::string const& lfn, std::vector<std::string>& locations)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::get_entries: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // retrieve number of elements
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from data where node_id = :id",
            into(count, ind), use(node_id);
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::get_entries: "
                    "could not access data for replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }
        if (0 == count) 
            return false;   // no replica entries exist for this lfn
        
    // read data
        if (1 == count) {
            std::string location;
            sql_ << "select filename from data where node_id = :id",
                into(location, ind), use(node_id);
                
            if (ind != eOK) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::get_entries: "
                        "could not access data for replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
            locations.push_back(location);
        }
        else {
            std::vector<eIndicator> inds(count, eOK);
        
            locations.resize(count);
            sql_ << "select filename from data where node_id = :id",
                into(locations, inds), use(node_id);
                
            if (!detail::verify_indicators(inds)) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::get_entries: "
                        "could not access data for replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool database_connection::add_entry(std::string const& lfn, std::string const& location)
    {
        return execute(TR1::bind(&database_connection::add_entry_db, this, lfn, location), 
            "add_entry");
    }
    
    bool database_connection::add_entry_db(std::string const& lfn, std::string location)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::add_entry: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // verify that the entry does not exist
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from data "
                "where node_id = :id and filename = :value",
            into(count, ind), use(node_id), use(location);
            
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::add_entries: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }

    // spec requires to silently do nothing if the entry does already exist
        if (0 != count) 
            return true;

    // add the given location to this node
        sql_ << "insert into data (node_id, filename) values(:id, :value)", 
            use(node_id), use(location);
    
        return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    bool 
    database_connection::remove_entry(std::string const& lfn, std::string const& location)
    {
        return execute(TR1::bind(&database_connection::remove_entry_db, this, lfn, location), 
            "remove_entry");
    }
    
    bool 
    database_connection::remove_entry_db(std::string const& lfn, std::string location)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::get_entries: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // verify that the entry exists
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from data "
                "where node_id = :id and filename = :value",
            into(count, ind), use(node_id), use(location);
            
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::remove_entry: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }
        if (0 == count) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::remove_entry: "
                    "replica entry '" << lfn << "' does not contain location: " 
                 << location;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::DoesNotExist);
        }

    // remove the entry/entries
        sql_ << "delete from data where node_id = :id and filename = :value",
            use(node_id), use(location);
            
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool 
    database_connection::update_entry(std::string const& lfn, std::string const& oldlocation, 
        std::string const& newlocation)
    {
        return execute(TR1::bind(&database_connection::update_entry_db, this, lfn, 
                oldlocation, newlocation), "update_entry");
    }
    
    bool 
    database_connection::update_entry_db(std::string const& lfn, std::string oldlocation, 
        std::string newlocation)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::get_entries: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // verify that the entry exists
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from data "
                "where node_id = :id and filename = :value",
            into(count, ind), use(node_id), use(oldlocation);
            
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::update_entry: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }
        if (0 == count) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::update_entry: "
                    "replica entry '" << lfn << "' does not contain location: " 
                 << oldlocation;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::DoesNotExist);
        }

    // remove the entry/entries
        sql_ << "delete from data where node_id = :id and filename = :value",
            use(node_id), use(oldlocation);
            
    // add the given location to this node
        sql_ << "insert into data (node_id, filename) values(:id, :value)", 
            use(node_id), use(newlocation);

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool database_connection::remove_node(std::string const& lfn, int node_id, bool is_entry)
    {
        int count = 0;
        eIndicator ind = eOK;

        if (is_entry) {
        // remove all replica entries from the database_connection
            sql_ << "select count(*) from data where node_id = :id",
                into(count, ind), use(node_id);
                
            if (ind != eOK) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::remove_node: "
                        "could not access replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
            if (0 != count) {
            // remove the entry/entries
                sql_ << "delete from data where node_id = :id", use(node_id);
            }
        }
        else {    // this is a directory, remove everything recursively
        // retrieve node_id's of all contained objects
            sql_ << "select count(*) from nodes where parent_id = :id",
                into(count, ind), use(node_id);
            if (ind != eOK) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::remove_node: "
                        "could not access replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }

        // remove all contained entries recursively
            if (1 == count) {
            // retrieve the node_id of the single child node
                int is_dir = 0;
                int id = 0;
                eIndicator ind1 = eOK, ind2 = eOK;
                sql_ << "select node_id, is_directory from nodes where parent_id = :id",
                    into(id, ind1), into(is_dir, ind2), use(node_id);
                if (ind1 != eOK || ind2 != eOK) {
                    SAGA_OSSTREAM strm;
                    strm << "replica::logical_file_cpi_impl::remove_node: "
                            "could not access replica entry: " << lfn;
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                        saga::NoSuccess);
                }
                remove_node(lfn, id, !is_dir);
            }
            else if (count > 1) {
            // retrieve node_id's of all child nodes
                std::vector<int> ids(count), is_dirs(count);
                std::vector<eIndicator> inds1(count, eOK), inds2(count, eOK);
                sql_ << "select node_id, is_directory from nodes where parent_id = :id",
                    into(ids, inds1), into(is_dirs, inds2), use(node_id);
                if (!detail::verify_indicators(inds1) || 
                    !detail::verify_indicators(inds2)) 
                {
                    SAGA_OSSTREAM strm;
                    strm << "replica::logical_file_cpi_impl::remove_node: "
                            "could not access replica entry: " << lfn;
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                        saga::NoSuccess);
                }
                
            // now delete all the nodes
                for (unsigned int i = 0; i < ids.size(); ++i)
                {
                    remove_node(lfn, ids[i], is_dirs[i] == 0);
                }
            }
        }
        
    // remove the attributes (metadata)
        count = 0;
        ind = eOK;
        sql_ << "select count(*) from metadata where node_id = :id",
            into(count, ind), use(node_id);
            
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::remove_node: "
                    "could not access attributes for replica entry: " 
                << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }
        if (0 != count) {
        // remove attribute data
            sql_ << "delete from metadata where node_id = :id", use(node_id);
        }

    // remove the leaf node 
        sql_ << "delete from nodes where node_id = :id", use(node_id);
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool database_connection::remove(std::string const& lfn)
    {
        return execute(TR1::bind(&database_connection::remove_db, this, lfn), "remove");
    }
   
    bool database_connection::remove_db(std::string const& lfn)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::remove: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // remove this entry
        return remove_node(lfn, node_id, true);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    //  directory related functions
    bool database_connection::remove_directory(std::string const& lfn)
    {
        return execute(TR1::bind(&database_connection::remove_directory_db, this, lfn),
            "remove_directory");
    }
    
    bool database_connection::remove_directory_db(std::string const& lfn)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_directory(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::remove_directory: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // remove this directory
        return remove_node(lfn, node_id, false);
    }

    ///////////////////////////////////////////////////////////////////////////
    bool database_connection::list(std::string const& lfn, std::string const& pattern, 
        std::vector<std::string>& entries)
    {
        return execute(TR1::bind(&database_connection::list_db, this, lfn, pattern, 
                TR1::ref(entries)), "list");
    }
    
    bool 
    database_connection::list_db(std::string const& lfn, std::string pattern, 
        std::vector<std::string>& entries)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_directory(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::list: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

        detail::user_function match_func(*this, "match");

    // test, whether this directory node contains anything
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from nodes where parent_id = :id and "
                    + detail::get_match(type_, ":pattern", "node_name"), 
            into(count, ind), use(node_id), use(pattern);
            
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::list: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }
        if (0 == count) 
            return true;    // no entries available

    // retrieve node names
        if (1 == count) {
            std::string entry;
            sql_ << "select node_name from nodes where parent_id = :id and "
                        + detail::get_match(type_, ":pattern", "node_name"), 
                into(entry, ind), use(node_id), use(pattern);
                
            if (ind != eOK) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::list: "
                        "could not access data for replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
            entries.push_back(entry);
        }
        else {
            std::vector<eIndicator> inds(count, eOK);
        
            entries.resize(count);
            sql_ << "select node_name from nodes where parent_id = :id and "
                        + detail::get_match(type_, ":pattern", "node_name"),
                into(entries, inds), use(node_id), use(pattern);
                
            if (!detail::verify_indicators(inds)) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::list: "
                        "could not access data for replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
        }

        return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    bool 
    database_connection::list_attributes(std::string const& lfn, 
        std::vector<std::string>& keys)
    {
        return execute(TR1::bind(&database_connection::list_attributes_db, this, lfn, 
                TR1::ref(keys)), "list_attributes");
    }
    
    bool 
    database_connection::list_attributes_db(std::string const& lfn, 
        std::vector<std::string>& keys)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::list_attributes: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // retrieve number of elements
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from metadata where node_id = :id",
            into(count, ind), use(node_id);
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::list_attributes: "
                    "could not access attributes for replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }
        if (0 == count) 
            return false;   // no attributes exist for this lfn
        
    // read data
        if (1 == count) {
            std::string key;
            sql_ << "select metakey from metadata where node_id = :id",
                into(key, ind), use(node_id);
                
            if (ind != eOK) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::list_attributes: "
                        "could not access attributes for replica entry: " 
                     << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
            keys.push_back(key);
        }
        else {
            std::vector<eIndicator> inds(count, eOK);
        
            keys.resize(count);
            sql_ << "select metakey from metadata where node_id = :id",
                into(keys, inds), use(node_id);
                
            if (!detail::verify_indicators(inds)) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::list_attributes: "
                        "could not access attributes for replica entry: " 
                     << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    // find keys with a matching attribute
    bool 
    database_connection::find_attributes(std::string const& lfn, std::string const& kpat, 
        std::string const& vpat, std::vector<std::string>* keys, bool count_only)
    {
        return execute(TR1::bind(&database_connection::find_attributes_db, this, lfn, 
                kpat, vpat, keys, count_only), "find_attributes");
    }
    
    bool 
    database_connection::find_attributes_db(std::string const& lfn, std::string kpat, 
        std::string vpat, std::vector<std::string>* keys, bool count_only)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::find_attributes: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // install user defined function to match keys and values
        detail::user_function match_func(*this, "match");
            
    // retrieve count of matching records
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from metadata where node_id = :id and "
                    + detail::get_match(type_, ":kpat", "metakey") + " and "
                    + detail::get_match(type_, ":vpat", "metavalue"),
            into(count, ind), use(node_id), use(kpat), use(vpat);

        if (count_only || 0 == count)
            return 0 != count;
            
    // retrieve records
        if (1 == count) {
            std::string entry;
            sql_ << "select metakey from metadata where node_id = :id and "
                        + detail::get_match(type_, ":kpat", "metakey") + " and " 
                        + detail::get_match(type_, ":vpat", "metavalue"),
                into(entry, ind), use(node_id), use(kpat), use(vpat);
                
            if (ind != eOK) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::find_attributes: "
                        "could not access data for replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
            keys->push_back(entry);
        }
        else {
            std::vector<eIndicator> inds(count, eOK);
        
            keys->resize(count);
            sql_ << "select metakey from metadata where node_id = :id and "
                        + detail::get_match(type_, ":kpat", "metakey") + " and " 
                        + detail::get_match(type_, ":vpat", "metavalue"),
                into(*keys, inds), use(node_id), use(kpat), use(vpat);
                
            if (!detail::verify_indicators(inds)) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::find_attributes: "
                        "could not access data for replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
        }

        return 0 != count;
    }

    ///////////////////////////////////////////////////////////////////////////
    // find keys with matching attributes
    bool 
    database_connection::find_attributes(std::string const& lfn, 
        std::vector<std::string> const& kpat, 
        std::vector<std::string> const& vpat, std::vector<std::string>* keys,
        bool count_only)
    {
        return execute(TR1::bind(&database_connection::find_attributes_db_v, this, lfn, 
                kpat, vpat, keys, count_only), "find_attributes");
    }
    
    bool
    database_connection::find_attributes_db_v(std::string const& lfn, 
        std::vector<std::string> const& kpat, 
        std::vector<std::string> const& vpat, std::vector<std::string>* keys,
        bool count_only)
    {
        BOOST_ASSERT(kpat.size() == vpat.size());
        
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::find_attributes: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // build sql query
        SAGA_OSSTREAM strm;
        strm << "from metadata where node_id = :id";
        
        std::vector<std::string>::const_iterator end_kpat = kpat.end();
        for (std::vector<std::string>::const_iterator it_kpat = kpat.begin(),
             it_vpat = vpat.begin(); it_kpat != end_kpat; 
             ++it_kpat, ++it_vpat)
        {
        // build the query
            strm << " and " << detail::get_match(type_, ("'" + *it_kpat + "'").c_str(), "metakey")
                 << " and " << detail::get_match(type_, ("'" + *it_vpat + "'").c_str(), "metavalue");
        }

    // install the functions
        detail::user_function match_func(*this, "match");

    // retrieve count of matching records
        int count = 0;
        eIndicator ind = eOK;
        std::string sql ("select count(*) " + SAGA_OSSTREAM_GETSTRING(strm));
        sql_ << sql, into(count, ind), use(node_id);

        if (count_only || 0 == count) 
            return 0 != count;

    // retrieve records
        sql = "select metakey " + SAGA_OSSTREAM_GETSTRING(strm);
        if (1 == count) {
            std::string entry;
            sql_ << sql, into(entry, ind), use(node_id);
                
            if (ind != eOK) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::find_attributes: "
                        "could not access data for replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
            keys->push_back(entry);
        }
        else {
            std::vector<eIndicator> inds(count, eOK);
        
            keys->resize(count);
            sql_ << sql, into(*keys, inds), use(node_id);
                
            if (!detail::verify_indicators(inds)) {
                SAGA_OSSTREAM strm;
                strm << "replica::logical_file_cpi_impl::find_attributes: "
                        "could not access data for replica entry: " << lfn;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::NoSuccess);
            }
        }

        return 0 != count;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    bool database_connection::attribute_exists(std::string const& lfn, 
        std::string const& key, bool& val)
    {
        return execute(TR1::bind(&database_connection::attribute_exists_db, this, lfn, 
                key, TR1::ref(val)), "attribute_exists");
    }
    
    bool database_connection::attribute_exists_db(std::string const& lfn, 
        std::string key, bool& val)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::attribute_exists: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // read attribute value for the given metakey 
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from metadata "
                    "where node_id = :id and metakey = :key",
            into(count, ind), use(node_id), use(key);
            
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::attribute_exists: "
                    "could not access attribute '" << key <<
                    "' data for replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::DoesNotExist);
        }
        
        val = (count != 0);
        return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    bool database_connection::get_attribute(std::string const& lfn, 
        std::string const& key, std::string& val, bool& is_vector)
    {
        return execute(TR1::bind(&database_connection::get_attribute_db, this, lfn, 
                key, TR1::ref(val), TR1::ref(is_vector)), "get_attribute");
    }
    
    bool 
    database_connection::get_attribute_db(std::string const& lfn, std::string key, 
        std::string& val, bool& is_vector)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::get_attribute: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // read attribute value for the given metakey 
        int is_vector_data = 0;
        eIndicator ind1 = eOK, ind2 = eOK;
        sql_ << "select metavalue, is_vector from metadata "
                    "where node_id = :id and metakey = :key",
            into(val, ind1), into(is_vector_data, ind2), use(node_id), use(key);
            
        if (ind1 != eOK || ind2 != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::get_attribute: "
                    "could not access attribute '" << key <<
                    "' data for replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }
        
        is_vector = is_vector_data != 0;
        return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    bool 
    database_connection::set_attribute(std::string const& lfn, std::string const& key, 
        std::string const& val, bool is_vector)
    {
        return execute(TR1::bind(&database_connection::set_attribute_db, this, lfn, 
                key, val, is_vector), "set_attribute");
    }
    
    bool 
    database_connection::set_attribute_db(std::string const& lfn, std::string key, 
        std::string val, bool is_vector)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::set_attribute: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // figure out, if the given metakey is already defined for this node
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from metadata "
                    "where node_id = :id and metakey = :key",
            into(count, ind), use(node_id), use(key);
            
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::set_attribute: "
                    "could not access attribute '" << key <<
                    "' data for replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }

    // insert/update attribute data
        int is_vector_data = is_vector ? 1 : 0;
        if (0 == count) {
        // insert a new entry
            sql_ << "insert into "
                        "metadata(node_id, metakey, metavalue, is_vector) "
                        "values(:node_id, :key, :value, :is_vector)",
                use(node_id), use(key), use(val), use(is_vector_data);
        }
        else {
        // update the existing entry
            sql_ << "update metadata "
                        "set metavalue = :value, is_vector = :is_vector "
                        "where node_id = :id and metakey = :key",
                use(val), use(is_vector_data), use(node_id), use(key);
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool 
    database_connection::remove_attribute(std::string const& lfn, 
        std::string const& key)
    {
        return execute(TR1::bind(&database_connection::remove_attribute_db, this, lfn, 
                key), "remove_attribute");
    }
    
    bool 
    database_connection::remove_attribute_db(std::string const& lfn, std::string key)
    {
    // retrieve node corresponding to the given lfn
        int node_id = find_entry(lfn);
        if (0 == node_id) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::remove_attribute: "
                    "could not access replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::BadParameter);
        }

    // figure out, if the given metakey is defined for this node
        int count = 0;
        eIndicator ind = eOK;
        sql_ << "select count(*) from metadata "
                    "where node_id = :id and metakey = :key",
            into(count, ind), use(node_id), use(key);
            
        if (ind != eOK) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::remove_attribute: "
                    "could not access attribute '" << key <<
                    "' data for replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::NoSuccess);
        }
        if (0 == count) {
            SAGA_OSSTREAM strm;
            strm << "replica::logical_file_cpi_impl::remove_attribute: "
                    "could not access attribute '" << key <<
                    "' data for replica entry: " << lfn;
            SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::DoesNotExist);
        }

    // remove attribute data
        sql_ << "delete from metadata where node_id = :id and metakey = :key",
            use(node_id), use(key);

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    // dispatcher to initialize tables in database_connection 
    bool database_connection::init_tables()
    {
        switch(type_) {
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
        case db_sqlite:
            init_tables_sqlite3();
            return true;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
        case db_postgresql:   // nothing to do
            break;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
        case db_mysql:        // nothing to do
            break;
#endif
        default:
            break;
        }
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // dispatcher to initialize back-end 
    void database_connection::init_backend()
    {
        switch(type_) {
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
        case db_sqlite:
            init_backend_sqlite3();
            break;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
        case db_postgresql:   // nothing to do
            break;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
        case db_mysql:        // nothing to do
            break;
#endif

        default:
            break;
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // dispatcher to test if last error may be handled by repeating the 
    // transaction
    bool database_connection::error_is_resolvable()
    {
        switch(type_) {
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
        case db_sqlite:
            return error_is_resolvable_sqlite3();
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
        case db_postgresql:   // nothing to do
            break;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
        case db_mysql:        // nothing to do
            break;
#endif

        default:
            break;
        }
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // install user define function
    void 
    database_connection::install_function(char const* name, void *data)
    {
        switch(type_) {
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
        case db_sqlite:
            install_function_sqlite3(name, data);
            break;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
        case db_postgresql:   // nothing to do
            break;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
        case db_mysql:        // nothing to do
            break;
#endif

        default:
            break;
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // un-install user define function
    void 
    database_connection::uninstall_function(char const* name)
    {
        switch(type_) {
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
        case db_sqlite:
            uninstall_function_sqlite3(name);
            break;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
        case db_postgresql:   // nothing to do
            break;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
        case db_mysql:        // nothing to do
            break;
#endif

        default:
            break;
        }
    }
    
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
    ///////////////////////////////////////////////////////////////////////////
    //  SQLite3 specifics
    static char const *init_sqlite3_stmts[] = 
    {
        "begin transaction;",

        // create and populate the main table 
        "create table main ("
            "id integer primary key on conflict rollback autoincrement, "
            "key varchar(32) not null on conflict rollback, "
            "value varchar(128)"
        ");",
        "insert or rollback into main(key, value) values('version', '256');",
        "insert or rollback into main(key, value) values('type', 'logicalfile');",

        // create and populate the node table 
        "create table nodes ("
            "node_id integer primary key on conflict rollback autoincrement, "
            "node_name not null on conflict rollback, "  
            "parent_id integer, "   // references key nodes(node_id) 
            "is_directory integer"  // 0: is a leaf node (file), 1: is a directory
        ");",
        "insert or rollback into nodes(node_name, parent_id, is_directory) "
            "values('__root__', NULL, 1);",

        // create the data table, all replicas for a logical file name share 
        // the same node_id
        "create table data ("
            "node_id integer not null on conflict rollback, "    // references key nodes(node_id) 
            "filename"
        ");",

        // create the meta data table (this table holds all the meta data)
        "create table metadata ("
            "node_id integer not null on conflict rollback, "    // references key nodes(node_id) 
            "metakey not null on conflict rollback, "    // meta key
            "metavalue, "          // meta value
            "is_vector integer"    // meta type (0: plain, 1: vector)
        ");",
        "insert or rollback into metadata(node_id, metakey, metavalue, is_vector) "
            "values('1', 'ctime', current_timestamp, '0');",
        "insert or rollback into metadata(node_id, metakey, metavalue, is_vector) "
            "values('1', 'utime', current_timestamp, '0');",

        // create indices for metadata.metakey
        "create index metakey_idx on metadata (metakey);",
        "create index node_id_idx on metadata (node_id);",

        // create a trigger to be executed on node creation
        "create trigger insert_nodes after insert on nodes "
        "begin "
            // new metadata items: creation time and modification 
            "insert into metadata(node_id, metakey, metavalue, is_vector) "
                "values(new.node_id, 'ctime', current_timestamp, 0); "
            "insert into metadata(node_id, metakey, metavalue, is_vector) "
                "values(new.node_id, 'utime', current_timestamp, 0); "

            // update the parents modification time
            "update metadata set metavalue = current_timestamp "
                "where node_id = new.parent_id and metakey = 'utime'; "
        "end;",

        // create a trigger to be executed on data insertion
        "create trigger insert_data after insert on data "
        "begin "
            "update metadata set metavalue = current_timestamp "
                "where node_id = new.node_id and metakey = 'utime'; "
        "end;",

        // create a trigger to be executed on data modification
        "create trigger update_data after update on data "
        "begin "
            "update metadata set metavalue = current_timestamp "
                "where node_id = new.node_id and metakey = 'utime'; "
        "end;",

        // create a trigger to be executed on node deletion
        "create trigger delete_nodes before delete on nodes "
        "begin "
            // delete all metadata for this node
            "delete from metadata where old.node_id = node_id; "

            // update the parents modification time
            "update metadata set metavalue = current_timestamp "
                "where node_id = old.parent_id and metakey = 'utime'; "
        "end;",

        // commit everything
        "commit transaction;",
        NULL
    };

    void database_connection::init_tables_sqlite3()
    {
        Sqlite3SessionBackEnd *be = 
            static_cast<Sqlite3SessionBackEnd *>(sql_.getBackEnd());

        for (int i = 0; NULL != init_sqlite3_stmts[i]; ++i)
        {
            char *err_msg = NULL;
            int result = sqlite_api::sqlite3_exec(be->conn_, 
                init_sqlite3_stmts[i], 0, 0, &err_msg);

            if (SQLITE_OK != result) {
                SAGA_OSSTREAM ss;
                ss << "Cannot execute sqlite3 statement (" 
                   << init_sqlite3_stmts[i] << "): " << err_msg
                   << " (error code: " << result << ")";
                sqlite_api::sqlite3_free(err_msg);
                throw SOCI::SOCIError(SAGA_OSSTREAM_GETSTRING(ss));
            }
        }
    }

    // sqlite callback function for user defined function current_timestamp
    void current_timestep_sqlite3(sqlite_api::sqlite3_context *ctx, int argc, 
        sqlite_api::sqlite3_value **argv)
    {
        if (0 != argc) {
            SAGA_OSSTREAM ss;
            ss << "Wrong parameter count for sqlite3::current_timestamp "
                  "function: " << argc << ", expected no parameters";
            sqlite_api::sqlite3_result_error(ctx, 
                SAGA_OSSTREAM_GETSTRING(ss).c_str(), SQLITE_MISUSE);
        }
        else {
        // get the current time
            std::time_t current = 0;
            std::time(&current);
            std::string current_time(std::ctime(&current));
            current_time.resize(current_time.size()-1);

        // add some randomness (to make sure timestamp will be different even
        // if in the same second
            boost::minstd_rand &data = 
                *(boost::minstd_rand *)sqlite_api::sqlite3_user_data(ctx);
            current_time += " " + boost::lexical_cast<std::string>(data());
            
        // return the current data/time
            char *result = strdup(current_time.c_str());
            sqlite_api::sqlite3_result_text(ctx, result, 
                (int)current_time.size(), free);
        }
    }

    // initialize sqlite3 backend
    void database_connection::init_backend_sqlite3()
    {
        Sqlite3SessionBackEnd *be = 
            static_cast<Sqlite3SessionBackEnd *>(sql_.getBackEnd());

        // set busy timeout
        int result = sqlite_api::sqlite3_busy_timeout(be->conn_, 30000);    // FIXME: read timeout from ini
        if (SQLITE_OK != result) {
            SAGA_OSSTREAM ss;
            ss << "Cannot initialize sqlite3 backend timeout" 
               << " (error code: " << result << ")";
            throw SOCI::SOCIError(SAGA_OSSTREAM_GETSTRING(ss));
        }
        
        // install user defined functions
        result = sqlite_api::sqlite3_create_function(be->conn_, 
            "current_timestamp", 0, SQLITE_UTF8, &rand_, 
            current_timestep_sqlite3, NULL, NULL);
            
        if (SQLITE_OK != result) {
            SAGA_OSSTREAM ss;
            ss << "Cannot install sqlite3 user function: current_timestamp" 
               << " (error code: " << result << ")";
            throw SOCI::SOCIError(SAGA_OSSTREAM_GETSTRING(ss));
        }
    }

    // test, if the last error was SQLITE_BUSY
    bool database_connection::error_is_resolvable_sqlite3()
    {
        Sqlite3SessionBackEnd *be = 
            static_cast<Sqlite3SessionBackEnd *>(sql_.getBackEnd());
        return SQLITE_BUSY == sqlite_api::sqlite3_errcode(be->conn_);
    }
    
    // sqlite callback function for user define function match()
    void re_match_sqlite3(sqlite_api::sqlite3_context *ctx, int argc, 
        sqlite_api::sqlite3_value **argv)
    {
        if (2 != argc) {
            SAGA_OSSTREAM ss;
            ss << "Wrong parameter count for sqlite3::match function: " 
               << argc << ", expected exactly 2 parameters";
            sqlite_api::sqlite3_result_error(ctx, 
                SAGA_OSSTREAM_GETSTRING(ss).c_str(), SQLITE_MISUSE);
        }
        else if (NULL != argv[0] && NULL != argv[1]) {
        // try to match field value
            TR1::regex rx((char const*)sqlite3_value_text(argv[0]));
            char const* value = (char const*)sqlite3_value_text(argv[1]);
            
            sqlite_api::sqlite3_result_int(ctx, 
                TR1::regex_match(value, rx) ? 1 : 0);
        }
        else {
        // field value is NULL
            sqlite_api::sqlite3_result_int(ctx, 0);
        }
    }

    void 
    database_connection::install_function_sqlite3(char const* name, void *data)
    {
        Sqlite3SessionBackEnd *be = 
            static_cast<Sqlite3SessionBackEnd *>(sql_.getBackEnd());

        int result = sqlite_api::sqlite3_create_function(be->conn_, name, 2,
            SQLITE_UTF8, data, re_match_sqlite3, NULL, NULL);
            
        if (SQLITE_OK != result) {
            SAGA_OSSTREAM ss;
            ss << "Cannot install sqlite3 user function: " << name 
               << " (error code: " << result << ")";
            throw SOCI::SOCIError(SAGA_OSSTREAM_GETSTRING(ss));
        }
    }

    void 
    database_connection::uninstall_function_sqlite3(char const* name)
    {
        Sqlite3SessionBackEnd *be = 
            static_cast<Sqlite3SessionBackEnd *>(sql_.getBackEnd());

        int result = sqlite_api::sqlite3_create_function(be->conn_, name, 2,
            SQLITE_UTF8, NULL, NULL, NULL, NULL);

        if (SQLITE_OK != result) {
            SAGA_OSSTREAM ss;
            ss << "Cannot uninstall sqlite3 user function: " << name 
               << " (error code: " << result << ")";
            throw SOCI::SOCIError(SAGA_OSSTREAM_GETSTRING(ss));
        }
    }
#endif // defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)

///////////////////////////////////////////////////////////////////////////////
}   // replica
