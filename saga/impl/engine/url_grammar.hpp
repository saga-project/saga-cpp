//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_IMPL_ENGINE_URL_GRAMMAR_HPP)
#define SAGA_IMPL_ENGINE_URL_GRAMMAR_HPP

// #define BOOST_SPIRIT_DEBUG

#include <boost/version.hpp>

#if BOOST_VERSION >= 103800

#include <boost/spirit/include/classic_debug.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <boost/spirit/include/classic_loops.hpp>
#include <boost/spirit/include/classic_chset.hpp>
#include <boost/spirit/include/phoenix1_casts.hpp>
#include <boost/spirit/include/phoenix1_primitives.hpp>
#include <boost/spirit/include/phoenix1_binders.hpp>

#include <boost/spirit/home/classic/core/typeof.hpp>

#else

#include <boost/spirit/debug.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/utility/loops.hpp>
#include <boost/spirit/utility/chset.hpp>
#include <boost/spirit/phoenix/casts.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/binders.hpp>

#if BOOST_VERSION >= 103401
#include <boost/spirit/core/typeof.hpp>
#endif

#endif


///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

///////////////////////////////////////////////////////////////////////////////
//
//  This grammar was taken from the RFC1808 (http://www.w3.org/Addressing/rfc1808.txt)
//
//    URL         = ( absoluteURL | relativeURL ) [ "#" fragment ]
//
//    absoluteURL = generic-RL | ( scheme ":" *( uchar | reserved ) )
//
//    generic-RL  = scheme ":" relativeURL
//
//    relativeURL = net_path | abs_path | rel_path
//
//    net_path    = "//" net_loc [ abs_path ]
//    abs_path    = "/"  rel_path
//    rel_path    = [ path ] [ ";" params ] [ "?" query ]
//
//    path        = fsegment *( "/" segment )
//    fsegment    =  *pchar
//    segment     =  *pchar
//
//    params      = param *( ";" param )
//    param       = *( pchar | "/" )
//
//    scheme      =  *( alpha | digit | "+" | "-" | "." )
//    net_loc     =  *( pchar | ";" | "?" )
//    query       =  *( uchar | reserved )
//    fragment    =  *( uchar | reserved )
//
//    pchar       = uchar | ":" | "@" | "&" | "="
//    uchar       = unreserved | escape
//    unreserved  = alpha | digit | safe | extra
//
//    escape      = "%" hex hex
//    hex         = digit | "A" | "B" | "C" | "D" | "E" | "F" |
//                          "a" | "b" | "c" | "d" | "e" | "f"
//
//    alpha       = lowalpha | hialpha
//    lowalpha    = "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" |
//                  "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" |
//                  "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
//    hialpha     = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" |
//                  "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" |
//                  "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
//
//    digit       = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" |
//                  "8" | "9"
//
//    safe        = "$" | "-" | "_" | "." | "+"
//    extra       = "!" | "*" | "'" | "(" | ")" | ","
//    national    = "{" | "}" | "|" | "\" | "^" | "~" | "[" | "]" | "`"
//    reserved    = ";" | "/" | "?" | ":" | "@" | "&" | "="
//    punctuation = "<" | ">" | "#" | "%" | <">
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
boost::spirit::uint_parser<unsigned, 16, 2, 2> const
        hex_escape_p = boost::spirit::uint_parser<unsigned, 16, 2, 2>();

