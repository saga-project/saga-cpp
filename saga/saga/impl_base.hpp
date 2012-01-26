//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_BASE_HPP
#define SAGA_IMPL_ENGINE_BASE_HPP

#include <cstdlib>
#include <cstddef>

#include <saga/saga/export_definitions.hpp>
#include <saga/saga/detail.hpp>
#include <saga/saga/detail/module_needs_logging.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/logging/format/named_write.hpp>
#include <boost/logging/format_fwd.hpp>

BOOST_LOG_FORMAT_MSG(optimize::cache_string_one_str<>)

///////////////////////////////////////////////////////////////////////////////
#define SAGA_CPIVERSION_1_0       0x0100
#define SAGA_CPIVERSION_MINORMASK 0x000F
#define SAGA_CPIVERSION_LATEST    SAGA_CPIVERSION_1_0

///////////////////////////////////////////////////////////////////////////////

/// @cond
/** Hide from Doxygen */
namespace saga
{
  namespace adaptors 
  {
    class task;
    class attribute;
    class metric;
    class exception;
  }

  // forward declaration for implementation
  namespace impl
  {
    struct runtime;
    struct job_runtime;
    class exception;

    class url;
    class uuid;
    class const_buffer;
    class buffer;
    
    class object;
    class session;
    class context;
    class attribute;
    class permissions;
    struct permissions_interface;
    
    struct task_interface;
    class task_base;
    class task_container;
    class adaptor_selector_state;

    class namespace_entry;
    class namespace_dir;

    class directory;
    class file;
    class const_iovec;
    class iovec;

    class logical_directory;
    class logical_file;

    class advert;
    class advert_directory;

    class rpc;
    class parameter;
    
    class stream;
    class server;

    class job;
    class self;
    class job_service;
    class description;
    class istream_interface;
    class ostream_interface;

    class service_description;
    class service_data;
    class discoverer;
    
    class cpr_job;
    class cpr_job_service;
    class cpr_checkpoint;
    class cpr_directory;
    class cpr_job_description;

    class metric;
//  class monitor;
//  class actor;

    class adaptor_selector;

    class proxy;

    namespace v1_0 
    {
      class cpi;
      class cpi_info;
      class op_info;
    }
    
    // interfaces
    class attribute;
    class monitorable;
    class steerable;
    struct task_interface;
    
    // exception handling
    class exception_list;

    // special void class    
    struct void_t {};
  } // namespace impl

  namespace ini
  {
    class section;
  }
} // namespace saga

/// @endcond

///////////////////////////////////////////////////////////////////////////////
//
// Macros for verbose and debug output
//

namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    SAGA_EXPORT std::string levelname(int level);
    SAGA_EXPORT std::string logging_filename(char const* filename, int lineno);
    SAGA_EXPORT std::vector<std::string> const& get_logging_init_data();
    SAGA_EXPORT void init_logging(saga::ini::section const& ini);

    ///////////////////////////////////////////////////////////////////////////
    typedef boost::logging::named_logger<>::type logger_type;
    typedef boost::logging::level::holder filter_type;

    ///////////////////////////////////////////////////////////////////////////
    SAGA_EXPORT BOOST_DECLARE_LOG_FILTER(logger_level, filter_type)
    SAGA_EXPORT BOOST_DECLARE_LOG(logger, logger_type)
}}

///////////////////////////////////////////////////////////////////////////////
#define SAGA_LOG_USE_LOG_IF_LEVEL_AND_MDOULE(l, holder, the_level)            \
    BOOST_LOG_USE_LOG(l, read_msg().gather().out(),                           \
        holder->is_enabled(::boost::logging::level::the_level) &&             \
        saga::detail::module_needs_logging(                                   \
            BOOST_PP_STRINGIZE(SAGA_MANGLED_MODULE_NAME)))                    \
/**/

///////////////////////////////////////////////////////////////////////////////
#define SAGA_LOG(lvl) SAGA_LOG_USE_LOG_IF_LEVEL_AND_MDOULE(                   \
    saga::impl::logger(), saga::impl::logger_level(), lvl)                    \
        << BOOST_PP_STRINGIZE(SAGA_MODULE_NAME) << ": "                       \
        << saga::impl::levelname(::boost::logging::level::lvl) << ": "        \
        << saga::impl::logging_filename(__FILE__, __LINE__) << " "            \
/**/

#define SAGA_VERBOSE(lvl)                                                     \
    if (saga::impl::logger_level()->is_enabled(::boost::logging::level::lvl)  \
     && saga::detail::module_needs_logging(                                   \
            BOOST_PP_STRINGIZE(SAGA_MANGLED_MODULE_NAME)))                    \
/**/

///////////////////////////////////////////////////////////////////////////////
#define SAGA_LOG_ALWAYS(msg)                                                  \
        SAGA_LOG(SAGA_VERBOSE_LEVEL_ALWAYS) << msg;                           \
  /**/

#define SAGA_LOG_CRITICAL(msg)                                                \
        SAGA_LOG(SAGA_VERBOSE_LEVEL_CRITICAL) << msg;                         \
  /**/

#define SAGA_LOG_ERROR(msg)                                                   \
        SAGA_LOG(SAGA_VERBOSE_LEVEL_ERROR) << msg;                            \
  /**/

#define SAGA_LOG_WARN(msg)                                                    \
        SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING) << msg;                          \
  /**/

#define SAGA_LOG_INFO(msg)                                                    \
        SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) << msg;                             \
  /**/

#define SAGA_LOG_DEBUG(msg)                                                   \
        SAGA_LOG(SAGA_VERBOSE_LEVEL_DEBUG) << msg;                            \
  /**/

#define SAGA_LOG_BLURB(msg)                                                   \
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) << msg;                            \
  /**/

#define SAGA_VERBOSE_LEVEL_ALWAYS         enable_all
#define SAGA_VERBOSE_LEVEL_CRITICAL       fatal
#define SAGA_VERBOSE_LEVEL_ERROR          error
#define SAGA_VERBOSE_LEVEL_WARNING        warning
#define SAGA_VERBOSE_LEVEL_INFO           info
#define SAGA_VERBOSE_LEVEL_DEBUG          debug
#define SAGA_VERBOSE_LEVEL_BLURB          debug

///////////////////////////////////////////////////////////////////////////////
// Helper macros/functions to overcome the lack of stringstream on certain 
// architectures
#ifdef BOOST_NO_STRINGSTREAM
# include <strstream>
  inline std::string SAGA_OSSTREAM_GETSTRING (std::ostrstream & ss)
  {
    ss << std::ends;
    std::string rval = ss.str ();
    ss.freeze (false);
    return (rval);
  }
# define SAGA_OSSTREAM std::ostrstream
# define SAGA_ISSTREAM std::istrstream
#else
# include <sstream>
# define SAGA_OSSTREAM_GETSTRING(ss) ss.str()
# define SAGA_OSSTREAM std::ostringstream
# define SAGA_ISSTREAM std::istringstream
#endif

///////////////////////////////////////////////////////////////////////////////
#if defined(__AIX__) && defined(__GNUC__)
// we must call the initialization functions of the shared libraries ourselves, 
// because AIX's dlopen() system call does not know about it. 
extern "C" {
    void _GLOBAL__DI();   // call constructors of all global objects
    void _GLOBAL__DD();   // call destructors of all global objects
}
#endif

#endif // SAGA_IMPL_ENGINE_BASE_HPP


