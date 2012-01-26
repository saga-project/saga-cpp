//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <stdlib.h>

#include <boost/regex.hpp>

#include "saga/saga.hpp"
#include "saga/saga/sd.hpp"

bool compare_nocase(const std::string& first, const std::string& second) {
    size_t len = 0;
    while ((len < first.length()) && (len < second.length())) {
        if (std::tolower(first[len]) < std::tolower(second[len])) {
            return true;
        }

        else if (std::tolower(first[len]) > tolower(second[len])) {
            return false;
        }
        ++len;
    }

    if (first.length() < second.length()) {
        return true;
    }

    else {
        return false;
    }
}

/**
 * Function to return value from a string. A check is made that there is nothing else after valid input apart from white space.
 */
template<class T>
bool from_string(T& t, const std::string& s) {
    std::istringstream iss(s);
    if ((iss >> t).fail()) {
        return false;
    }
    int eof;
    iss >> eof;
    return iss.fail() && iss.eof();
}

/**
 * Get next parameter safely
 */
std::string getNext(int & optind, int argc, char *argv[]) {
    if (optind >= argc-1) {
        std::cerr << argv[optind] << " must be followed by a value" << std::endl;
        exit(1);
    }
    ++optind;
    return argv[optind];
}

/**
 * Trim blanks from each end of a string
 */
std::string trim(std::string& s) {   
    std::string::size_type os = s.find_first_not_of(" ");
    std::string::size_type cs = s.find_last_not_of(" ");
    return s.substr(os, cs-os+1);
}

/**
 * Main program
 */
