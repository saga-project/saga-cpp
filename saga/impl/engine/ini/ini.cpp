//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2008, 2009 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// System Header Files
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <list>
#include <vector>
#include <iostream>
#include <fstream>

#include <saga/impl/engine/ini/ini.hpp>

#include <boost/assert.hpp>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/bind.hpp>

#include <saga/saga/exception.hpp>
#include <saga/impl/exception.hpp>

#ifdef SAGA_APPLE
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#elif !defined(BOOST_WINDOWS)
extern char **environ;
#endif

///////////////////////////////////////////////////////////////////////////////
// example ini line: line # comment
const char pattern_comment[] =  "^([^#]*)(#.*)$";

// example uses ini line: [sec.ssec]
const char pattern_section[] = "^\\[([^\\]]+)\\]$";

// example uses ini line: key = val
const char pattern_entry[] = "^([^\\s=]+)\\s*=\\s*(.*[^\\s])?\\s*$";

///////////////////////////////////////////////////////////////////////////////

namespace
{
  /////////////////////////////////////////////////////////////////////////////
  inline std::string
  trim_whitespace (std::string const &s)
  {
    typedef std::string::size_type size_type;

    size_type first = s.find_first_not_of (" \t\r\n");

    if ( std::string::npos == first )
      return (std::string ());

    size_type last = s.find_last_not_of (" \t\r\n");
    return s.substr (first, last - first + 1);
  }

} // namespace
///////////////////////////////////////////////////////////////////////////////

saga::impl::ini::section::section (std::string filename, section* root)
  : name (filename)
  , root (root ? root : this_())
{
  if (!filename.empty())
    read (filename);
}

void saga::impl::ini::section::read (std::string filename)
{
#if defined(__AIX__) && defined(__GNUC__)
  // NEVER ask why... seems to be some weird stdlib initialization problem
  // If you don't call getline() here the while(getline...) loop below will
  // crash with a bad_cast excetion. Stupid AIX...
  std::string l1;
  std::ifstream i1;
  i1.open(filename.c_str(), std::ios::in);
  std::getline(i1, l1);
  i1.close();
#endif

  std::ifstream input;

  // build ini - open file and parse each line
  input.open (filename.c_str (), std::ios::in);

  if ( ! input.is_open () )
  {
    saga_ini_line_msg ("Cannot open file ", filename);
  }

  // parse file
  std::string                line;
  std::vector <std::string>  lines;

  while ( std::getline (input, line) )
  {
    lines.push_back (line);
  }

  parse (filename, lines);

  input.close ();
}

void saga::impl::ini::section::parse (std::string sourcename,
        std::vector <std::string> lines)
{
  int linenum = 0;

  // NOTE: Can't use shared_ptr here, because we're called from the constructor!
  section * current = this;

  for ( unsigned int i = 0; i < lines.size (); i++ )
  {
    std::string line = lines[i];

    ++linenum;

    // remove trailing new lines and white spaces
    line = trim_whitespace (line);

    // skip if empty line
    if ( line.empty () )
      continue;

    // weep out comments
    TR1::smatch what_comment;
    TR1::regex regex_comment (pattern_comment,
            TR1::regex::ECMAScript | TR1::regex::icase);
    if ( TR1::regex_match (line, what_comment, regex_comment) )
    {
      BOOST_ASSERT (3 == what_comment.size ());

      line = trim_whitespace (what_comment[1]);

      if ( line.empty () )
        continue;
    }

    // no comments anymore: line is either section, key=val,
    // or garbage/empty
    TR1::smatch what;
    TR1::regex regex_section (pattern_section,
            TR1::regex::ECMAScript | TR1::regex::icase);
    TR1::regex regex_entry   (pattern_entry,
            TR1::regex::ECMAScript | TR1::regex::icase);

    if ( TR1::regex_match (line, what, regex_section) )
    {
      // found a section line
      if ( 2 != what.size () )
      {
        saga_ini_line_msg ("Cannot parse sec in ", sourcename, linenum);
      }

      current = get_section (what[1]).get ();
    }

    // did not match section, so might be key/val entry
    else if ( TR1::regex_match (line, what, regex_entry) )
    {
      // found a entry line
      if ( 3 != what.size () )
      {
        saga_ini_line_msg ("Cannot parse key/value in ", sourcename, linenum);
      }

      // add key/val to current section
      current->add_entry (what[1], what[2]);
    }
    else
    {
      // Hmm, is not a section, is not an entry, is not empty - must be an
      // error!
      saga_ini_line_msg ("Cannot parse line at ", sourcename, linenum);
    }
  } // loop over lines
}

