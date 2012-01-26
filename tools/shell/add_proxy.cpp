//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// need for for no-echo passwd typing
#ifdef BOOST_WINDOWS
# include <conio.h>
#else
# include <termios.h>
# include <cerrno>
#endif

#include "proxy_data.hpp"

///////////////////////////////////////////////////////////////////////////////
std::string expand_key(char const* value)
{
    if (NULL == value)
        return "";

    std::string str (value);

    // search for something to expand
    std::string::size_type p = str.find_first_of("$");
    while (p != std::string::npos)
    {
        // handle env expansion
        if (str[p+1] == '{')
        {
            std::string::size_type p1 = str.find_first_of("}", p+1);
            if (p != std::string::npos)
            {
                std::string key = str.substr (p+2, p1-p-2);
                char const* val = saga::detail::safe_getenv(key.c_str());
                if (NULL != val)
                    str.replace(p, p1-p+1, val);
                else
                    str.erase(p, p1-p+1);
            }
        }
        p = str.find_first_of ("$", p+1);
    }
    return str;
}

///////////////////////////////////////////////////////////////////////////////
#ifdef BOOST_WINDOWS
std::string read_password()
{
    std::string pwd;

    char ch = 0;
    while ((ch = getch ()  ) != EOF && ch != '\n' && ch != '\r')
    {
        if (ch == '\b') {
            if (!pwd.empty())
                pwd.erase(pwd.size()-1);
        }
        else {
            pwd += ch;
        }
    }
    return pwd;
}
#else
std::string read_password()
{
    struct termios ti;
    std::string pwd;
    int error = tcgetattr(STDIN_FILENO, &ti);
    if (-1 == error) {
        std::cerr << "add_proxy (tcgetattr): " << strerror(errno) << std::endl;
        return pwd;
    }

    // switch off echo
    tcflush(STDOUT_FILENO, TCIOFLUSH);

    ti.c_lflag &= ~ECHO;
//    ti.c_lflag &= ~ECHONL;    // newline needs to be suppressed as well

    error = tcsetattr(STDIN_FILENO, TCSADRAIN, &ti);
    if (-1 == error) {
        std::cerr << "add_proxy (tcsetattr): " << strerror(errno) << std::endl;
        return pwd;
    }
    tcflush(STDIN_FILENO, TCIOFLUSH);

    char ch = 0;
    while ((ch = getchar()) != EOF && ch != '\n' && ch != '\r')
    {
        if (ch == '\b') {
            if (!pwd.empty())
                pwd.erase(pwd.size()-1);
        }
        else {
            pwd += ch;
        }
    }

    tcflush(STDOUT_FILENO, TCIOFLUSH);

    // re-enable echo
    ti.c_lflag |= ECHO;
//    ti.c_lflag |= ECHONL;    // newline needs to be suppressed as well

    error = tcsetattr(STDIN_FILENO, TCSADRAIN, &ti);
    if (-1 == error) {
        std::cerr << "add_proxy (tcsetattr): " << strerror(errno) << std::endl;
        return pwd;
    }
    tcflush(STDIN_FILENO, TCIOFLUSH);

    return pwd;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// create a new credential proxy and add it to the used session object
int shell::c_add_proxy (std::vector<std::string>& args, props& p)
{
    if (args.empty())
    {
        carp ("format: add_proxy <type> [<key:value> ...]");
        return FAILURE;
    }

    // try to find the given proxy type
    proxy_type const* kp = known_proxies;
    for (/**/; NULL != kp->type; ++kp)
    {
        if (args[0] == kp->type)
            break;
    }
    args.erase(args.begin());

    // collect default values (format is key:value, if value is '-', then the
    // default value is used)
    std::map<std::string, std::string> defaults;
    std::vector<std::string>::iterator end = args.end();
    for (std::vector<std::string>::iterator it = args.begin(); it != end; ++it)
    {
        std::string d(*it);
        std::string::size_type p = d.find_first_of(":");
        if (p != std::string::npos)
        {
            defaults.insert(std::make_pair(d.substr(0, p), d.substr(p+1)));
        }
    }

    // complain if type is not found
    if (!kp->type)
    {
        std::string msg("add_proxy: unknown proxy type. Known types are: ");
        for (proxy_type const* p = known_proxies; NULL != p->type; ++p)
        {
            msg += std::string("'") + p->type + "', ";
        }
        carp (msg);
        return FAILURE;
    }

    // create new context object from template, ask for each of the given keys
    saga::context ctx (kp->context_type);

    for (proxy_info const* pi = kp->info; pi->key; ++pi)
    {
        std::string val (expand_key(pi->value));

        // prompt the user for the value, providing the given default
        std::string prompt (pi->key);
        if (!val.empty())
            prompt += std::string(" [") + val +  "]";
        prompt += std::string(": ");

        // if value is given as a default, use it
        std::string input;
        std::map<std::string, std::string>::iterator itd = defaults.find(pi->key);
        if (itd != defaults.end())
        {
            if ((*itd).second == "-")
            {
                input = val;
            }
            else
            {
                input = (*itd).second;
            }

            std::cout << prompt;
            if (pi->doecho)
            {
                std::cout << input << std::endl << std::flush;
            }
            else
            {
                std::cout << std::string(15, '*') << std::endl << std::flush;
            }
        }
        else
        {
            // input value from user if not given
#if SAGA_HAVE_READLINE == 1
          {
                if (pi->doecho)
                {
                    input = ::readline (prompt.c_str());
                    if (!input.empty())
                    {
                        ::add_history (input.c_str());
                    }
                }
                else
                {
                    std::cout << prompt << std::flush;
                    input = read_password();
                    std::cout << std::endl << std::flush;
                }
            }
#else
            std::cout << prompt << std::flush;
            if (pi->doecho)
            {
                std::getline (std::cin, input);
            }
            else
            {
                input = read_password();
                std::cout << std::endl << std::flush;
            }
#endif
            if (input.empty())
                input = val;
        }

        ctx.set_attribute(pi->key, input);
    }

//     ctx.set_defaults();    // has been removed as of newest SAGA spec
    session_.add_context(ctx);

    return SUCCESS;
}