///////////////////////////////////////////////////////////////////////////////
struct net_loc_closure
:   boost::spirit::closure<net_loc_closure, std::string, std::string>
{
    member1 userinfo;
    member2 hostname;
};

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct url_grammar
:   public boost::spirit::grammar<url_grammar<T> >
{
    url_grammar(T& work) : work_(work)
    {
        BOOST_SPIRIT_DEBUG_TRACE_GRAMMAR_NAME(*this, "url_grammar", (true));
    }

    template <typename ScannerT>
    struct definition
    {
        typedef boost::spirit::rule<ScannerT> rule_type;

        rule_type full_url, absolute_url, relative_url, fragment;
        rule_type scheme, uchar, reserved, full_params, full_query;
        rule_type net_path, abs_path, rel_path, net_loc, net_loc_part;
        rule_type path, params, pchar;

        boost::spirit::subrule<0, net_loc_closure::context_t> net_loc_sub;

        definition(url_grammar<T> const &self)
        {
            using namespace boost::spirit;
            using namespace phoenix;

            full_url
                =   (   absolute_url | relative_url )
                >> !(   '#'
                    >>  fragment
                        [
                            bind(&self.work_, &T::set_fragment)
                            (
                                construct_<std::string>(arg1, arg2)
                            )
                        ]
                    )
                ;

            absolute_url
                =   (   scheme >> ':' )
                    [   // recognized scheme
                        bind(&self.work_, &T::set_scheme)
                        (
                            construct_<std::string>(arg1, arg2)
                        )
                    ]
                >>  (   relative_url
                    |  *(uchar | reserved)
                    )
                    [   // recognized scheme specific part
                        bind(&self.work_, &T::set_scheme_specific_part)
                        (
                            construct_<std::string>(arg1, arg2)
                        )
                    ]
                ;

            scheme
                =  repeat_p(2, more)    // schemes are at least 2 characters
                   [
                       alpha_p | digit_p | chset_p("+.-")
                   ]
                ;

            relative_url
                =   net_path | abs_path | rel_path
                ;

            fragment
                =  *(uchar | reserved)
                ;

            net_path
                =  "//" >> net_loc >> !abs_path 
                ;

            abs_path 
                // optional path
                =   (+ch_p('/') >> !path)
                    [
                        bind(&self.work_, &T::set_path)
                        (
                            construct_<std::string>(arg1, arg2)
                        )
                    ]
                >> !full_params       // optional parameters
                >> !full_query        // optional query
                ;

            net_loc
                =   (   net_loc_sub = (
                        // optional username + (optional) password
                           !(   (   *net_loc_part >> !( ':' >> *net_loc_part ) >> '@' )
                                [
                                    net_loc_sub.userinfo = construct_<std::string>(arg1, arg2)
                                ]
                            )
                        // hostname
                            >>  (*net_loc_part)
                                [
                                    net_loc_sub.hostname = construct_<std::string>(arg1, arg2)
                                ]
                        // optional port number
                            >> !(   ':'
                                >>  int_p [ bind(&self.work_, &T::set_port)(arg1) ]
                                )
                        )
                        [   // successfully parsed net_loc, assign the findings
                            bind(&self.work_, &T::set_host_userinfo)(net_loc_sub.hostname, net_loc_sub.userinfo)
                        ]
                    )
                ;

            rel_path
                // optional path
                =  !path
                    [
                        bind(&self.work_, &T::set_path)
                        (
                            construct_<std::string>(arg1, arg2)
                        )
                    ]
                >> !full_params       // optional parameters
                >> !full_query        // optional query
                ;

            full_params
                =   ';'
                    >> !params
                        [
                            bind(&self.work_, &T::set_params)
                            (
                                construct_<std::string>(arg1, arg2)
                            )
                        ]
                ;

            full_query
                =   '?'
                    >> !(*(uchar | reserved))
                        [
                            bind(&self.work_, &T::set_query)
                            (
                                construct_<std::string>(arg1, arg2)
                            )
                        ]
                ;

            path
                =   *pchar >> *('/' >> *pchar)
                ;

            params
                =   +(pchar | '/') >> *(';' >> *(pchar | '/'))
                ;

            net_loc_part
                =   uchar | chset_p("&=;?#")
                ;

            pchar
                =   uchar | chset_p(":@&=\t%") | str_p("\\ ")
                ;

            uchar
                // first 4 alternatives are from 'unreserved' rule
                =   alpha_p
                |   digit_p
                |   chset_p("$_.+<>\"-")        // safe: '"' is non-standard
                |   chset_p("|*\'(),~[]")       // extra: '[', ']' are non-standard
                |   '%' >> hex_escape_p         // escape
                ;

            reserved
                =   chset_p(";/?:@&=|{}\\^`")   // '|', '{', '}', '\\', '^', '`' are non-standard
                ;

            BOOST_SPIRIT_DEBUG_RULE(full_url);
            BOOST_SPIRIT_DEBUG_RULE(absolute_url);
            BOOST_SPIRIT_DEBUG_RULE(relative_url);
            BOOST_SPIRIT_DEBUG_RULE(fragment);
            BOOST_SPIRIT_DEBUG_RULE(scheme);
            BOOST_SPIRIT_DEBUG_RULE(uchar);
            BOOST_SPIRIT_DEBUG_RULE(reserved);
            BOOST_SPIRIT_DEBUG_RULE(net_path);
            BOOST_SPIRIT_DEBUG_RULE(abs_path);
            BOOST_SPIRIT_DEBUG_RULE(rel_path);
            BOOST_SPIRIT_DEBUG_RULE(net_loc);
            BOOST_SPIRIT_DEBUG_RULE(net_loc_sub);
            BOOST_SPIRIT_DEBUG_RULE(net_loc_part);
            BOOST_SPIRIT_DEBUG_RULE(params);
            BOOST_SPIRIT_DEBUG_RULE(pchar);
            BOOST_SPIRIT_DEBUG_RULE(path);
            BOOST_SPIRIT_DEBUG_RULE(full_params);
            BOOST_SPIRIT_DEBUG_RULE(full_query);
        }

    // start rule of this grammar
        rule_type const& start() const
        {
            return full_url;
        }
    };

    T& work_;
};

} }    // namespace saga::impl
///////////////////////////////////////////////////////////////////////////////

#endif // SAGA_IMPL_ENGINE_URL_GRAMMAR_HPP

