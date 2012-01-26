//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

// #define USE_SPIRIT

#ifdef USE_SPIRIT
#include <boost/version.hpp>

#if BOOST_VERSION >= 103800
#include <boost/spirit/include/classic_symbols.hpp>
#else
#include <boost/spirit/symbols.hpp>
#endif

#else // USE_SPIRIT

# include <saga/impl/exception.hpp>
# include <saga/saga/adaptors/utils.hpp>

#endif // USE_SPIRIT

#include <saga/impl/config.hpp>
#include <saga/impl/engine/url_grammar.hpp>
#include <saga/impl/url.hpp>

#include <saga/saga/url.hpp>
#include <saga/impl/runtime.hpp>
#include <saga/saga/adaptors/file_transfer_spec.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors
{
    // parse the given string for a file transfer specification and return true
    // if successful
    bool 
    parse_file_transfer_specification(std::string spec, 
        std::string& left_url, file_transfer_operator& mode, 
        std::string& right_url)
    {
#ifdef USE_SPIRIT
        using namespace boost::spirit;
        using namespace phoenix;

        saga::url url_left, url_right;
        impl::url* impl_left(impl::runtime::get_impl(url_left));
        impl::url* impl_right(impl::runtime::get_impl(url_right));

        symbols<file_transfer_operator> ops;
        ops.add (">" , copy_local_remote)
                (">>", append_local_remote)
                ("<" , copy_remote_local)
                ("<<", append_remote_local)
            ;

        file_transfer_operator parsed_mode = unknown_mode;
        parse_info<> pi = parse(
                spec.c_str(),
                lexeme_d[impl::url_grammar<impl::url>(*impl_left)] 
                >>  ops [var(parsed_mode) = arg1]
                >>  lexeme_d[impl::url_grammar<impl::url>(*impl_right)]
                >> !end_p,
                space_p
            );

        if (!pi.full)
            return false;

        impl_left->set_checked();
        impl_right->set_checked();

        left_url = url_left.get_url();
        right_url = url_right.get_url();
        mode = parsed_mode;
        return true;

#else // USE_SPIRIT

        // for split to work correctly, we first collect superflous spaces
        size_t pos = spec.find ("  ");
        while ( std::string::npos != pos )
        {
          spec.erase (pos, 1);
          pos = spec.find ("  ");
        }

        std::vector <std::string> items = saga::adaptors::utils::split (spec, ' ');

        if ( 3 != items.size () )
        {
          SAGA_THROW_NO_OBJECT ("Invalid file transfer specification", 
                                saga::BadParameter);
        }

        saga::url   url_left  = items[0];
        std::string string_op = items[1];
        saga::url   url_right = items[2];

        if      ( string_op == ">"  ) mode =   copy_local_remote; 
        else if ( string_op == ">>" ) mode = append_local_remote; 
        else if ( string_op == "<"  ) mode =   copy_remote_local; 
        else if ( string_op == "<<" ) mode = append_remote_local; 
        else SAGA_THROW_NO_OBJECT ("Invalid mode in file transfer specification", 
                                   saga::BadParameter);

        left_url  = url_left.get_url();
        right_url = url_right.get_url();

        return true;
#endif // USE_SPIRIT
    }

    ///////////////////////////////////////////////////////////////////////////////
}}
