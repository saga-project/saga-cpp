//  Copyright (c) 2005-2010 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/path_leaf.hpp>
#include <saga/saga/adaptors/utils/ini/ini.hpp>

#include <boost/spirit/core/non_terminal/impl/static.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/logging/format/named_write.hpp>
#include <boost/logging/format/destination/defaults.hpp>

/// @cond
/** These methods are not within API scope */
namespace saga { namespace impl 
{
    ///////////////////////////////////////////////////////////////////////////
    int get_log_level(std::string const& env)
    {
        try {
            int saga_level = boost::lexical_cast<int>(env);
            if (saga_level <= 0)
                return boost::logging::level::disable_all;

            switch (saga_level) {
            case 1: return boost::logging::level::fatal;
            case 2: return boost::logging::level::error;
            case 3: return boost::logging::level::warning;
            case 4: return boost::logging::level::info;
            default:
                break;
            }
            return boost::logging::level::debug;
        }
        catch (boost::bad_lexical_cast const&) {
            return boost::logging::level::disable_all;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    std::string levelname(int level)
    {
        switch (level) {
        case boost::logging::level::enable_all:
            return "LOG";
        case boost::logging::level::debug:
            return "DEBUG";
        case boost::logging::level::info:
            return "INFO";
        case boost::logging::level::warning:
            return "WARNING";
        case boost::logging::level::error:
            return "ERROR";
        case boost::logging::level::fatal:
            return "CRITICAL";
        }
        return "LOG(" + boost::lexical_cast<std::string>(level) + ")";
    }

    ///////////////////////////////////////////////////////////////////////////
    std::string logging_filename(char const* filename, int lineno)
    {
    #if BOOST_FILESYSTEM_VERSION == 3
        boost::filesystem::path p(filename);    
    #else
        boost::filesystem::path p(filename, boost::filesystem::native);
    #endif
    
        std::string s = saga::detail::leaf(p);
        s += std::string("(") + boost::lexical_cast<std::string>(lineno) + "):";
        return s;
    }

    ///////////////////////////////////////////////////////////////////////////
    // unescape config entry
    std::string unescape(std::string const &value)
    {
        std::string result;
        std::string::size_type pos = 0;
        std::string::size_type pos1 = value.find_first_of ("\\", 0);
        if (std::string::npos != pos1) {
            do {
                switch (value[pos1+1]) {
                case '\\':
                case '\"':
                case '?':
                    result = result + value.substr(pos, pos1-pos);
                    pos1 = value.find_first_of ("\\", (pos = pos1+1)+1);
                    break;

                case 'n':
                    result = result + value.substr(pos, pos1-pos) + "\n";
                    pos1 = value.find_first_of ("\\", pos = pos1+1);
                    ++pos;
                    break;

                default:
                    result = result + value.substr(pos, pos1-pos+1);
                    pos1 = value.find_first_of ("\\", pos = pos1+1);
                }

            } while (pos1 != std::string::npos);
            result = result + value.substr(pos);
        }
        else {
        // the string doesn't contain any escaped character sequences
            result = value;
        }
        return result;
    }

    ///////////////////////////////////////////////////////////////////////////
    // this is required in order to use the logging library
    BOOST_DEFINE_LOG_FILTER_WITH_ARGS(logger_level, filter_type, 
        boost::logging::level::disable_all) 
    BOOST_DEFINE_LOG(logger, logger_type) 

    ///////////////////////////////////////////////////////////////////////////
    // the Boost.Log generates bogus milliseconds on Linux systems
    #if defined(SAGA_WINDOWS)
    #define SAGA_TIMEFORMAT "$hh:$mm.$ss.$mili"
    #else
    #define SAGA_TIMEFORMAT "$hh:$mm.$ss"
    #endif

    struct logging_configuration
    {
        logging_configuration();
        std::vector<std::string> prefill_; 
    };

    logging_configuration::logging_configuration()
    {
        try {
            // add default logging configuration as defaults to the ini data
            // this will be overwritten by related entries in the read hpx.ini
            using namespace boost::assign;
            prefill_ +=
            // general logging
                "[saga.logging]",
                "level = ${SAGA_VERBOSE:0}",
                "destination = ${SAGA_LOGDESTINATION:file(saga.$[saga.pid].log)}",
                "format = ${SAGA_LOGFORMAT:%time%(" SAGA_TIMEFORMAT ") [%idx%] |\\n}",
                "modules = ${SAGA_LOGMODULES:*}",
                "engine = ${SAGA_LOGENGINE:1}"
            ;
        }
        catch (std::exception const&) {
            // just in case something goes wrong
            std::cerr << "caught std::exception during initialization" 
                      << std::endl;
        }
    }

    #undef SAGA_TIMEFORMAT

    struct init_logging_tag {};
    std::vector<std::string> const& get_logging_init_data()
    {
        boost::spirit::static_<logging_configuration, init_logging_tag> init;
        return init.get().prefill_;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Initialize logging
    struct initialize_logging
    {
        initialize_logging(saga::ini::section const& ini)
        {
            std::string loglevel, logdest, logformat;
            if (ini.has_section_full("saga.logging")) {
                saga::ini::section logini (ini.get_section("saga.logging"));

                std::string empty;
                loglevel = logini.get_entry("level", empty);
                logdest = logini.get_entry("destination", empty);
                logformat = unescape(logini.get_entry("format", empty));
            }

            unsigned int lvl = boost::logging::level::disable_all;
            if (!loglevel.empty()) 
                lvl = get_log_level(loglevel);

            if (static_cast <unsigned int> (boost::logging::level::disable_all) != lvl) 
            {
                if (logdest.empty())      // ensure minimal defaults
                    logdest = "cerr";
                if (logformat.empty())
                    logformat = "|\\n";

                logger()->writer().write(logformat, logdest);
                logger()->mark_as_initialized();
                logger_level()->set_enabled(lvl);
            }
        }
    };

    void init_logging(saga::ini::section const& ini)
    {
        boost::spirit::static_<initialize_logging, init_logging_tag> init(ini);
    }
}}
/// @endcond