bool saga::impl::ini::section::has_section (std::string sec_name) const
{
  if ( sections.find (sec_name) == sections.end () )
  {
    return false;
  }

  return (true);
}

bool saga::impl::ini::section::has_section_full (std::string sec_name) const
{
  std::string::size_type i  = sec_name.find (".");

  if ( i != std::string::npos )
  {
    std::string cor_sec_name = sec_name.substr (0,  i);
    std::string sub_sec_name = sec_name.substr (1 + i);

    section_map::const_iterator it = sections.find (cor_sec_name);

    if ( it != sections.end () )
    {
      return (*it).second->has_section_full (sub_sec_name);
    }

    return false;
  }

  return has_section (sec_name);
}

TR1::shared_ptr<saga::impl::ini::section>
saga::impl::ini::section::get_section (std::string sec_name)
{
    std::string sub_sec_name;

    std::string::size_type i = sec_name.find (".");
    if ( i != std::string::npos )
    {
        sub_sec_name = sec_name.substr (i + 1);
        sec_name.erase(i);
    }

    TR1::shared_ptr<section> & cor_sec = sections [sec_name];
    if ( !cor_sec )
    {
        TR1::shared_ptr<section> newsec (new section());
        newsec->name = sec_name;
        newsec->root = root;

        cor_sec = newsec;
    }

    if (sub_sec_name.empty())
        return cor_sec;
    else
        return cor_sec->get_section (sub_sec_name);
}

void saga::impl::ini::section::add_entry (std::string key,
                                          std::string val)
{
  if ( val.empty () )
    entries.erase (key);
  else
    entries[key] = val;
}

bool saga::impl::ini::section::has_entry (std::string key) const
{
    return !get_entry(key, "").empty();
}

std::string saga::impl::ini::section::get_entry (std::string key) const
{
    std::string result = get_entry(key, "");

    if (result.empty())
        SAGA_THROW("No such key (" + key + ") in section " + get_name (),
            saga::DoesNotExist);

    return result;
}

std::string saga::impl::ini::section::get_entry (std::string key,
                                                 std::string default_val) const
{
  typedef std::vector<std::string> string_vector;

  string_vector split_key;

  boost::split(split_key, key, boost::bind<bool>(boost::is_equal(), '.', _1));
  key = split_key.back();
  split_key.pop_back();

  section const * cur_section = this;
  string_vector::const_iterator end = split_key.end();
  for (string_vector::const_iterator it = split_key.begin(); it!= end; ++it)
  {
    section_map::const_iterator next = cur_section->sections.find (*it);
    if (cur_section->sections.end () == next)
      return default_val;

    cur_section = next->second.get ();
  }

  entry_map::const_iterator entry = cur_section->entries.find (key);
  if (cur_section->entries.end () == entry)
    return default_val;

  std::string result = expand_entry (entry->second);
  if (result.empty ())
    return default_val;

  return result;
}

void saga::impl::ini::section::dump (int ind, std::ostream& strm) const
{
  bool header = false;

  if ( 0 == ind )
  {
    header = true;
  }

  ++ind;

  if ( header )
  {
    strm << "============================\n["
      << get_name () << "]\n";
  }

  entry_map::const_iterator end = entries.end();
  for (entry_map::const_iterator it = entries.begin(); it != end; ++it)
  {
    indent (ind, strm);

    std::string key = (*it).first;
    std::string raw = (*it).second;
    std::string exp = expand_entry (raw);


    strm << key << "   \t: " << raw;

    if ( raw != exp )
    {
      strm  << " \t => " << exp;
    }

    strm << std::endl;
  }

  section_map::const_iterator ends = sections.end();
  for (section_map::const_iterator it = sections.begin(); it != ends; ++it)
  {
    indent (ind, strm);
    strm << "[" << (*it).first << "]\n";
    (*it).second->dump (ind, strm);
  }

  if ( header )
  {
    strm << "============================\n";
  }

}

