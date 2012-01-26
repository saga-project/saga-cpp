//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <libgen.h>

#include "saga/saga/packages/isn/entity_data_set.hpp"
#include "saga/saga/packages/isn/entity_data.hpp"
#include "saga/saga.hpp"

/**
 * Takes the program arguments and gives back a pointer to the next one for each call to next()
 */
class Args {

    private:

        std::vector<std::string> m_argv;
        int m_pos;

    public:

        Args(int argc, char* argv[]) :
            m_pos(0) {
            for (int i = 1; i < argc; i++) {
                m_argv.push_back(argv[i]);
            }
        }

        std::string * next() {
            if (m_pos < m_argv.size()) {
                return &m_argv[m_pos++];
            } else {
                return 0;
            }
        }

};

/**
 * Split the string str into the provided vector. The delimiter set defaults to a space.
 */
void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ") {
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0); /* skip leading delimiters */
    std::string::size_type pos = str.find_first_of(delimiters, lastPos); /* then look for next delimiter */

    while (std::string::npos != pos || std::string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos)); /* store token */
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void printTrace(const saga::isn::entity_data_set& eds, const std::string entityType, const std::string filter) {
    std::cout << entityType << " [" << filter << "] has " << eds.get_entity_count() << " -> ";
    std::vector<std::string> res = eds.list_related_entity_names();
    std::cout << res[0];
    for (int i = 1; i < res.size() - 1; i++) {
        std::cout << ", " << res[i];
    }
    if (res.size() > 1) {
        std::cout << " or " << res[res.size() - 1];
    }
    std::cout << std::endl << std::flush;
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
 * Main program
 */
int main(int argc, char *argv[]) {
#if !defined(SAGA_HAVE_PACKAGE_ISN)
  std::cerr << "saga-isn: saga has been compiled without the isn "
               "package, bailing out" << std::endl;
  return -1;
#else
    std::string usage("Usage: " + std::string(basename(argv[0]))
            + " [options] <model> <entity type> ([filter] <entity type>) [attributes]");

    try {
        Args args(argc, argv);

        bool trace = false;
        int maxCount = std::numeric_limits<int>::max();
        std::string serverUrl = "";
        std::string* nextArgP = args.next();
        while (nextArgP && nextArgP->at(0) == '-') {
            if (*nextArgP == "-h" || *nextArgP == "--help") {
                std::cout << std::endl;
                std::cout << usage << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << "   --trace, -t          display count after each hop and related entity sets"
                        << std::endl;
                std::cout << "   --count, -n <value>  the number of entities to display - default unlimited"
                        << std::endl;
                std::cout << "   --url <server url>   this overrides any environment variables" << std::endl;
                std::cout << "   --help, -h           print this message" << std::endl;
                std::cout << "Parameters:" << std::endl;
                std::cout << "    <model>             name of the model - must be known to the adaptor" << std::endl;
                std::cout << "    <entity type>       name of an entity type" << std::endl;
                std::cout << "    <filter>            filter enclosed in square brackets" << std::endl;
                std::cout
                        << "    <attributes>        attributes to display separated by spaces and or commas and enclosed in parantheses - default all"
                        << std::endl;
                std::cout << std::endl;
                return 0;
            } else if (*nextArgP == "-t" || *nextArgP == "--trace") {
                trace = true;
            } else if (*nextArgP == "-n" || *nextArgP == "--count") {
                nextArgP = args.next();
                if (!from_string<int> (maxCount, *nextArgP) || maxCount <= 0) {
                    throw "Count must be a positive integer and not: " + *nextArgP;
                }
            } else if (*nextArgP == "--url") {
                nextArgP = args.next();
                serverUrl = *nextArgP;
            } else {
                throw "Unexpected switch '" + *nextArgP + "'";
            }
            nextArgP = args.next();
        }

        if (!nextArgP) {
            throw usage;
        }
        std::string model = *nextArgP;
        nextArgP = args.next();

        std::string entityType;
        std::string filter;
        saga::session sess;
        std::string attrs;

        if (!nextArgP) {
            throw usage;
        }
        entityType = *nextArgP;
        nextArgP = args.next();
        filter = ""; /* set the default value */
        if (nextArgP && (*nextArgP)[0] == '[') {
            filter = nextArgP->substr(1, nextArgP->size() - 2);
            nextArgP = args.next();
        }
        if (nextArgP && (*nextArgP)[0] == '(') {
            attrs = nextArgP->substr(1, nextArgP->size() - 2);
            nextArgP = args.next();
            if (nextArgP) {
                throw "Attribute list may only be at the end";
            }
        }

        saga::isn::entity_data_set eds(model, entityType, filter, sess, serverUrl);
        if (trace) {
            printTrace(eds, entityType, filter);
        }

        while (nextArgP && !eds.get_data().empty()) {
            entityType = *nextArgP;
            nextArgP = args.next();
            filter = ""; /* set the default value */
            if (nextArgP && (*nextArgP)[0] == '[') {
                filter = nextArgP->substr(1, nextArgP->size() - 2);
                nextArgP = args.next();
            }
            if (nextArgP && (*nextArgP)[0] == '(') {
                attrs = nextArgP->substr(1, nextArgP->size() - 2);
                nextArgP = args.next();
                if (nextArgP) {
                    throw "Attribute list may only be at the end";
                }
            }
            eds = eds.get_related_entities(entityType, filter);

            if (trace) {
                printTrace(eds, entityType, filter);
            }
        }
        std::vector<std::string> attListRequested;
        tokenize(attrs, attListRequested, ", ");
        std::vector<saga::isn::entity_data> data = eds.get_data();
        std::vector<std::string> attList;
        for (int i = 0; i < data.size() && maxCount > 0; i++, --maxCount) {
            if (attListRequested.empty()) {
                attList = data[i].list_attributes();
            } else {
                attList = attListRequested;
            }
            bool spaceNeeded = attListRequested.size() != 1;
            for (int j = 0; j < attList.size(); j++) {
                try {
                    if (!data[i].attribute_is_vector(attList[j])) {
                        std::string attribValue = data[i].get_attribute(attList[j]);
                        if (spaceNeeded) {
                            std::cout << std::endl;
                            spaceNeeded = false;
                        }
                        std::cout << attList[j] << " : " << attribValue << std::endl;
                    } else {
                        std::vector<std::string> attribValues = data[i].get_vector_attribute(attList[j]);
                        if (spaceNeeded) {
                            std::cout << std::endl;
                            spaceNeeded = false;
                        }
                        for (int k = 0; k < attribValues.size(); k++) {
                            std::cout << attList[j] << " : " << attribValues[k] << std::endl;
                        }
                    }
                } catch (saga::does_not_exist& e) {
                    /* ignore it */
                }
            }
        }
    } catch (const std::string& str) {
        std::cerr << str << std::endl;
        return 1;
    } catch (char const * str) {
        std::cerr << str << std::endl;
        return 1;
    } catch (saga::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
#endif
}
