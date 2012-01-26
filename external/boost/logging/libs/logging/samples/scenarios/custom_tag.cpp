/**
 Boost Logging library

 Author: John Torjo, www.torjo.com

 Copyright (C) 2007 John Torjo (see www.torjo.com for email)

 Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)

 See http://www.boost.org for updates, documentation, and revision history.
 See http://www.torjo.com/log2/ for more details
*/


#include <boost/logging/format_fwd.hpp>

struct my_tag {
    my_tag(const char * data = "" /* default value */) : data(data) {}
   const char* data;
};

namespace bl = boost::logging;
typedef bl::tag::holder< bl::default_, my_tag> tag_holder;
BOOST_LOG_FORMAT_MSG( tag_holder )


#include <boost/logging/format.hpp>
#include <boost/logging/format/formatter/tags.hpp>

typedef bl::logger_format_write< > logger_type;
typedef bl::filter::no_ts filter_type;


struct my_tag_formatter : bl::formatter::class_<my_tag_formatter, bl::formatter::implement_op_equal::no_context > {
    void operator()(tag_holder & val) const {
        typedef tag_holder::string_type string_type;
        // automatic conversion - tag holder provides this
        const my_tag & tag = val;
        string_type & str = val;

        // print the tag
        str = tag.data + str;
    }
};


#define L_(data) BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled() ) .set_tag( my_tag(data) )

BOOST_DEFINE_LOG_FILTER(g_log_filter, filter_type ) 
BOOST_DEFINE_LOG(g_l, logger_type) 


void custom_tag_class_example() {
    g_l()->writer().add_formatter( bl::formatter::time("$hh:$mm ") );
    g_l()->writer().add_formatter( my_tag_formatter() );

    g_l()->writer().add_formatter( bl::formatter::append_newline() );     
    g_l()->writer().add_destination( bl::destination::cout() );
    g_l()->mark_as_initialized();

    int i = 1;
    L_("tag_a ") << "this is so cool " << i++;
    L_("tag_b ") << "this is so cool again " << i++;
}




int main() {
    custom_tag_class_example();
}
