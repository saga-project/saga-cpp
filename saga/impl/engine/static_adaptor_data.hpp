//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_ENGINE_STATIC_ADAPTOR_DATA_HPP)
#define SAGA_ENGINE_STATIC_ADAPTOR_DATA_HPP

///////////////////////////////////////////////////////////////////////////////
#include <boost/config.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/plugin.hpp>

struct static_adaptor_load_data_type
{
    char const* const name;     // adaptor name
    void (*force_load)();       // function to force linking adaptor
    boost::plugin::get_plugins_list_type get_factory;
};

struct static_package_load_data_type
{
    char const* const name;     // package name
    void (*force_load)();       // function to force linking package
    unsigned long (*get_version)(); // function to force linking version API
};

///////////////////////////////////////////////////////////////////////////////
#define SAGA_LITE_DECLARE_ADAPTOR(name)                                               \
    extern "C" void BOOST_PLUGIN_FORCE_LOAD_NAME(SAGA_MANGLE_ADAPTOR_NAME(name))();   \
    extern "C" BOOST_PLUGIN_EXPORT_API std::map<std::string, boost::any>&             \
        BOOST_PLUGIN_API BOOST_PLUGIN_LIST_NAME(SAGA_MANGLE_ADAPTOR_NAME(name))()     \
    /**/
    
#define SAGA_LITE_DEFINE_ADAPTOR(name)                                                \
    {                                                                                 \
        BOOST_PP_STRINGIZE(name),                                                     \
        BOOST_PLUGIN_FORCE_LOAD_NAME(SAGA_MANGLE_ADAPTOR_NAME(name)),                 \
        BOOST_PLUGIN_LIST_NAME(SAGA_MANGLE_ADAPTOR_NAME(name))                        \
    }                                                                                 \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define SAGA_LITE_DECLARE_PACKAGE(name)                                               \
    extern "C" void BOOST_PLUGIN_FORCE_LOAD_NAME(SAGA_MANGLE_PACKAGE_NAME(name))();   \
    namespace saga { unsigned long get_ ## name ## _package_version(); }              \
    /**/
    
#define SAGA_LITE_DEFINE_PACKAGE(name)                                                \
    {                                                                                 \
        BOOST_PP_STRINGIZE(name),                                                     \
        BOOST_PLUGIN_FORCE_LOAD_NAME(SAGA_MANGLE_PACKAGE_NAME(name)),                 \
        &saga::get_ ## name ## _package_version                                       \
    }                                                                                 \
    /**/

///////////////////////////////////////////////////////////////////////////////
// prototypes of functions used to force linking of adaptors
#if defined(BUILD_SAGA_LITE)
    SAGA_LITE_DECLARE_ADAPTOR(default_advert);
    SAGA_LITE_DECLARE_ADAPTOR(default_file);
    SAGA_LITE_DECLARE_ADAPTOR(default_job);
    SAGA_LITE_DECLARE_ADAPTOR(default_replica);
    SAGA_LITE_DECLARE_ADAPTOR(default_rpc);
    SAGA_LITE_DECLARE_ADAPTOR(default_sd);
    SAGA_LITE_DECLARE_ADAPTOR(default_stream);
#endif

// table of adaptors to link statically 
static_adaptor_load_data_type static_adaptor_load_data[] = 
{
#if defined(BUILD_SAGA_LITE)
    SAGA_LITE_DEFINE_ADAPTOR(default_advert),
    SAGA_LITE_DEFINE_ADAPTOR(default_file),
    SAGA_LITE_DEFINE_ADAPTOR(default_job),
    SAGA_LITE_DEFINE_ADAPTOR(default_replica),
    SAGA_LITE_DEFINE_ADAPTOR(default_rpc),
    SAGA_LITE_DEFINE_ADAPTOR(default_sd),
    SAGA_LITE_DEFINE_ADAPTOR(default_stream),
#endif
    { NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////
// prototypes of functions used to force linking of packages
#if defined(BUILD_SAGA_LITE)
    SAGA_LITE_DECLARE_PACKAGE(advert);
    SAGA_LITE_DECLARE_PACKAGE(filesystem);
    SAGA_LITE_DECLARE_PACKAGE(job);
    SAGA_LITE_DECLARE_PACKAGE(namespace);
    SAGA_LITE_DECLARE_PACKAGE(replica);
    SAGA_LITE_DECLARE_PACKAGE(rpc);
    SAGA_LITE_DECLARE_PACKAGE(sd);
    SAGA_LITE_DECLARE_PACKAGE(stream);
#endif

// table of packages to link statically 
static_package_load_data_type static_package_load_data[] = 
{
#if defined(BUILD_SAGA_LITE)
    SAGA_LITE_DEFINE_PACKAGE(advert),
    SAGA_LITE_DEFINE_PACKAGE(filesystem),
    SAGA_LITE_DEFINE_PACKAGE(job),
    SAGA_LITE_DEFINE_PACKAGE(namespace),
    SAGA_LITE_DEFINE_PACKAGE(replica),
    SAGA_LITE_DEFINE_PACKAGE(rpc),
    SAGA_LITE_DEFINE_PACKAGE(sd),
    SAGA_LITE_DEFINE_PACKAGE(stream),
#endif
    { NULL, NULL }
};

///////////////////////////////////////////////////////////////////////////////
// enable auto-linking on supported platforms
#if defined(BOOST_WINDOWS) && defined(BUILD_SAGA_LITE)
// auto-link adaptors
    #define SAGA_AUTOLINK_LIB_NAME "default_advert"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "default_file"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "default_job"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "default_replica"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "default_rpc"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "default_sd"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "default_stream"
    #include <saga/saga/autolink.hpp>
// auto-link packages
    #define SAGA_AUTOLINK_LIB_NAME "advert"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "filesystem"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "job"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "namespace"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "replica"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "rpc"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "sd"
    #include <saga/saga/autolink.hpp>
    #define SAGA_AUTOLINK_LIB_NAME "stream"
    #include <saga/saga/autolink.hpp>
#endif

#endif

