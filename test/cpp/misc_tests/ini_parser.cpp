//
//  Copyright (c) 2008 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//
// This file and the homonymous .ini file constitute an interface and
// functionality test for the ini processing facilities available to adaptor
// developers.
//

#include <saga/saga/adaptors/utils/ini/ini.hpp>

#include <boost/test/minimal.hpp>

#include <string>
#include <iostream>

int test_main(int, char *[])
{
    saga::ini::section conf("ini_parser.ini");

    // Identifying sections and nested sections.
    BOOST_CHECK(conf.has_section("section"));

    // FIXME:   has_section_full should be folded into has_section.
    BOOST_CHECK(conf.has_section_full("section.nested"));
    BOOST_CHECK(conf.has_section_full("section.nested.even.more"));

    {
        // Getting and reading from a section

        BOOST_CHECK(conf.has_entry("section.entry"));
        BOOST_CHECK(!conf.has_entry("section.no-entry"));

        saga::ini::section section = conf.get_section("section");
        BOOST_CHECK(section.has_entry("entry"));
        BOOST_CHECK(!section.has_entry("no-entry"));
    }

    // Getting nested entries
    BOOST_CHECK("value" == conf.get_entry("section.entry"));
    BOOST_CHECK("default" == conf.get_entry("section.no-entry", "default"));

    {
        // Accessing a nested section
        saga::ini::section very_nested_section
            = conf.get_section("section.nested.even.more");
        BOOST_CHECK("very-nested-value"
                == very_nested_section.get_entry("very-nested-entry"));
    }

    // Variable expansion
    {
        saga::ini::section section = conf.get_section("variable-expansion");

        BOOST_CHECK(section.get_entry("defined")
                == conf.get_entry("section.entry"));
        BOOST_CHECK(section.has_entry("not-defined")
                == conf.has_entry("section.no-entry"));
        BOOST_CHECK("default value"
                == section.get_entry("default-value"));

        BOOST_CHECK(section.get_entry("nested-entry-1")
                == section.get_entry("defined"));
        BOOST_CHECK(section.get_entry("nested-entry-2")
                == section.get_entry("defined"));
    }

    return 0;
}

