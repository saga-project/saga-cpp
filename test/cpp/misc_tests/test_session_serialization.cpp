//  Copyright (c) 2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/detail/lightweight_test.hpp>

#include <saga/saga.hpp>
#include <iostream>

int main ()
{
    // serialize session
    try {
        saga::session s;
        saga::context ctx("default_advert_db");
        ctx.set_attribute(saga::attributes::context_userid, "SAGA");
        ctx.set_attribute(saga::attributes::context_userpass, "Example");
        s.add_context(ctx);

        std::vector<saga::context> ctxs = s.list_contexts();
        BOOST_TEST(ctxs.size() == 1 &&
            ctxs[0].get_attribute(saga::attributes::context_type) == "default_advert_db");

        saga::advert::entry e("advert://localhost/session_test?dbname=session_test.db&dbtype=sqlite3",
            saga::advert::Create|saga::advert::Overwrite|saga::advert::ReadWrite);
        e.store_object(s);
    }
    catch (saga::exception const& e)
    {
        std::cerr << e.what () << std::endl;
        BOOST_TEST(false);
    }

    // deserialize session
    try {
        saga::advert::entry e("advert://localhost/session_test?dbname=session_test.db&dbtype=sqlite3",
            saga::advert::ReadWrite);

        saga::session s(e.retrieve_object());
        std::vector<saga::context> ctxs = s.list_contexts();
        BOOST_TEST(ctxs.size() == 1 &&
            ctxs[0].get_attribute(saga::attributes::context_type) == "default_advert_db");
    }
    catch (saga::exception const& e)
    {
        std::cerr << e.what () << std::endl;
        BOOST_TEST(false);
    }

    // serialize context
    try {
        saga::session s;
        saga::context ctx("default_advert_db");
        ctx.set_attribute(saga::attributes::context_userid, "SAGA");
        ctx.set_attribute(saga::attributes::context_userpass, "Example");
        s.add_context(ctx);

        saga::advert::entry e("advert://localhost/context_test?dbname=session_test.db&dbtype=sqlite3",
            saga::advert::Create|saga::advert::Overwrite|saga::advert::ReadWrite);
        e.store_object(ctx);
    }
    catch (saga::exception const& e)
    {
        std::cerr << e.what () << std::endl;
        BOOST_TEST(false);
    }

    // deserialize session
    try {
        saga::advert::entry e("advert://localhost/context_test?dbname=session_test.db&dbtype=sqlite3",
            saga::advert::ReadWrite);

        saga::context ctx(e.retrieve_object());
        BOOST_TEST(ctx.get_attribute(saga::attributes::context_type) == "default_advert_db");
        BOOST_TEST(ctx.get_attribute(saga::attributes::context_userid) == "SAGA");
        BOOST_TEST(ctx.get_attribute(saga::attributes::context_userpass) == "Example");
    }
    catch (saga::exception const& e)
    {
        std::cerr << e.what () << std::endl;
        BOOST_TEST(false);
    }

    return boost::report_errors();
}
