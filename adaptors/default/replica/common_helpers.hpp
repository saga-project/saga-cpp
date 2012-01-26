//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(ADAPTORS_DEFAULT_REPLICA_COMMON_HELPERS_HPP)
#define ADAPTORS_DEFAULT_REPLICA_COMMON_HELPERS_HPP

#include <vector>
#include <string>

#include <boost/config.hpp>
#include <boost/algorithm/string/predicate.hpp>
#if defined(BOOST_HAS_UNISTD_H)
#include <unistd.h>
#endif
#if defined(BOOST_WINDOWS)
#include <windows.h>
#endif

#include <saga/saga/adaptors/utils/ini/ini.hpp>
#include <saga/saga/context.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/utils/is_local_address.hpp>

#include "replica_database_connection.hpp"
#include "connectstring.hpp"
#include "config.hpp"

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

///////////////////////////////////////////////////////////////////////////////
namespace replica { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////
    // wrapper for gethostname()
    inline std::string 
    get_hostname()
    {
        char buffer[MAX_PATH] = { '\0' };
        gethostname(buffer, sizeof(buffer));
        return std::string(buffer);
    }

    // parse a parameter string and fill the parameters into the provided map
    inline void 
    fill_parameter_map(std::string query, std::map<std::string, std::string>& params)
    {
        typedef std::map<std::string, std::string> params_type;

        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep(";&");
        tokenizer tokens(query, sep);
        tokenizer::iterator end = tokens.end();
        for (tokenizer::iterator it = tokens.begin(); it != end; ++it)
        {
            std::string::size_type p = (*it).find_first_of("=");
            if (p != std::string::npos)
                params.insert(params_type::value_type((*it).substr(0, p), (*it).substr(p+1)));
            else
                params.insert(params_type::value_type((*it).substr(0, p), ""));
        }
    }

    // retrieve a (optional) dbtype from the given url
    template <typename InstanceData>
    inline bool retrieve_dbtype(saga::impl::proxy* p, std::string& dbtype)
    {
        saga::url advert_url;
        {
            InstanceData data(p);
            advert_url = data->location_;
        }

        std::string query(advert_url.get_query());
        if (!query.empty())
        {
            typedef std::map<std::string, std::string> param_type;
            param_type params;
            fill_parameter_map(query, params);
            typename param_type::iterator it = params.find("dbtype");
            if (it != params.end()) {
                dbtype = (*it).second;
                return true;
            }
        }
        return false;
    }

    // retrieve a (optional) dbtype from the given url
    template <typename InstanceData>
    inline bool retrieve_dbname(saga::impl::proxy* p, std::string& dbname)
    {
        saga::url advert_url;
        {
            InstanceData data(p);
            advert_url = data->location_;
        }

        std::string query(advert_url.get_query());
        if (!query.empty())
        {
            typedef std::map<std::string, std::string> param_type;
            param_type params;
            fill_parameter_map(query, params);
            typename param_type::iterator it = params.find("dbname");
            if (it != params.end()) {
                dbname = (*it).second;
                return true;
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  retrieve from ini file the type of database_connection to use
    template <typename InstanceData>
    inline database_connection::db_type 
    get_db_type(saga::ini::ini const& ini, saga::impl::proxy* p)
    {
    // try to find database_connection back end
        std::string dbtype;
        if (retrieve_dbtype<InstanceData>(p, dbtype) || 
            ini.has_section("preferences")) 
        {
        saga::ini::ini prefs = ini.get_section ("preferences");

            if (dbtype.empty() && prefs.has_entry("dbtype"))
                dbtype = prefs.get_entry("dbtype");

            if (dbtype == "sqlite3")
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
                return database_connection::db_sqlite;
#else
            {
                SAGA_OSSTREAM strm;
                strm << "replica::detail::get_db_type: "
                        "SQLite3 backend has not been configured.";
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::BadParameter);
            }
#endif
        // handle postgresql
            if (dbtype == "postgresql")
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
                return database_connection::db_postgresql;
#else
            {
                SAGA_OSSTREAM strm;
                strm << "replica::detail::get_db_type: "
                        "PostgreSQL backend has not been configured.";
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::BadParameter);
            }
#endif

      // handle mysql
            if (dbtype == "mysql")
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
                return database_connection::db_mysql;
#else
            {
                SAGA_OSSTREAM strm;
                strm << "replica::detail::get_db_type: "
                        "MySQL backend has not been configured.";
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::BadParameter);
            }
#endif

            if (!dbtype.empty()) {
                SAGA_OSSTREAM strm;
                strm << "replica::detail::get_db_type: "
                        "unknown database_connection dbtype specified in configuration: "
                     << dbtype;
                SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
                    saga::BadParameter);
            }
        }

#if !defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
    // throw error 
        SAGA_OSSTREAM strm;
        strm << "replica::detail::get_db_type: "
                "no database_connection dbtype specified in configuration.";
        SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::BadParameter);