int main(int argc, char *argv[]) {
#if !defined(SAGA_HAVE_PACKAGE_SD)
  std::cerr << "saga-sd: saga has been compiled without the sd "
               "package, bailing out" << std::endl;
  return -1;
#else
    std::string svcFilter = "";
    std::string authzFilter = "";
    std::string dataFilter = "";
    std::string serverUrl = ""; 
    std::vector<std::string> contexts;
    bool haveAuthzFilter = false;
    bool sa = false;
    bool da = false;
    int maxCount = (std::numeric_limits<int>::max)();

    int optind = 1;
    while ((optind < argc) && (argv[optind][0] == '-')) {
        std::string sw = argv[optind];

        if (sw == "--context") {
          contexts.push_back(getNext(optind, argc, argv));
        } else if (sw == "--sf") {
            svcFilter = getNext(optind, argc, argv);
        } else if (sw == "--af") {
            authzFilter = getNext(optind, argc, argv);
            haveAuthzFilter = true;
        } else if (sw == "--df") {
            dataFilter = getNext(optind, argc, argv);
        } else if (sw == "--url") {
            serverUrl = getNext(optind, argc, argv);
        } else if (sw == "--da") {
            da = true;
        } else if (sw == "--sa") {
            sa = true;
        } else if (sw == "-n" || sw == "--count") {
            std::string val = getNext(optind, argc, argv);
            if (!from_string<int> (maxCount, val) || maxCount <= 0) {
                std::cerr << "Count must be a positive integer and not: " << argv[optind] << std::endl;
                exit(1);
            }
        } else if (sw == "-h" || sw == "--help") {
            std::cout << std::endl;
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "   --context <context>   context (may be many) with optional attributes in form context1(p1=v1,p2=v2)" << std::endl;
            std::cout << "   --sf <service filter> the default is no filtering on service attributes" << std::endl;
            std::cout << "   --df <data filter>    the default is no filtering on service data" << std::endl;
            std::cout << "   --af <authz filter>   the default is derived from the context" << std::endl;
            std::cout << "   --count, -n           the number of services to display - default unlimited" << std::endl;
            std::cout << "   --sa                  display service attributes " << std::endl;
            std::cout << "   --da                  display data attributes" << std::endl;
            std::cout << "   --url <server url>    overrides any environment variables" << std::endl;
            std::cout << "   --help, -h            print this message" << std::endl;
            std::cout << std::endl;
            return 0;
        } else {
            std::cerr << "Unknown switch: " << argv[optind] << std::endl;
            std::cerr << "Run with -h for help" << std::endl;
            exit(1);
        }
        optind++;
    }

    saga::session s;
    std::vector<std::string>::const_iterator pos;
    for (pos = contexts.begin(); pos != contexts.end(); pos++) {
        std::string::size_type op,cp,ep;
        op = (*pos).find("(");
        cp = (*pos).rfind(")");
        if (op != std::string::npos && cp != std::string::npos) {
            std::string parms = (*pos).substr(op+1, cp-op-1);
            std::string contextName = (*pos).substr(0,op);
            contextName = trim(contextName);
            saga::context ctx(contextName);
            std::string::size_type here = 0;
            while ( (ep = parms.find("=", here))  != std::string::npos ) {
                std::string att = parms.substr(here, ep-here);
                att = trim(att);
                here = ep + 1;
                cp = parms.find(",", here);
                if (cp !=  std::string::npos) {
                    std::string value = parms.substr(here, cp-here);
                    value = trim(value);
                    here = cp + 1;
                    ctx.set_attribute(att,value);
                } else {
                    std::string value = parms.substr(here);
                    value = trim(value);
                    ctx.set_attribute(att,value);
                    break;
                }
            } ;
            s.add_context(ctx);
        } else {
            std::string contextName = *pos;
            contextName = trim(contextName);
            saga::context ctx(contextName);
            s.add_context(ctx);
        }
    }

    saga::sd::discoverer sd(s, serverUrl);
    std::vector<saga::sd::service_description> services;

    try {
        if (haveAuthzFilter) {
            services = sd.list_services(svcFilter, dataFilter, authzFilter);
        } else {
            services = sd.list_services(svcFilter, dataFilter);
        }
    }

    catch (saga::exception& e) {
        std::cerr << e.get_message() << std::endl << "Exiting." << std::endl;
        exit(1);
    }

    catch (std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << std::endl;
        exit(1);
    }

    catch (...) {
        std::cerr << "Caught something unexpected." << std::endl;
        exit(1);
    }

    std::vector<saga::sd::service_description>::iterator servicesIter;
    std::vector<saga::sd::service_description>::const_iterator servicesEnd = services.end();

    for (servicesIter = services.begin(); servicesIter != servicesEnd && maxCount > 0; ++servicesIter, --maxCount) {
        using namespace saga::sd::attributes;

        if (da || sa) {
            std::cout << std::endl;
        }

        std::vector<std::string> attribNames = servicesIter->list_attributes();

        // Deal with the URL if it exists - it should!
        std::vector<std::string>::iterator pos = find(attribNames.begin(), attribNames.end(), "Url");
        if (pos == attribNames.end()) {
            std::cout << "Url : is not known" << std::endl;
        } else {
            std::cout << "Url : " << servicesIter->get_attribute("Url") << std::endl;
            attribNames.erase(pos);
        }

        if (sa) {
            //Sort our array of attribute names so they come out in a decent order
            sort(attribNames.begin(), attribNames.end(), compare_nocase);

            for (std::vector<std::string>::const_iterator attribNamesIter = attribNames.begin(); attribNamesIter
                    != attribNames.end(); ++attribNamesIter) {
                if (!servicesIter->attribute_is_vector(*attribNamesIter)) {
                    std::string attribValue = servicesIter->get_attribute(*attribNamesIter);
                    std::cout << "Service " << *attribNamesIter << " : " << attribValue << std::endl;
                }

                else {
                    std::vector<std::string> attribValues = servicesIter->get_vector_attribute(*attribNamesIter);
                    std::vector<std::string>::const_iterator attribValuesIter;
                    std::vector<std::string>::const_iterator attribValuesEnd = attribValues.end();
                    for (attribValuesIter = attribValues.begin(); attribValuesIter != attribValuesEnd; ++attribValuesIter) {
                        std::cout << "Service " << *attribNamesIter << " : " << *attribValuesIter << std::endl;
                    }
                }
            }
        }

        if (da) {
            saga::sd::service_data data;
            try {
                data = servicesIter->get_data();
            }

            //Not a lot we can do here so just skip it
            catch (saga::exception& e) {
                std::cerr << "EXCEPTION: " << e.get_message() << std::endl;
            }

            catch (std::exception& e) {
                std::cerr << "Caught std::exception: " << e.what() << std::endl;
            }

            catch (...) {
                std::cerr << "Caught something unexpected." << std::endl;
            }

            std::vector<std::string> dataNames = data.list_attributes();

            //Sort our array of data attribute names so they come out in a decent order
            sort(dataNames.begin(), dataNames.end(), compare_nocase);

            std::vector<std::string>::const_iterator dataIter;
            std::vector<std::string>::const_iterator dataEnd = dataNames.end();

            for (dataIter = dataNames.begin(); dataIter != dataEnd; ++dataIter) {
                if (!data.attribute_is_vector(*dataIter)) {
                    std::string dataValue;
                    dataValue = data.get_attribute(*dataIter);
                    std::cout << "Data " << *dataIter << " : " << dataValue << std::endl;
                }

                else {
                    std::vector<std::string> dataValues = data.get_vector_attribute(*dataIter);
                    std::vector<std::string>::const_iterator dataValuesIter;
                    std::vector<std::string>::const_iterator dataValuesEnd = dataValues.end();
                    for (dataValuesIter = dataValues.begin(); dataValuesIter != dataValuesEnd; ++dataValuesIter) {
                        std::cout << "Data " << *dataIter << " : " << *dataValuesIter << std::endl;
                    }
                }
            }
        }
    }
    if (da || sa) {
        std::cout << std::endl;
    }
    std::cout << services.size() << " services matched the query" << std::endl;
    if (da || sa) {
        std::cout << std::endl;
    }

    return 0;
#endif
}
