//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_FILE_COMMON_HELPERS_HPP
#define ADAPTORS_DEFAULT_FILE_COMMON_HELPERS_HPP

#include <climits>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <saga/saga/adaptors/utils/is_local_address.hpp>
#include <saga/saga/path_leaf.hpp>

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

///////////////////////////////////////////////////////////////////////////////
namespace detail
{
    ///////////////////////////////////////////////////////////////////////////
    inline std::ios_base::openmode 
    translate_open_mode (int saga_mode)
    {
      int mode = std::ios::binary;

      if ( saga_mode & saga::filesystem::Read ) 
      {
        mode |= std::ios::in;

        if ( saga_mode & saga::filesystem::Append )
        {
          mode |= std::ios::ate;
        }
      }

      if ( saga_mode & saga::filesystem::Write ) 
      {
        mode |= std::ios::out;

        if ( saga_mode & saga::filesystem::Append )
        {
          mode |= std::ios::ate;
        }
      }

      if ( saga_mode & saga::filesystem::Binary ) 
      {
        mode |= std::ios::binary;
      }

      if (saga_mode & saga::filesystem::Truncate) 
      {
        mode |= std::ios::trunc;
      }

      return (std::ios_base::openmode) mode;
    }

    ///////////////////////////////////////////////////////////////////////////
    // wrapper for gethostname()
    inline std::string get_hostname()
    {
        char buffer[MAX_PATH] = { '\0' };
        gethostname(buffer, sizeof(buffer));
        return std::string(buffer);
    }

    ///////////////////////////////////////////////////////////////////////////
    inline std::string 
    get_filepath(saga::url const& loc)
    {
        return loc.get_path();
    }

    ///////////////////////////////////////////////////////////////////////////
    inline std::string 
    get_scheme_authority(saga::url const& file_url)
    {
        std::string result (file_url.get_scheme());
        if (!result.empty())
        {
            result += ':';
        }
    
        std::string authority (file_url.get_authority());
        if (!authority.empty())
        {
            result += "//";
            result += authority;
            result += '/';
        }
        return result;
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool
    file_islocal(saga::url const& file_url)
    {
      // we consider a file to be local if it's located on the local host
      if ( ! saga::adaptors::utils::is_local_address (file_url) )
            return false;   // remote files are not local

    // we consider a file to be local if it has no scheme at all or if the scheme 
    // if file or any, given that the host is the local host
        std::string scheme(file_url.get_scheme());
        return scheme.empty() || scheme == "file" || scheme == "any";
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool
    file_ftp(saga::url const& file_url)
    {
        std::string scheme(file_url.get_scheme());
        return scheme == "ftp" || scheme == "ftps";
    }

    ///////////////////////////////////////////////////////////////////////////
    inline bool
    file_http(saga::url const& file_url)
    {
        std::string scheme(file_url.get_scheme());
        return scheme == "http" || scheme == "https";
    }

    ///////////////////////////////////////////////////////////////////////////
    inline void canonize(boost::filesystem::path& p)
    {
        namespace fs = boost::filesystem;

        if (fs::is_directory(p)) {
            // make sure directories end with a path delimiter
            {
                std::string const& name = p.string();
                if (name.find_last_of(SAGA_PATH_DELIMITERS) != name.size()-1)
                    p = p.string() + '/';
            }
#if BOOST_FILESYSTEM_VERSION < 3
            p.canonize();
#endif
            p.normalize();

            // make sure directories do not end with a '.'
#if BOOST_FILESYSTEM_VERSION < 3
            {
                std::string const& name = p.string();
                if (name.find_last_of('.') == name.size()-1)
                    p = p.string().substr(0, name.size()-1);
            }
#endif
        }
        else {
#if BOOST_FILESYSTEM_VERSION < 3
            p.canonize();
#endif
            p.normalize();
        }
    }

    // convert a SAGA pattern into a TR1::regex 
    inline std::string 
    regex_from_character_set(std::string::const_iterator& it, 
        std::string::const_iterator end)
    {
        std::string::const_iterator start = it;
        std::string result(1, *it);  // copy '['
        if (*++it == '!') {
            result.append(1, '^');   // negated character set
        }
        else if (*it == ']') {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                "Invalid pattern (empty character set) at: " + 
                std::string(start, end), saga::NoSuccess);
        }
        else {
            result.append(1, *it);   // append this character
        }
        
        // copy while in character set
        while (++it != end) {
            result.append(1, *it);
            if (*it == ']')
                break;
        }
        
        if (it == end || *it != ']') {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                "Invalid pattern (missing closing ']') at: " + 
                std::string(start, end), saga::NoSuccess);
        }

        return result;
    }
    
