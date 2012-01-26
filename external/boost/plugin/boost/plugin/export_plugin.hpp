// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_EXPORT_PLUGIN_VP_2004_08_25
#define BOOST_EXPORT_PLUGIN_VP_2004_08_25

#include <string>
#include <map>

#include <boost/config.hpp>
#include <boost/any.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/plugin/config.hpp>
#include <boost/plugin/concrete_factory.hpp>

///////////////////////////////////////////////////////////////////////////////
#define BOOST_PLUGIN_LIST_NAME(name)                                          \
    BOOST_PP_CAT(boost_exported_plugins_list_, name)                          \
    /**/
    
///////////////////////////////////////////////////////////////////////////////
#define BOOST_PLUGIN_FORCE_LOAD_NAME(name)                                    \
    BOOST_PP_CAT(boost_exported_plugins_force_load_, name)                    \
    /**/
    
///////////////////////////////////////////////////////////////////////////////
#define BOOST_PLUGIN_EXPORT(name, BaseType, ActualType, classname)            \
    extern "C" BOOST_PLUGIN_EXPORT_API std::map<std::string, boost::any>&     \
               BOOST_PLUGIN_API BOOST_PLUGIN_LIST_NAME(name)();               \
                                                                              \
    namespace {                                                               \
        struct BOOST_PP_CAT(boost_plugin_exporter_, name) {                   \
            BOOST_PP_CAT(boost_plugin_exporter_, name)()                      \
            {                                                                 \
                static boost::plugin::concrete_factory<BaseType, ActualType> cf; \
                boost::plugin::abstract_factory<BaseType>* w = &cf;           \
                BOOST_PLUGIN_LIST_NAME(name)().insert(                        \
                    std::make_pair(classname, w));                            \
            }                                                                 \
        } BOOST_PP_CAT(boost_plugin_exporter_instance_, name);                \
    }                                                                         \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define BOOST_PLUGIN_EXPORT_LIST(name)                                        \
    extern "C" BOOST_PLUGIN_EXPORT_API std::map<std::string, boost::any>&     \
        BOOST_PLUGIN_API BOOST_PLUGIN_LIST_NAME(name)()                       \
    {                                                                         \
        static std::map<std::string, boost::any> r;                           \
        return r;                                                             \
    }                                                                         \
    extern "C" BOOST_PLUGIN_EXPORT_API                                        \
        void BOOST_PLUGIN_FORCE_LOAD_NAME(name)()                             \
    {                                                                         \
        BOOST_PLUGIN_LIST_NAME(name)();                                       \
    }                                                                         \
    /**/

#endif