void saga::impl::ini::section::indent (int ind, std::ostream& strm) const
{
  for ( int i = 0; i < ind; ++i )
  {
    strm << "  ";
  }
}


void saga::impl::ini::section::merge (std::string filename)
{
    TR1::shared_ptr<section> sec (new section(filename, root));
    merge (sec);
}

void saga::impl::ini::section::merge (TR1::shared_ptr<section> other)
{
    // merge entries: keep own entries, and add other entries
    entry_map::const_iterator end = other->entries.end();
    for (entry_map::const_iterator it = other->entries.begin(); it != end; ++it)
    {
        entries[(*it).first] = (*it).second;
    }

    // merge subsections
    section_map::const_iterator ends = other->sections.end();
    for (section_map::const_iterator it = other->sections.begin(); it != ends; ++it)
    {
        TR1::shared_ptr<section> sec = get_section ((*it).first);
        sec->merge ((*it).second);
    }
}

///////////////////////////////////////////////////////////////////////////////

void
saga::impl::ini::section::saga_ini_line_msg (std::string msg, std::string file, int lnum)
{
  if ( lnum > 0 )
  {
    SAGA_THROW_VERBATIM(saga::object(), msg + " " + file + ":"
      + boost::lexical_cast<std::string>(lnum), saga::NoSuccess);
  }

  SAGA_THROW_VERBATIM(saga::object(), msg + " " + file,
      saga::NoSuccess);
}

///////////////////////////////////////////////////////////////////////////////
void saga::impl::ini::section::expand_entry(std::string& value,
        std::string::size_type begin) const
{
    std::string::size_type p = value.find_first_of("$", begin+1);
    while (p != std::string::npos && value.size()-1 != p) {
        if ('[' == value[p+1])
            expand_bracket(value, p);
        else if ('{' == value[p+1])
            expand_brace(value, p);
        p = value.find_first_of("$", p+1);
    }
}

///////////////////////////////////////////////////////////////////////////////
// find the matching closing brace starting from 'begin', escaped braces will 
// be un-escaped
inline std::string::size_type 
find_next(char const* ch, std::string& value, 
    std::string::size_type begin = (std::string::size_type)(-1))
{
    std::string::size_type end = value.find_first_of(ch, begin+1);
    while (end != std::string::npos) {
        if (end != 0 && value[end-1] != '\\')
            break;
        value.replace(end-1, 2, ch);
        end = value.find_first_of(ch, end);
    } 
    return end;
}

///////////////////////////////////////////////////////////////////////////////
void saga::impl::ini::section::expand_bracket(std::string& value,
        std::string::size_type begin) const
{
    // expand all keys embedded inside this key
    expand_entry(value, begin);

    // now expand the key itself
    std::string::size_type end = find_next("]", value, begin+1);
    if (end != std::string::npos) 
    {
        std::string to_expand = value.substr(begin+2, end-begin-2);
        std::string::size_type colon = find_next(":", to_expand);

        std::string deflt;
        if (colon != std::string::npos)
        {
            deflt = to_expand.substr(colon + 1);
            to_expand.erase(colon);
        }

        value.replace(begin, end-begin+1, root->get_entry(to_expand, deflt));
    }
}

void saga::impl::ini::section::expand_brace(std::string& value,
        std::string::size_type begin) const
{
    // expand all keys embedded inside this key
    expand_entry(value, begin);

    // now expand the key itself
    std::string::size_type end = find_next("}", value, begin+1);
    if (end != std::string::npos) 
    {
        std::string to_expand = value.substr(begin+2, end-begin-2);
        std::string::size_type colon = find_next(":", to_expand);
        if (colon == std::string::npos) {
            char* env = saga::detail::safe_getenv(to_expand.c_str());
            value.replace(begin, end-begin+1, 0 != env ? env : "");
        }
        else {
            char* env = saga::detail::safe_getenv(to_expand.substr(0, colon).c_str());
            value.replace(begin, end-begin+1, 
                0 != env ? std::string(env) : to_expand.substr(colon+1));
        }
    }
}

std::string saga::impl::ini::section::expand_entry (std::string value) const
{
    expand_entry(value, (std::string::size_type)(-1));
    return value;
}

