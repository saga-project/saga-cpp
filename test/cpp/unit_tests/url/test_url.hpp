//  Copyright (c) 2005-2006 Chris Miceli (cmicel1@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_TEST_URL_HPP)
#define SAGA_TEST_URL_HPP

#include <boost/test/unit_test.hpp>
#include "test_api.hpp"
#include "../test_functions.hpp"

namespace test_url
{
void instantiate_tests(boost::unit_test_framework::test_suite *test_suite_);

struct test_data {
   char const *uri;        // URI to parse */
   bool isvalid;           // is this URI expected to be valid?
   char const *error_msg;  // if not: expected error message
   char const *scheme;     // if yes: expected schema
   char const *authority;  // if yes: expected authority
   char const *host;       // if yes: expected host
   int port;               // if yes: expected port
   char const *path;       // if yes: expected path
   char const *userinfo;   // if yes: expected userinfo
   char const *query;      // if yes: expected query
   char const *fragment;   // if yes: expected fragment
};
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct test_data const uris_totest[] = 
{
  { "http://example.com", true, NULL, "http", "example.com", "example.com", -1, NULL, NULL, NULL, NULL, },
  { "/bar/foo/", true, NULL, NULL, NULL, NULL, -1, "/bar/foo/", NULL, NULL, NULL, },
  { "bar/foo/", true, NULL, NULL, NULL, NULL, -1, "bar/foo/", NULL, NULL, NULL, },
  { "./bar/foo/", true, NULL, NULL, NULL, NULL, -1, "./bar/foo/", NULL, NULL, NULL, },
  { "file:////bar/foo/", true, NULL, "file", NULL, NULL, -1, "/bar/foo/", NULL, NULL, NULL, },
  { "file:///bar/foo/", true, NULL, "file", NULL, NULL, -1, "/bar/foo/", NULL, NULL, NULL, },
  { "file:///./bar/foo/", true, NULL, "file", NULL, NULL, -1, "./bar/foo/", NULL, NULL, NULL, },
  { "file://host//bar/foo", true, NULL, "file", "host", "host", -1, "/bar/foo", NULL, NULL, NULL, },
  { "file://host/bar/foo", true, NULL, "file", "host", "host", -1, "/bar/foo", NULL, NULL, NULL, },
  { "file://host/./bar/foo", true, NULL, "file", "host", "host", -1, "./bar/foo", NULL, NULL, NULL, },
  { "file:////bar/foo", true, NULL, "file", NULL, NULL, -1, "/bar/foo", NULL, NULL, NULL, },
  { "file:///bar/foo", true, NULL, "file", NULL, NULL, -1, "/bar/foo", NULL, NULL, NULL, },
  { "file:///./bar/foo", true, NULL, "file", NULL, NULL, -1, "./bar/foo", NULL, NULL, NULL, },
  { "?foo=rugue&bar", true, NULL, NULL, NULL, NULL, -1, NULL, NULL, "foo=rugue&bar", NULL, },
  { ";foo=rugue&bar", true, NULL, NULL, NULL, NULL, -1, NULL, NULL, NULL, NULL, },
  { "#fragment", true, NULL, NULL, NULL, NULL, -1, NULL, NULL, NULL, "fragment", },
  { "http:#frag", true, NULL, "http", NULL, NULL, -1, NULL, NULL, NULL, "frag", },
  { "/bar/foo", true, NULL, NULL, NULL, NULL, -1, "/bar/foo", NULL, NULL, NULL, },
  { "./bar/foo", true, NULL, NULL, NULL, NULL, -1, "./bar/foo", NULL, NULL, NULL, },
  { "bar/foo", true, NULL, NULL, NULL, NULL, -1, "bar/foo", NULL, NULL, NULL, },
  { "../bar/../foo", true, NULL, NULL, NULL, NULL, -1, "../foo", NULL, NULL, NULL, },
  { "http:/bar/foo", true, NULL, "http", NULL, NULL, -1, "/bar/foo", NULL, NULL, NULL, },
  { "http:bar/foo", true, NULL, "http", NULL, NULL, -1, "bar/foo", NULL, NULL, NULL, },
  { "http://www.ceic.com/", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/", NULL, NULL, NULL, },
  { "HTTP://www.ceic.com/", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/", NULL, NULL, NULL, },
  { "HTTP://www.CEIC.com/", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/", NULL, NULL, NULL, },
  { "http://www.ceic.com./", true, NULL, "http", "www.ceic.com.", "www.ceic.com.", -1, "/", NULL, NULL, NULL, },
  { "http://www.ceic.com:8400/", true, NULL, "http", "www.ceic.com:8400", "www.ceic.com", 8400, "/", NULL, NULL, NULL, },
  { "http://www.ceic.com:80/", true, NULL, "http", "www.ceic.com:80", "www.ceic.com", 80, "/", NULL, NULL, NULL, },
  { "http://www.ceic.com:8400/cgi.pl?foo", true, NULL, "http", "www.ceic.com:8400", "www.ceic.com", 8400, "/cgi.pl", NULL, "foo", NULL, },
  { "http://www.ceic.com:8400/cgi.pl;bar?foo", true, NULL, "http", "www.ceic.com:8400", "www.ceic.com", 8400, "/cgi.pl", NULL, "foo", NULL, },
  { "http://www.ceic.com:8400/foo/bar/nitz.html#frob", true, NULL, "http", "www.ceic.com:8400", "www.ceic.com", 8400, "/foo/bar/nitz.html", NULL, NULL, "frob", },
  { "http://loic:foobar@www.ceic.com:8400/foo/bar/nitz.html#frob", true, NULL, "http", "loic:foobar@www.ceic.com:8400", "www.ceic.com", 8400, "/foo/bar/nitz.html", "loic:foobar", NULL, "frob", },
  { "http://john:lec%7erre@www.ceic.com/", true, NULL, "http", "john:lec~rre@www.ceic.com", "www.ceic.com", -1, "/", "john:lec~rre", NULL, NULL, },
  { "http://redford@www.ceic.com:8400/", true, NULL, "http", "redford@www.ceic.com:8400", "www.ceic.com", 8400, "/", "redford", NULL, NULL, },
  { "http://redford@www.ceic.com:8400/:foo&'bar@=/dir/file.html", true, NULL, "http", "redford@www.ceic.com:8400", "www.ceic.com", 8400, "/:foo&'bar@=/dir/file.html", "redford", NULL, NULL, },
  { "http://red%2fgros@www.ceic.com:8400/", true, NULL, "http", "red/gros@www.ceic.com:8400", "www.ceic.com", 8400, "/", "red/gros", NULL, NULL, },
  { "http://www.ceic.com:8400/cgi-bin/script?bar/foo#ta/g", true, NULL, "http", "www.ceic.com:8400", "www.ceic.com", 8400, "/cgi-bin/script", NULL, "bar/foo", "ta/g", },
  { "http://www.ceic.com../cgi-bin/script", true, NULL, "http", "www.ceic.com..", "www.ceic.com..", -1, "/cgi-bin/script", NULL, NULL, NULL, },
  { "http://www.ceic.com/dir//file.html", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/dir/file.html",  NULL, NULL, NULL, },
  { "http://www.ceic.com/%2F/dir/", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/dir/", NULL, NULL, NULL, },
  { "http://www.ceic.com/dir/../file", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/file", NULL, NULL, NULL, },
  { "http://www.ceic.com/dir/./file", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/dir/file", NULL, NULL, NULL, },
  { "http://www.ceic.com/./file", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "./file", NULL, NULL, NULL, },
  { "http://www.ceic.com/file/.", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/file/", NULL, NULL, NULL, },
  { "http://www.ceic.com/file/./", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/file/", NULL, NULL, NULL, },
  { "http://www.ceic.com/dir1/dir2/..", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/dir1/", NULL, NULL, NULL, },
  { "http://www.ceic.com/file/../dir/../../foo", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/foo", NULL, NULL, NULL, },
  { "/foo.html%20http://w3.austin.ibm.com/", true, NULL, NULL, NULL, NULL, -1, "/foo.html http:/w3.austin.ibm.com/", NULL, NULL, NULL, },
  { "/foo.html%20http://w3.austin.ibm.com/", true, NULL, NULL, NULL, NULL, -1, "/foo.html http:/w3.austin.ibm.com/", NULL, NULL, NULL, },
  { "http://www.ceic.com/file?a+%2B+b", true, NULL, "http", "www.ceic.com", "www.ceic.com", -1, "/file", NULL, "a+++b", NULL, },
  { "document.html%20http://www.senat.fr/", true, NULL, NULL, NULL, NULL, -1, "document.html http:/www.senat.fr/", NULL, NULL, NULL, },
  { "#frag%20http://www.senat.fr/", true, NULL, NULL, NULL, NULL, -1, NULL, NULL, NULL, "frag http://www.senat.fr/", },
  { "/document.html%20http://www.senat.fr/iway/", true, NULL, NULL, NULL, NULL, -1, "/document.html http:/www.senat.fr/iway/", NULL, NULL, NULL, },
  { "//www.curie.u-psud.fr/umr146/%20http://www.curie.fr", true, NULL, NULL, "www.curie.u-psud.fr", "www.curie.u-psud.fr", -1, "/umr146/ http:/www.curie.fr", NULL, NULL, NULL, },
  { "http://www.foo.bar/bar//foo/file//", true, NULL, "http", "www.foo.bar", "www.foo.bar", -1, "/bar/foo/file/", NULL, NULL, NULL, },
  { "/bar/////bbobo/grunt.html%20http://www.pire.org/", true, NULL, NULL, NULL, NULL, -1, "/bar/bbobo/grunt.html http:/www.pire.org/", NULL, NULL, NULL, },
  { "ftp://www.foo.bar/bar/", true, NULL, "ftp", "www.foo.bar", "www.foo.bar", -1, "/bar/", NULL, NULL, NULL, },
  { "ftp://www.foo.bar/bar?foo/bar", true, NULL, "ftp", "www.foo.bar", "www.foo.bar", -1, "/bar", NULL, "foo/bar", NULL, },
  { "ftp://ftp@www.foo.bar/bar?foo/bar", true, NULL, "ftp", "ftp@www.foo.bar", "www.foo.bar", -1, "/bar", "ftp", "foo/bar", NULL, },
  { "ftp://anonymous@www.foo.bar/bar?foo/bar", true, NULL, "ftp", "anonymous@www.foo.bar", "www.foo.bar", -1, "/bar", "anonymous", "foo/bar", NULL, },
  { "ftp://anonymous:pass@www.foo.bar/bar?foo/bar", true, NULL, "ftp", "anonymous:pass@www.foo.bar", "www.foo.bar", -1, "/bar", "anonymous:pass", "foo/bar", NULL, },
  { "gsiftp://www.foo.bar/bar/", true, NULL, "gsiftp", "www.foo.bar", "www.foo.bar", -1, "/bar/", NULL, NULL, NULL, },
  { "gsiftp://www.foo.bar/bar?foo/bar", true, NULL, "gsiftp", "www.foo.bar", "www.foo.bar", -1, "/bar", NULL, "foo/bar", NULL, },
  { "gsiftp://ftp@www.foo.bar/bar?foo/bar", true, NULL, "gsiftp", "ftp@www.foo.bar", "www.foo.bar", -1, "/bar", "ftp", "foo/bar", NULL, },
  { "gsiftp://anonymous@www.foo.bar:8211/bar?foo/bar", true, NULL, "gsiftp", "anonymous@www.foo.bar:8211", "www.foo.bar", 8211, "/bar", "anonymous", "foo/bar", NULL, },
  { "gsiftp://anonymous:pass@www.foo.bar:8211/bar?foo/bar", true, NULL, "gsiftp", "anonymous:pass@www.foo.bar:8211", "www.foo.bar", 8211, "/bar", "anonymous:pass", "foo/bar", NULL, },
  { "mailto:loic@ceic.com", true, NULL, "mailto", NULL, NULL, -1, "loic@ceic.com", NULL, NULL, NULL, },
  { "file:/dir/a/b/c.html", true, NULL, "file", NULL, NULL, -1, "/dir/a/b/c.html", NULL, NULL, NULL, },
  { "file://hostname/dir/a/b/c.html", true, NULL, "file", "hostname", "hostname", -1, "/dir/a/b/c.html", NULL, NULL, NULL, },
  { "file://locAlhost/dir/a/b/c.html", true, NULL, "file", "localhost", "localhost", -1, "/dir/a/b/c.html", NULL, NULL, NULL, },
  { "file:///dir/a/b/c.html", true, NULL, "file", NULL, NULL, -1, "/dir/a/b/c.html", NULL, NULL, NULL, },
  { "file://dir/a/b/c.html#frag", true, NULL, "file", "dir", "dir", -1, "/a/b/c.html", NULL, NULL, "frag", },
  { "file:/dir/a/b?/c.html", true, NULL, "file", NULL, NULL, -1, "/dir/a/b", NULL, "/c.html", NULL, },
  { "gopher://host:10/Tfoo%09bar%09foobar", true, NULL, "gopher", "host:10", "host", 10, "/Tfoo\tbar\tfoobar", NULL, NULL, NULL, },
  { "gopher://host", true, NULL, "gopher", "host", "host", -1, NULL, NULL, NULL, NULL, },
  { "gopher://host/Tfoo%09bar", true, NULL, "gopher", "host", "host", -1, "/Tfoo\tbar", NULL, NULL, NULL, },
  { "gopher://host/Tfoo", true, NULL, "gopher", "host", "host", -1, "/Tfoo", NULL, NULL, NULL, },
  { "gopher://host/T", true, NULL, "gopher", "host", "host", -1, "/T", NULL, NULL, NULL, },
  { "news:alt.adoptive.parenting", true, NULL, "news", NULL, NULL, -1, "alt.adoptive.parenting", NULL, NULL, NULL, },
  { "news:EFGJG4.7A@deshaw.com", true, NULL, "news", NULL, NULL, -1, "EFGJG4.7A@deshaw.com", NULL, NULL, NULL, },
  { "news://binky.capnet.state.tx.us/5rb1or$67v@news.jumpnet.com", true, NULL, "news", "binky.capnet.state.tx.us", "binky.capnet.state.tx.us", -1, "/5rb1or$67v@news.jumpnet.com", NULL, NULL, NULL, },
  { "news://adenine.c.dna.affrc.go.jp/misc.transport.air-industry.cargo", true, NULL, "news", "adenine.c.dna.affrc.go.jp", "adenine.c.dna.affrc.go.jp", -1, "/misc.transport.air-industry.cargo", NULL, NULL, NULL, },
  { "snews://binky.capnet.state.tx.us/5rb1or$67v@news.jumpnet.com", true, NULL, "snews", "binky.capnet.state.tx.us", "binky.capnet.state.tx.us", -1, "/5rb1or$67v@news.jumpnet.com", NULL, NULL, NULL, },
  { "nntp://news.ceic.com/alt.binaries/234", true, NULL, "nntp", "news.ceic.com", "news.ceic.com", -1, "/alt.binaries/234", NULL, NULL, NULL, },
  { "nntp://news.ceic.com:8080/alt.binaries/234", true, NULL, "nntp", "news.ceic.com:8080", "news.ceic.com", 8080, "/alt.binaries/234", NULL, NULL, NULL, },
  { "nntp://news.ceic.com:119/alt.binaries/234", true, NULL, "nntp", "news.ceic.com:119", "news.ceic.com", 119, "/alt.binaries/234", NULL, NULL, NULL, },
  { "wais://wais.ceic.com/base", true, NULL, "wais", "wais.ceic.com", "wais.ceic.com", -1, "/base", NULL, NULL, NULL, },
  { "wais://wais.ceic.com:210/base?query", true, NULL, "wais", "wais.ceic.com:210", "wais.ceic.com", 210, "/base", NULL, "query", NULL, },
  { "wais://wais.ceic.com/base/wtype/wpath", true, NULL, "wais", "wais.ceic.com", "wais.ceic.com", -1, "/base/wtype/wpath", NULL, NULL, NULL, },
  { "wais://wais.ceic.com:210/base/wtype/wpath", true, NULL, "wais", "wais.ceic.com:210", "wais.ceic.com", 210, "/base/wtype/wpath", NULL, NULL, NULL, },
  { "wais://wais.ceic.com//base/wtype/wpath", true, NULL, "wais", "wais.ceic.com", "wais.ceic.com", -1, "/base/wtype/wpath", NULL, NULL, NULL, },
  { "wais://wais.ceic.com:8080//base/wtype/wpath", true, NULL, "wais", "wais.ceic.com:8080", "wais.ceic.com", 8080, "/base/wtype/wpath", NULL, NULL, NULL, },
  { "wais://usr:pass@wais.ceic.com:210//base/wtype/wpath", true, NULL, "wais", "usr:pass@wais.ceic.com:210", "wais.ceic.com", 210, "/base/wtype/wpath", "usr:pass", NULL, NULL, },
  { NULL, false, NULL, NULL, NULL, NULL, 0, NULL, NULL, NULL, NULL, },
};
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct test_data const invalid_uris_totest[] = {
  { "http://:8080/", false, "saga::url: uri_cannonicalize: http://:8080/ has null netloc", NULL, NULL, NULL, 8080, NULL, NULL, NULL, NULL, },
  { "http://loic:foo34@/#frag", false, "saga::url: cannonicalize: in http://#frag, passwd cannot be set in relative uri", NULL, NULL, NULL, -1, NULL, NULL, NULL, NULL, },
  { "http://redgros@www.c%2feic.com:8400/", false, NULL, "http", "redgros@www.c%2feic.com:8400", "www.c%2feic.com", 8400, NULL, NULL, NULL, NULL, }, 
  { NULL, false, NULL, NULL, NULL, NULL, 0, NULL, NULL, NULL, NULL, },
};
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
struct valid_urls
{
  static std::string name;

  static int test()
  {
    for ( int index = 0; NULL != uris_totest[index].uri; ++index ) 
    {
      std::string  in_uri         = uris_totest[index].uri;                                       // URI to parse */
      int          in_port        = uris_totest[index].port;                                      // expected port

      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
        std::cout << " === " << in_uri << "\n";
      }

      std::string  in_error_msg   = (uris_totest[index].error_msg != NULL ) ? uris_totest[index].error_msg : "";  // expected error message
      std::string  in_scheme      = (uris_totest[index].scheme    != NULL ) ? uris_totest[index].scheme    : "";  // expected schema
      std::string  in_authority   = (uris_totest[index].authority != NULL ) ? uris_totest[index].authority : "";  // expected authority
      std::string  in_host        = (uris_totest[index].host      != NULL ) ? uris_totest[index].host      : "";  // expected host
      std::string  in_path        = (uris_totest[index].path      != NULL ) ? uris_totest[index].path      : "";  // expected path
      std::string  in_userinfo    = (uris_totest[index].userinfo  != NULL ) ? uris_totest[index].userinfo  : "";  // expected userinfo
      std::string  in_query       = (uris_totest[index].query     != NULL ) ? uris_totest[index].query     : "";  // expected query
      std::string  in_fragment    = (uris_totest[index].fragment  != NULL ) ? uris_totest[index].fragment  : "";  // expected fragment

      saga::url u (in_uri);

      std::string  out_uri        = u.get_string ();               // parsed URI */
      std::string  out_scheme     = u.get_scheme ();               // found schema
      std::string  out_authority  = u.get_authority ();            // found authority
      std::string  out_host       = u.get_host ();                 // found host
      int          out_port       = u.get_port ();                 // found port
      std::string  out_path       = u.get_path ();                 // found path
      std::string  out_userinfo   = u.get_userinfo ();             // found userinfo
      std::string  out_query      = u.get_query ();                // found query
      std::string  out_fragment   = u.get_fragment ();             // found fragment

//       SAGA_CHECK_EQUAL(in_uri, out_uri);   // FIXME: add check for full url

      SAGA_CHECK_EQUAL(in_scheme, out_scheme);
      SAGA_CHECK_EQUAL(in_authority, out_authority);
      SAGA_CHECK_EQUAL(in_host, out_host);
      SAGA_CHECK_EQUAL(in_port, out_port);
      SAGA_CHECK_EQUAL(in_path, out_path);
      SAGA_CHECK_EQUAL(in_userinfo, out_userinfo);
      SAGA_CHECK_EQUAL(in_query, out_query);
      SAGA_CHECK_EQUAL(in_fragment, out_fragment);
    }
    
    return 0;
  }

}; //valid_urls
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
struct invalid_urls
{
  static std::string name;

  static int test()
  {
    saga::url u;
    for (int index = 0; NULL != invalid_uris_totest[index].uri; ++index)
    {
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
        std::cout << " === " << invalid_uris_totest[index].uri << "\n";
      }
#if BOOST_VERSION <= 103301
      SAGA_CHECK_THROW((u = saga::url(invalid_uris_totest[index].uri)), std::exception);
#else
      SAGA_CHECK_THROW((u = saga::url(invalid_uris_totest[index].uri)), saga::exception);
#endif
    }
    return 0;
  }
}; //invalid_urls
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
namespace all
{
    class suite : public boost::unit_test_framework::test_suite
    {
    public:
        struct add_tests
        {
            add_tests(suite& outer) 
                : outer_(outer) {}

            template<typename Test>
            void operator()(Test)
            {
                outer_.add(
                    SAGA_TEST_CASE(
                        boost::bind(&test_url::api::run_test<Test>::execute),
                        Test::name.c_str()
                    ), 0, 40); // timeout in sec.
            }
            suite& outer_;
        };
        
        suite () : test_suite ("saga::url test suite")
        {
          typedef boost::mpl::vector <valid_urls,
                                      invalid_urls> apitests;
            boost::mpl::for_each<apitests>(add_tests(*this));
        }
   }; //end suite for unit tests

}}//End namespace test_url::all

#endif // !SAGA_TEST_URL_HPP