#endif
        return database_connection::db_sqlite;   // default to sqlite, if possible
    }

#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
    // generate connectstring for a sqlite3 back end
    inline std::string
    construct_sqlite3_connect(std::string const& connect, std::string dbname)
    {
        connectstring connectstr(connect);

        if (dbname.empty())
            dbname = connect;
        if (dbname.empty())
            dbname = "replica.db";

        return dbname;
    }
#endif

#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
    // generate connectstring for a postgresql back end
    template <typename InstanceData>
    inline std::string
    construct_pg_connect(std::string const& connect, saga::session const& s,
        saga::impl::proxy* p, std::string const& dbname)
    {
        InstanceData data(p);
        connectstring connectstr(connect);

    // set username and password from a (possibly) existing context
        std::vector<saga::context> ctxs(s.list_contexts());
        std::vector<saga::context>::iterator end = ctxs.end();
        for (std::vector<saga::context>::iterator it = ctxs.begin(); it != end; ++it) 
        {
            if ((*it).attribute_exists(saga::attributes::context_type) && 
                "default_replica_db" == (*it).get_attribute(saga::attributes::context_type))
            {
                if ((*it).attribute_exists(saga::attributes::context_userid)) {
                    connectstr.set_value("user",
                        (*it).get_attribute(saga::attributes::context_userid));
                }
                if ((*it).attribute_exists(saga::attributes::context_userpass)) {
                    connectstr.set_value("password",
                        (*it).get_attribute(saga::attributes::context_userpass));
                }
                break;
            }
        }

    // set the host name and port number
        saga::url lfn_url(data->location_);
        std::string host(lfn_url.get_host());
        if (!host.empty())
            connectstr.set_value("host", host);
        
        int port = lfn_url.get_port();
        if (-1 != port) 
            connectstr.set_value("port", boost::lexical_cast<std::string>(port));
        
    // set username and password from the url
        std::string username(lfn_url.get_username());
        if (!username.empty())
            connectstr.set_value("user", username);

        std::string password(lfn_url.get_password());
        if (!password.empty())
            connectstr.set_value("password", password);

    // make sure a dbname is given
        if (!dbname.empty())
            connectstr.set_value("dbname", dbname);
        else if (!connectstr.has_value("dbname"))
            connectstr.set_value("dbname", "replicadb");
            
        return connectstr.get();
    }
