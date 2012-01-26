//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_UUID_HPP
#define SAGA_IMPL_ENGINE_UUID_HPP

#include <string>
#include <iosfwd>

#include <boost/thread.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
// #include <saga/impl/engine/uuid/saga_uuid.h>
#include <saga/impl/exception.hpp>

// FIXME: impl should only get pulled in cpp -- move implementation of methods
// into cpp

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

///////////////////////////////////////////////////////////////////////////////
class uuid 
{
  private:
    mutable boost::uuids::uuid uuid_;

// protect access to uuid_create_... functions
  protected:
    static boost::mutex &mutex_instance()
    {
        static boost::mutex mutex;
        return mutex;
    }
    typedef boost::uuids::random_generator uuid_generator_type;
    static bool is_null(boost::uuids::uuid const& id)
    {
        return id.is_nil();
    }
    static void from_string(boost::uuids::uuid& id, std::string const& str)
    {
        id = boost::uuids::string_generator()(str);
    }
    static uuid_generator_type& uuid_generator_instance()
    {
        static uuid_generator_type gen;
        return gen;
    }
    static void mutex_init()
    {
        mutex_instance();
        uuid_generator_instance();
    }
    static void initialize_mutex()
    {
        static boost::once_flag been_here = BOOST_ONCE_INIT;
        boost::call_once(mutex_init, been_here);
    }

  public:
    uuid (void) 
      : uuid_(boost::uuids::nil_generator()())
    {
    }

    uuid (char const * uuid_str)  
    {
        create (uuid_str);
    }

    ~uuid()
    {
    }

    void ensure_is_initialized() const
    {
        if (is_null(uuid_))
            create();

        if (is_null(uuid_))
        {
            SAGA_THROW_NO_OBJECT("Could not create UUID for this object", 
              saga::NoSuccess);
        }
    }

    std::string string (void) const
    {
        ensure_is_initialized();
        return boost::lexical_cast<std::string>(uuid_);
    }

    // create new uuid from scratch
    void create (void) const
    {
        initialize_mutex();
        boost::mutex::scoped_lock lock(mutex_instance());
        uuid_ = uuid_generator_instance()();
    }

    // create new uuid from string
    void create (char const* str, size_t len = 0) const
    {
        initialize_mutex();
        boost::mutex::scoped_lock lock(mutex_instance());
        from_string(uuid_, str);
    }

    // comparison operators
    friend bool operator== (uuid const & lhs, uuid const & rhs)
    {
        lhs.ensure_is_initialized();
        rhs.ensure_is_initialized();

        return lhs.uuid_ == rhs.uuid_;
    }

    friend bool operator!= (uuid const & lhs, uuid const & rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator< (uuid const & lhs, uuid const & rhs)
    {
        lhs.ensure_is_initialized();
        rhs.ensure_is_initialized();

        return lhs.uuid_ < rhs.uuid_;
    }
    
    friend bool operator> (uuid const & lhs, uuid const & rhs)
    {
        lhs.ensure_is_initialized();
        rhs.ensure_is_initialized();

        return lhs.uuid_ > rhs.uuid_;
    }

    friend bool operator<= (uuid const & lhs, uuid const & rhs)
    {
      return !(lhs > rhs);
    }

    friend bool operator>= (uuid const & lhs, uuid const & rhs)
    {
      return !(lhs < rhs);
    }

    // streaming operators
//     friend std::ostream & operator<< (std::ostream & ostrm, 
//                                       uuid const   & rhs)
//     {
//       return ostrm << rhs.string ();
//     }
// 
//     friend std::istream & operator>> (std::istream & istrm, 
//                                       uuid         & rhs)
//     {
//       std::string instr;
//      
//       istrm >> instr;
//       
//       rhs.create (instr.c_str (), instr.size ());
// 
//       return istrm;
//     }
};

uuid const null_uuid = uuid();

///////////////////////////////////////////////////////////////////////////////
} }   // namespace saga::impl

#endif // SAGA_SAGA_ENGINE_UUID_UUID_HPP

