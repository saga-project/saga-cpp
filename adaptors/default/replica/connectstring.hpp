//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(ADAPTORS_DEFAULT_REPLICA_CONNECTSTRING_HPP)
#define ADAPTORS_DEFAULT_REPLICA_CONNECTSTRING_HPP

#include <boost/tokenizer.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace replica { namespace detail
{

///////////////////////////////////////////////////////////////////////////////
class connectstring
{
    typedef std::map<std::string, std::string> params_type;

public:
    connectstring(std::string const& connect)
    {
        typedef boost::char_separator<char> separator_type;
        typedef boost::tokenizer<separator_type> tokenizer_type;
        
        separator_type sep (";");
        tokenizer_type tok (connect, sep);
        tokenizer_type::iterator end = tok.end();
        for (tokenizer_type::iterator it = tok.begin(); it != end; ++it)
        {
            std::string::size_type p = (*it).find_first_of("=");
            std::string key((*it).substr(0, p));
            std::string value;
            if (p != std::string::npos)
                value = (*it).substr(p+1);
            params_.insert(params_type::value_type(key, value));
        }
    }
    
    std::string get() const
    {
        std::string result;

        params_type::const_iterator end = params_.end();
        for (params_type::const_iterator it = params_.begin(); it != end; ++it)
        {
            result += (*it).first + "='" + (*it).second + "' ";
        }

        return result;
    }
    
    void set_value(std::string const& key, std::string const& value)
    {
        params_type::iterator it = params_.find(key);
        if (it != params_.end())
            params_.erase(it);
        params_.insert(params_type::value_type(key, value));
    }
    
    bool has_value(std::string const& key)
    {
        return params_.end() != params_.find(key);
    }
    
private:
    std::map<std::string, std::string> params_;
};

///////////////////////////////////////////////////////////////////////////////
}} // namespace replica::detail

#endif // ADAPTORS_DEFAULT_REPLICA_CONNECTSTRING_HPP