#endif // SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL

    // compose the connect string to the backend
    template <typename InstanceData>
    inline std::string 
    get_db_connectstring(saga::ini::ini const& ini, saga::session const& s,
        saga::impl::proxy* p)
    {
        // The url may specify the needed dbtype as a query parameter:
        // lfn://host/path?dbtype=sqlite3
        std::string dbtype;
        std::string dbname;
        retrieve_dbtype<InstanceData>(p, dbtype);
        retrieve_dbname<InstanceData>(p, dbname);

        // try to find database_connection back end
        if (ini.has_section("preferences")) 
        {
            saga::ini::ini prefs = ini.get_section ("preferences");

            // the default dbtype is given in the ini file
            if (dbtype.empty() && prefs.has_entry("dbtype"))
                dbtype = prefs.get_entry("dbtype");

            if (prefs.has_section (dbtype) )
                prefs = prefs.get_section (dbtype);

            std::string dbconnect;
            if (prefs.has_entry("dbconnect"))
                dbconnect = prefs.get_entry("dbconnect");

            if (!dbconnect.empty()) {
                switch (get_db_type<InstanceData>(ini, p)) {
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
                case database_connection::db_sqlite:
                return construct_sqlite3_connect(dbconnect, dbname);
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
                case database_connection::db_postgresql:
                    return construct_pg_connect<InstanceData>(dbconnect, s, p, dbname);
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
                case database_connection::db_mysql:
                    return construct_mysql_connect<InstanceData>(dbconnect, s, p, dbname);
#endif
                default:
                    return dbconnect;
                }
            }
        }

        switch (get_db_type<InstanceData>(ini, p)) {
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
        case database_connection::db_sqlite:
            return construct_sqlite3_connect("replica.db", dbname);
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
        case database_connection::db_postgresql:
            return construct_pg_connect<InstanceData>("dbname=replicadb", s, p, dbname);
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
        case database_connection::db_mysql:
            return construct_mysql_connect<InstanceData>("dbname=replicadb", s, p, dbname);
#endif

        default:
            break;
        }

    // throw error 
        SAGA_OSSTREAM strm;
        strm << "replica::detail::get_db_connectstring: "
                "no database_connection back end specified in configuration.";
        SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::BadParameter);
        return "";    // unreachable code
    }

    // generate a regex matching sub-clause for different db back ends
    inline std::string
    get_match (database_connection::db_type type, char const* lhs, char const* rhs)
    {
        switch(type) {
        // these databases use a special user define function 'match()'
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
        case database_connection::db_sqlite:
            return std::string("match(") + lhs + ", " + rhs + ")";
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
        case database_connection::db_postgresql:
            return std::string("match(") + lhs + ", " + rhs + ")";
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
        // MySql natively supports regular expressions
        case database_connection::db_mysql:
            return std::string(lhs) + " REGEXPR " + rhs;
#endif

        default:
            break;
        }

    // throw error 
        SAGA_OSSTREAM strm;
        strm << "replica::detail::get_match: "
                "no database_connection back end specified in configuration.";
        SAGA_ADAPTOR_THROW_NO_CONTEXT(SAGA_OSSTREAM_GETSTRING(strm), 
            saga::BadParameter);
        return "";    // unreachable code
    }

    ///////////////////////////////////////////////////////////////////////////
    inline std::string 
    get_filepath(saga::url const& loc)
    {
        return saga::url::unescape(loc.get_path());
    }

    ///////////////////////////////////////////////////////////////////////////
    // verify the hostname will be understood by our adaptor
    inline bool 
    host_is_valid(database_connection::db_type type, std::string const& host)
    {
        switch (type) {
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_SQLITE3)
        case database_connection::db_sqlite:
            return saga::adaptors::utils::is_local_address(host);
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_POSTGRESQL)
        case database_connection::db_postgresql:
            return true;
#endif
#if defined(SAGA_DEFAULT_REPLICA_ADAPTOR_USE_MYSQL)
        case database_connection::db_mysql:
            return true;
