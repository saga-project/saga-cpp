//  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>

#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include <boost/bind.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <saga/saga/adaptors/utils/utils.hpp>

namespace saga
{
  namespace adaptors
  {
    namespace utils
    {
      ///////////////////////////////////////////////////////////////////////////////
      std::vector <std::string> split (std::string line,
                                       char        delim, 
                                       int         number)
      {
        std::vector <std::string> list;

        if (number == 0)    // nothing to do (asking for 0 elements)
          return list;

        std::string split_at (1, delim);
        boost::split (list, line, boost::is_any_of (split_at)); 


        if (number < 0)    // number of elems does not matter
          return list;

        // concatenate the superfluous elements to the last element
        for (std::size_t i = number; i < list.size(); ++i)
        {
          list[number-1] += delim;
          list[number-1] += list[i];
        }

        // adjust the list size
        list.resize (number);
        return list;
      }


      ///////////////////////////////////////////////////////////////////////////////
      // The command line splitting below is taken from the Boost.ProgramOptions
      // library.

      // Copyright Vladimir Prus 2002-2004.
      // Distributed under the Boost Software License, Version 1.0.
      // (See accompanying file LICENSE_1_0.txt
      // or copy at http://www.boost.org/LICENSE_1_0.txt)

      // Take a command line string and splits in into tokens, according
      // to the following rules
      //
      //  - Arguments are delimited by white space, which is either a space or 
      //    a tab.
      //  - A string surrounded by double quotation marks is interpreted as a 
      //    single argument, regardless of white space contained within. A 
      //    quoted string can be embedded in an argument. 
      //  - A double quotation mark preceded by a backslash, \", is interpreted 
      //    as a literal double quotation mark (").
      //  - Backslashes are interpreted literally, unless they immediately precede 
      //    a double quotation mark.
      //
      // These rules are a slight modification of the windows command line 
      // processor rules, as described here:
      //    http://article.gmane.org/gmane.comp.lib.boost.user/3005
      //    http://msdn.microsoft.com/library/en-us/vccelng/htm/progs_12.asp
      //
      std::vector <std::string> split_commandline (std::string const & line)
      {
        using namespace std;
        vector <string> result;

        string::const_iterator i = line.begin ();
        string::const_iterator e = line.end   ();

        for ( /**/ ; i != e; ++i )
          if ( ! isspace ((unsigned char)*i) )
            break;

        if ( i != e ) 
        {
          string current;
          bool   inside_quoted   = false;
          int    backslash_count = 0;

          for (/**/; i != e; ++i) 
          {
            if (*i == '\\') 
            {
              // just count backslashes 
              ++backslash_count;
            }
            else if ( *i == '"' ) 
            {
              // '"' preceded by a backslash is a literal quote
              // the backslash which quoted is removed
              if ( backslash_count > 0 ) 
              {
                current += '"';
                --backslash_count;
              }
              // '"' not preceded by a backslash limits a quote
              else 
              {
                inside_quoted = ! inside_quoted;
              }
            } 
            else 
            {
              // Not quote or backslash. All accumulated backslashes should be
              // added
              if ( backslash_count ) 
              {
                current.append (backslash_count, '\\');
                backslash_count = 0;
              }

              if ( isspace ((unsigned char) *i) && ! inside_quoted ) 
              {
                // Space outside quoted section terminate the current argument
                result.push_back (current);
                current.resize   (0);

                for ( /**/ ; i != e && isspace ((unsigned char) *i); ++i ) 
                  /**/;

                --i;
              } 
              else 
              {                  
                current += *i;
              }
            }
          }

          // If we have trailing backslashes, add them
          if ( backslash_count )
          {
            current.append (backslash_count, '\\');
          }

          // If we have non-empty 'current' or we're still in quoted
          // section (even if 'current' is empty), add the last token.
          if ( ! current.empty () || inside_quoted )
          {
            result.push_back (current);        
          }
          // FIXME: we SHOULD thrown an exception here, non-matching quotes
          // are a BadParameter -- AM
        }

        return result;
      }

      //////////////////////////////////////////////////////////////////
      std::vector <std::string> head (std::size_t n, 
                                      std::vector <std::string> const& in)
      {
        std::vector <std::string> out;
        std::copy(in.begin(), (std::min)(in.end(), in.begin()+n), 
            std::back_inserter(out));
        return out;
      }

      //////////////////////////////////////////////////////////////////
      std::vector <std::string> tail (std::size_t n, 
                                      std::vector <std::string> const& in)
      {
        std::vector <std::string> out;
        std::copy((std::min)(in.begin()+in.size()-n, in.end()), in.end(), 
            std::back_inserter(out));
        return out;
      }

      //////////////////////////////////////////////////////////////////
      // lines should not include newlines
      std::vector <std::string> rev  (std::vector <std::string> const& in)
      {
        std::vector <std::string> out;
        std::vector <std::string>::const_iterator end = in.end();
        for (std::vector <std::string>::const_iterator it = in.begin(); 
            it != end; ++it)
        {
            std::string line;
            std::reverse_copy((*it).begin(), (*it).end(), 
                std::back_inserter(line));
            out.push_back(line);
        }
        return out;
      }

      //////////////////////////////////////////////////////////////////
      std::vector <std::string> tac  (std::vector <std::string> const& in)
      {
        std::vector <std::string> out;
        std::reverse_copy(in.begin(), in.end(), std::back_inserter(out));
        return out;
      }

      //////////////////////////////////////////////////////////////////
      bool find(std::string const& s, std::string const& pattern)
      {
          return s.find(pattern) == std::string::npos;
      }

      std::vector <std::string> grep (std::string pattern, 
                                      std::vector <std::string> const& in)
      {
        std::vector <std::string> out;
        std::remove_copy_if(in.begin(), in.end(), std::back_inserter(out),
            TR1::bind(find, TR1::placeholders::_1, pattern));
        return out;
      }

    } // namespace utils
  } // namespace adaptors 
} // namespace saga