    inline std::string 
    regex_from_string_choice(std::string::const_iterator& it, 
        std::string::const_iterator end)
    {
        std::string::const_iterator start = it;
        std::string result(1, '(');

        // copy while in string choice
        bool has_one = false;
        while (++it != end && *it != '}') {
            if (*it == ',') {
                if (!has_one) {
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(
                        "Invalid pattern (empty string alternative) at: " + 
                        std::string(start, end), saga::NoSuccess);
                }
                result.append(1, '|');
                has_one = false;
            }
            else {
                result.append(1, *it);
                has_one = true;
            }
        }
        
        if (it == end || *it != '}') {
            SAGA_ADAPTOR_THROW_NO_CONTEXT(
                "Invalid pattern (missing closing '}') at: " + 
                std::string(start, end), saga::NoSuccess);
        }
        
        result.append(1, ')');
        return result;
    }
    
    inline std::string 
    regex_from_pattern(std::string const& pattern)
    {
        std::string result;
        std::string::const_iterator end = pattern.end();
        for (std::string::const_iterator it = pattern.begin(); it != end; ++it)
        {
            char c = *it;
            switch (c) {
            case '*':
                result.append(".*");
                break;
                
            case '?':
                result.append(1, '.');
                break;
                
            case '[':
                result.append(regex_from_character_set(it, end));
                break;
                
            case '{':
                result.append(regex_from_string_choice(it, end));
                break;
                
            case '\\':
                if (++it == end) {
                    SAGA_ADAPTOR_THROW_NO_CONTEXT(
                        "Invalid escape sequence at: " + pattern,
                        saga::NoSuccess);
                }
                result.append(1, *it);
                break;
                
            default:
                result.append(1, c);
                break;
            }
        }
        return result;
    }

    // copy a directory, even recursively, if appropriate
    inline void
    copy_directory(boost::filesystem::path src, boost::filesystem::path dst,
        int open_mode)
    {
        namespace fs = boost::filesystem;

        detail::canonize(dst);

        // ensure the whole destination paths exists, if appropriate
        if (open_mode & saga::name_space::CreateParents) {
            saga::detail::remove_trailing_dot(dst);
            if (!fs::exists(dst) && !fs::create_directories(dst)) {
                SAGA_THROW_NO_OBJECT(
                    dst.string() + ": couldn't create the directory hierarchy", 
                    saga::DoesNotExist);
            }
        }
        else if (open_mode & saga::filesystem::Create) 
        {
            saga::detail::remove_trailing_dot(dst);
            if (!fs::exists(dst) && !fs::create_directory (dst)) {
                SAGA_THROW_NO_OBJECT(
                    dst.string() + ": couldn't create the directory", 
                    saga::DoesNotExist);
            }
        }

        if (!fs::exists(dst)) {
            SAGA_THROW_NO_OBJECT(
                dst.string() + ": target directory does not exist "
                    "(did you forget to specify one of the flags: "
                    "CreateParents or Create?)", 
                saga::DoesNotExist);
        }

        // copy all entries
        fs::path curr_dst (dst.branch_path());
        if (fs::is_directory(dst))
            curr_dst = dst;

        fs::directory_iterator dir_end = fs::directory_iterator();
        for (fs::directory_iterator dir_it(src); dir_it != dir_end; ++dir_it)
        {
            // construct the destination path
            fs::path curr(curr_dst);
            curr /= saga::detail::leaf(*dir_it);

            // if the destination is a directory and we need to work recursively
            if (fs::is_directory(*dir_it)) {
                if (open_mode & saga::name_space::Recursive) {
                    // once we are here, Create/CreateParents is implied
                    detail::copy_directory(*dir_it, curr, 
                        open_mode|saga::name_space::CreateParents|saga::name_space::Create);
                }
                // no error here, we just skip subdirectories
            }
            else {
                // copy the file
                if ((open_mode & saga::name_space::Overwrite) && fs::exists(curr))
                    fs::remove(curr);

                fs::copy_file(*dir_it, curr);
            }
        }
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace detail

#endif // !ADAPTORS_DEFAULT_FILE_COMMON_HELPERS_HPP