#endif

        default:
            break;
        }
        return false;
    }

    // convert a string attribute in a vector attribute by splitting the 
    // attribute value at ',' symbols
    inline void
    tokenize_values(std::string val, std::vector<std::string>& values)
    {
        typedef boost::char_separator<char> char_separator_type;
        typedef boost::tokenizer<char_separator_type> tokenizer_type;

        char_separator_type sep (",");
        tokenizer_type tok (val, sep);
        tokenizer_type::iterator tok_end = tok.end();
        for (tokenizer_type::iterator tit = tok.begin (); tit != tok_end; ++tit)
        {
            values.push_back (*tit);
        }
    }

    // convert a vector attribute into a single value by concatenating the 
    // values into a list separated by ',' symbols
    inline std::string 
    concat_values(std::vector<std::string> const& values)
    {
        std::string result;
        std::vector<std::string>::const_iterator end = values.end();
        for (std::vector<std::string>::const_iterator it = values.begin();
             it != end; /**/)
        {
            result += *it;
            if (++it != end)
                result += ",";
        }
        return result;
    }

    // convert a SAGA pattern into a TR1::regex 
    inline std::string 
    regex_from_character_set(std::string::const_iterator& it, 
        std::string::const_iterator end)
    {
        std::string::const_iterator start = it;
        std::string result(1, *it);  // copy '['
        if (*++it == '!') {
            result.append(1, '^');   // negated character set
        }
        else if (*it == ']') {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                "Invalid pattern (empty character set) at: " + 
                std::string(start, end), saga::NoSuccess);
        }
        else {
            result.append(1, *it);   // append this character
        }

        // copy while in character set
        while (++it != end) {
            result.append(1, *it);
            if (*it == ']')
                break;
        }

        if (it == end || *it != ']') {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                "Invalid pattern (missing closing ']') at: " + 
                std::string(start, end), saga::NoSuccess);
        }

        return result;
    }

    inline std::string 
    regex_from_string_choice(std::string::const_iterator& it, 
        std::string::const_iterator end)
    {
        std::string::const_iterator start = it;
        std::string result(1, '(');

        // copy while in string choice
        bool has_one = false;
        while (++it != end && *it != '}') {
            if (*it == ',') {
                if (!has_one) {
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(
                        "Invalid pattern (empty string alternative) at: " + 
                        std::string(start, end), saga::NoSuccess);
                }
                result.append(1, '|');
                has_one = false;
            }
            else {
                result.append(1, *it);
                has_one = true;
            }
        }

        if (it == end || *it != '}') {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                "Invalid pattern (missing closing '}') at: " + 
                std::string(start, end), saga::NoSuccess);
        }

        result.append(1, ')');
        return result;
    }

    inline std::string 
    regex_from_pattern(std::string const& pattern)
    {
        std::string result;
        std::string::const_iterator end = pattern.end();
        for (std::string::const_iterator it = pattern.begin(); it != end; ++it)
        {
            char c = *it;
            switch (c) {
            case '*':
                result.append(".*");
                break;

            case '?':
                result.append(1, '.');
                break;

            case '[':
                result.append(regex_from_character_set(it, end));
                break;
                
            case '{':
                result.append(regex_from_string_choice(it, end));
                break;

            case '\\':
                if (++it == end) {
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(
                        "Invalid escape sequence at: " + pattern,
                        saga::NoSuccess);
                }
                result.append(1, *it);
                break;

            default:
                result.append(1, c);
                break;
            }
        }
        return result;
    }

    inline void
    regex_from_patterns(std::vector<std::string> const& patterns,
        std::vector<std::string>& result)
    {
        std::transform(patterns.begin(), patterns.end(), std::back_inserter(result),
            regex_from_pattern);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void 
    parse_pattern(saga::impl::v1_0::cpi const* target, 
        std::string const& pattern, std::string& kpat, std::string& vpat)
    {
        std::string::size_type p = pattern.find_first_of("=");
        if (std::string::npos == p) {
            SAGA_ADAPTOR_THROW_VERBATIM(target, 
                "Pattern has invalid format: " + pattern, saga::BadParameter);
        }
        kpat = pattern.substr(0, p);
        vpat = pattern.substr(p+1);
    }

    template <typename Iterator>
    inline void 
    parse_patterns(saga::impl::v1_0::cpi const* target, 
        std::vector<std::string> const& patterns, Iterator kit, Iterator vit)
    {
        std::vector<std::string>::const_iterator end = patterns.end();
        for (std::vector<std::string>::const_iterator it = patterns.begin(); 
             it != end; ++it, ++kit, ++vit)
        {
            std::string kpat, vpat;
            parse_pattern(target, *it, kpat, vpat);
            *kit = kpat;
            *vit = vpat;
        }
    }

    inline void 
    parse_patterns(saga::impl::v1_0::cpi const* target, 
        std::vector<std::string> const& patterns, std::vector<std::string>& kpat, 
        std::vector<std::string>& vpat)
    {
        parse_patterns(target, patterns, std::back_inserter(kpat),
            std::back_inserter(vpat));
    }

///////////////////////////////////////////////////////////////////////////////
}}  // namespace replica::detail

#endif // !ADAPTORS_DEFAULT_REPLICA_COMMON_HELPERS_HPP

