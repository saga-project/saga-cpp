//  Copyright (c) 2005-2008 Andre Merzky
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Boost include for boost::thread::sleep
#include <boost/thread.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "saga-shell.hpp"

// system includes
#include <climits>
#include <unistd.h>
#include <string.h>
#include <cctype>

#include <string>
#include <iostream>

#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

#define HISTORY_LEN 100

///////////////////////////////////////////////////////////
//
// we get some environment variables at startup, for the
// initial working directory and initial job manager.
//
std::string shell::init_contact (void)
{
    std::map <std::string, std::string>::iterator it = env_.find ("CONTACT");

    if ( it != env_.end () )
    {
      return (*it).second;
    }

    char const* env = saga::detail::safe_getenv ("SAGA_SHELL_CONTACT");

    std::string ret;

    if ( env )
        ret = env;
    else
        ret = "fork://localhost/";

    env_["CONTACT"] = ret;
    return ret;
}

std::string shell::init_host (const saga::url & url)
{
    std::string host = url.get_host ();

    if ( host.empty () )
    {
      return ("localhost");
    }

    return host;
}

std::string shell::init_home (void)
{
  std::map <std::string, std::string>::iterator it = env_.find ("HOME");

  if ( it != env_.end () )
  {
    return (*it).second;
  }

  char const * env = saga::detail::safe_getenv ("SAGA_SHELL_HOME");
  if (env)
  {
    env_["HOME"] = env;
    return env;
  }

  char cwd[MAX_PATH + 1] = { '\0' };

  // if string is non-empty
  if (NULL != ::getcwd (cwd, MAX_PATH) && 
      '\0' != cwd[0]                   )
  {
    std::string ret ("file://localhost/");
    ret += cwd;
    env_["HOME"] = ret;
    return ret;
  }

  std::string ret ("file://localhost/tmp");
  env_["HOME"] = ret;
  return ret;
}

void shell::init_env (char* envp[])
{
  for ( int n = 0; envp[n]; n++ )
  {
    std::string e   = envp[n];
    size_t      pos = e.find_first_of ("=");

    if ( pos != std::string::npos )
    {
      env_[e.substr (0, pos)] = e.substr (pos + 1);
    }
    else
    {
      env_[e.substr (0, pos)] = "";
    }
  }

}

void shell::init_history (void)
{
#if SAGA_HAVE_READLINE == 1

  char const * env = saga::detail::safe_getenv ("HOME");

  if ( env )
  {
    history_  = env;
    history_ += "/.saga-shell.hist";

    using_history  ();
    read_history   (history_.c_str ());
  }

#endif

  return;
}

void shell::close_history (void)
{
#if SAGA_HAVE_READLINE == 1
  if ( history_length && history_ != "" )
  {
    write_history         (history_.c_str ());
    history_truncate_file (history_.c_str (), HISTORY_LEN);
  }
#endif
}

///////////////////////////////////////////////////////////
//
// the c'tor only initializes a number of variables
//
shell::shell (void)
  : session_ (saga::get_default_session()),
    env_     (),
    home_    (init_home ()),
    cwd_     (session_, home_.get_url(), saga::name_space::ReadWrite),
    contact_ (init_contact ()),
    host_    (init_host (contact_)),
    js_      (session_, contact_)
{
}


///////////////////////////////////////////////////////////
//
// d'tor ...
//
shell::~shell (void)
{
  close_history ();
}


///////////////////////////////////////////////////////////
//
// now and then, we have to re-initialize member variables,
// as some operations change the environment
//
void shell::init (void)
{
  home_    = init_home    ();
  contact_ = init_contact ();
  host_    = init_host    (contact_);

  // carp ("reinit js with " + contact_, false);

  // recreate job service
  saga::job::service js (session_, contact_);
  js_ = js;

  // recreate cwd
  saga::name_space::directory cwd (session_, cwd_.get_url ());
  cwd_ = cwd;
}


///////////////////////////////////////////////////////////
//
// as long as there is input, get it, and run the
// respective command
//

void shell::run (int argc, char* argv[], char* envp[])
{

  init_env     (envp);
  init_history ();
  set_prompt   ();

  // if there are arguments given on the command line, source these first
  if ( 1 != argc )
  {
    std::vector <std::string> args;
    for ( int i = 1; i < argc; ++i )
    {
      args.push_back (argv[i]);
    }

    props p;
    c_source (args, p);
  }

  // keep running until we see 'exit'
  while ( true )
  {
    // get user input
    std::string input = get_input ();

    boost::algorithm::trim (input);

    // if input is complete
    while ( input.length () > 0 &&
            input.at (input.length () - 1 ) == '\\' )
    {
      input.erase (input.length () - 1); // remove '\'
      input += get_input ("> ");
    }

    // run it
    props p;
    int ret = execute (input, p);
    env_["status"] = to_string (ret);

    if ( p.err != "" )
    {
      std::cerr << p.err;
    }

    if ( p.out != "" )
    {
      std::cout << p.out;
    }

    // termination on exit/quit
    if ( input == "exit" ||
         input == "quit" )
    {
      std::cout << std::endl;
      return;
    }
  }

  return;
}


///////////////////////////////////////////////////////////
//
// run a shell command
//
int shell::execute (std::string input, props & p)
{
  return execute_ands (input, p);
}

int shell::execute_ands (std::string input, props & p)
{
  // split into &&'ed elements
  std::vector <std::string> ands = split_input_str (input, p, "&&");

  // if we don't use ands, just execute
  if ( ands.size () == 1 )
  {
    return execute_ors (ands[0], p);
  }
  else
  {
    // so we have ands!  run one after the other, and stop on first failure
    std::vector <std::string> :: iterator begin = ands.begin ();
    std::vector <std::string> :: iterator end   = ands.end ();
    std::vector <std::string> :: iterator iter  = begin;

    for ( iter = begin; iter != end; ++iter )
    {
      if ( FAILURE == execute (*iter, p) )
      {
        return FAILURE;
      }
    }
  }

  return SUCCESS;
}

int shell::execute_ors (std::string input, props & p)
{
  // split into ||'ed elements
  std::vector <std::string> ors = split_input_str (input, p, "||");

  // if we don't use ors, just execute
  if ( ors.size () == 1 )
  {
    return execute_pipes (ors[0], p);
  }
  else
  {
    // so we have ors!  run one after the other, until one succeeds
    std::vector <std::string> :: iterator begin = ors.begin ();
    std::vector <std::string> :: iterator end   = ors.end ();
    std::vector <std::string> :: iterator iter  = begin;

    for ( iter = begin; iter != end; ++iter )
    {
      if ( SUCCESS == execute (*iter, p) )
      {
        return SUCCESS;
      }
    }
  }

  return FAILURE;
}

int shell::execute_pipes (std::string input, props & p)
{
  // split into |'ed elements
  std::vector <std::string> pipes = split_input_str (input, p, "|");


  // if we don't use pipes, just execute
  if ( pipes.size () == 1 )
  {
    return execute_command (pipes[0], p);
  }
  else
  {
    p.piped = true;
    // so we have pies!  run one after the other, until one succeeds, but use
    // buffers to forward output
    std::vector <std::string> :: iterator begin = pipes.begin ();
    std::vector <std::string> :: iterator end   = pipes.end ();
    std::vector <std::string> :: iterator iter  = begin;

    for ( iter = begin; iter != end; ++iter )
    {
      if ( FAILURE == execute (*iter, p) )
      {
        return FAILURE;
      }

      if ( iter + 1 == end )
      {
        // last pipe element?

        p.piped = false;
      }
    }
  }

  return SUCCESS;
}

int shell::execute_command (std::string input, props & p)
{
  return execute_command (split_input (input, p), p);
}

int shell::execute_command (std::vector <std::string> args, props & p)
{
  if ( 0 == args.size () )
  {
    return SUCCESS;
  }

  std::string command = args[0];
  boost::algorithm::trim(command);
  args.erase (args.begin ());

  try
  {
    // operations on the name space
    if      ( command == "ls"            ) { c_ls            (args, p); }
    else if ( command == "pwd"           ) { c_pwd           (args, p); }
    else if ( command == "cd"            ) { c_cd            (args, p); }
    else if ( command == "touch"         ) { c_touch         (args, p); }
    else if ( command == "cp"            ) { c_cp            (args, p); }
    else if ( command == "mv"            ) { c_mv            (args, p); }
    else if ( command == "ln"            ) { c_ln            (args, p); }
    else if ( command == "rm"            ) { c_rm            (args, p); }
    else if ( command == "mkdir"         ) { c_mkdir         (args, p); }
    else if ( command == "rmdir"         ) { c_rmdir         (args, p); }

    // operations on file contents
    else if ( command == "cat"           ) { c_cat           (args, p); }
    else if ( command == "wc"            ) { c_wc            (args, p); }

    // operations on replica locations
    else if ( command == "rep_find"      ) { c_rep_find      (args, p); }
    else if ( command == "rep_list"      ) { c_rep_list      (args, p); }
    else if ( command == "rep_add"       ) { c_rep_add       (args, p); }
    else if ( command == "rep_remove"    ) { c_rep_remove    (args, p); }
    else if ( command == "rep_update"    ) { c_rep_update    (args, p); }
    else if ( command == "rep_replicate" ) { c_rep_replicate (args, p); }

    // job related operations
    else if ( command == "run"           ) { c_run           (args, p); }
    else if ( command == "run2"          ) { c_run2          (args, p); }
    else if ( command == "ps"            ) { c_ps            (args, p); }
    else if ( command == "status"        ) { c_status        (args, p); }
    else if ( command == "suspend"       ) { c_suspend       (args, p); }
    else if ( command == "resume"        ) { c_resume        (args, p); }
    else if ( command == "kill"          ) { c_kill          (args, p); }

    // core shell things
    else if ( command == "env"           ) { c_env           (args, p); }
    else if ( command == "setenv"        ) { c_setenv        (args, p); }
    else if ( command == "getenv"        ) { c_getenv        (args, p); }
    else if ( command == "unsetenv"      ) { c_unsetenv      (args, p); }

    else if ( command == "add_proxy"     ) { c_add_proxy     (args, p); }
    else if ( command == "remove_proxy"  ) { c_remove_proxy  (args, p); }

    else if ( command == "source"        ) { c_source        (args, p); }
    else if ( command == "."             ) { c_source        (args, p); }

    else if ( command == "init"          ) { c_init          (args, p); }
    else if ( command == "history"       ) { c_history       (args, p); }
    else if ( command == "echo"          ) { c_echo          (args, p); }
    else if ( command == "help"          ) { c_help          (args, p); }
    else if ( command == "man"           ) { c_help          (args, p); }

    // try again, really now
    else if ( command == "execute"       ) { execute_command (args, p); }

    // fallbacks
    else if ( command == "//"            ) { /* ignore comments   */    }
    else if ( command == "#"             ) { /* ignore comments   */    }
    else if ( command == ""              ) { /* nothing to do     */    }
    else if ( command == "exit"          ) { /* handled elsewhere */    }
    else if ( command == "quit"          ) { /* handled elsewhere */    }
    else                                   { c_help          (args, p); }
  }


  // catch SAGA exceptions, but leave system exceptions untouched
  catch ( saga::exception const & e )
  {
    switch ( e.get_error () )
    {
      // handle NotImplemented gracefully
      case saga::NotImplemented:
        carp ("This method is not yet implemented in SAGA", p);
        break;

        // generic handler and dump of other exceptions
      default:
        carp ("saga exception caught:\n" + std::string (e.what ()), p);
        break;
    }

    return FAILURE;
  }
  catch ( std::exception const & e )
  {
    carp (std::string("exception caught: ") + e.what(), p);
    return FAILURE;
  }
  catch (...)
  {
    carp ("unexpected exception caught", p);
    return FAILURE;
  }


  if ( p.piped )
  {
    p.in  = p.out;
  }

  return SUCCESS;
}


///////////////////////////////////////////////////////////
//
// print a shell prompt, with optional message
//
void shell::set_prompt (void)
{
  prompt_ = saga::url::unescape (cwd_.get_url ().get_string ()) + " # ";
}


///////////////////////////////////////////////////////////
//
// print an error message
//
void shell::carp (std::string text, props & p, bool is_error)
{
  if ( is_error )
    p.err += "Ooops!\n";

  if ( ! text.empty () )
  {
    p.err += text;
    p.err += "\n";
  }
}


void shell::carp (std::string text, bool is_error)
{
  if ( is_error )
    std::cerr << "Ooops!" << std::endl;

  if ( ! text.empty () )
  {
    std::cerr << text << std::endl;
  }
}


///////////////////////////////////////////////////////////
//
// translate a job state into a string
//
std::string shell::state_to_string (saga::job::state state)
{
  switch ( state )
  {
    case saga::job::New       : return ("New      ");
    case saga::job::Running   : return ("Running  ");
    case saga::job::Suspended : return ("Suspended");
    case saga::job::Done      : return ("Done     ");
    case saga::job::Failed    : return ("Failed   ");
    case saga::job::Canceled  : return ("Canceled ");
    default                   : std::cout << "Unknown state: " << state << std::endl;
                                return ("Unknown  ");
  }
}

void shell::usleep (long int usec)
{
  boost::xtime xt;
  boost::xtime_get (&xt, boost::TIME_UTC);
  xt.nsec += usec;
  boost::thread::sleep (xt);
}

std::string shell::expand (std::string in, props & p)
{
  // expand ~/ at begin of string to $HOME, and ~xyz/ to 
  // the home of user xyz (assumed to be $HOME/../xyz/)
  {
    if ( in == "~" )
    {
      in = "$HOME";
    }
    else if ( in.size () >  0   &&
              in[0]      == '~' )
    {
      std::string sub = in.substr (1);
      std::cout << "sub: " << sub << std::endl;

      size_t idx = sub.find ('/');

      if ( idx == 0 )
      {
        in  = "$HOME";
        in += sub;
      }
      else if ( idx == std::string::npos )
      {
        in  = "$HOME/../";
        in += sub;
      }
      else
      {
        std::string user = sub.substr (0, idx);
        std::string path = sub.substr (idx);

        in  = "$HOME/../";
        in += user;
        in += path;
      }
    }
  }

  // then expand env variables
  {
    size_t idx_1 = in.find ("$");
    size_t idx_2 = in.npos;

    if ( idx_1 == in.npos )
    {
      // nothing to do
      return escape (in, p);
    }

    // found variable start, now look for end.  Thats any non-alphanumeric
    // character, but not the opening curly bracket after $.
    //
    // so, lets first check the curly bracket version
    int is_curly = 0;
    if ( in[idx_1 + 1] == '{' )
    {
      idx_2 = in.find ("}", idx_1 + 1);
      is_curly = 1;
    }
    else
    {
      in   += " "; // add space so we find vars at end

      idx_2 = in.find_first_of (",./;:\"'[]() ", idx_1 + 1);

      in.erase (in.length () - 1); // get rid of space again
    }

    if ( idx_2 != in.npos )
    {
      // get the variable
      std::string key = in.substr (idx_1 + 1 + is_curly,
                                   idx_2 - 1 - is_curly);

      if ( env_.find (key) != env_.end () )
      {
        in.replace (idx_1 + is_curly,  // also replace '$'
                    idx_2 - is_curly,
                    env_[key]);
      }
      else
      {
        carp ("No such environment variable " + key, p);
      }
    }
  }

  return escape (in, p);
}

// escape URLs
std::string shell::escape (std::string in, props & p)
{
  std::string out = "";

  for ( unsigned int i = 0; i < in.size (); i++ )
  {
    // leave sequences which look like escape sequences alone
    if ( in[i] == '%' )
    {
      if ( i + 2 < in.size() )
      {
        if ( isxdigit(in[i+1]) && isxdigit(in[i+2]) )
        {
          // found such a sequence
          out += in[i  ];
          out += in[i+1];
          out += in[i+2];
          i += 2;
        }
        else
        {
          out += "%25";
        }
      }
      else
      {
        out += "%25";
      }
    }
    else
    {
      switch(in[i]) {
        case ' ': out += "%20"; break;
//      case '&': out += "%26"; break;
        case '<': out += "%3C"; break;
        case '>': out += "%3E"; break;
        case '"': out += "%22"; break;
        case '#': out += "%23"; break;
        case '$': out += "%24"; break;
        case '\'': out += "%27"; break;
        case '+': out += "%2B"; break;
        case ',': out += "%2C"; break;
//      case ':': out += "%3A"; break;
        case ';': out += "%3B"; break;
        case '=': out += "%3D"; break;
        case '?': out += "%3F"; break;
//      case '@': out += "%40"; break;
        case '[': out += "%5B"; break;
        case '\\': out += "%5C"; break;
        case ']': out += "%5D"; break;
        case '^': out += "%5E"; break;
        case '`': out += "%60"; break;
        case '{': out += "%7B"; break;
        case '|': out += "%7C"; break;
        case '}': out += "%7D"; break;
        case '~': out += "%7E"; break;
        default:  out += in[i]; break;
      }
    }
  }

  return (out);
}


std::string shell::get_input (std::string prompt)
{

  if ( prompt == "" )
  {
    prompt = prompt_;
  }


#if SAGA_HAVE_READLINE == 1


  char* line = ::readline (prompt.c_str ());

  input_ = "quit";
  
  if ( NULL != line )
  {
    input_ = line;
  }

  boost::algorithm::trim(input_);

  if ( input_ != ""          &&
       input_ != "quit"      &&
       input_ != last_input_ )
  {
    add_history (input_.c_str ());
  }


#else


  std::cout << prompt << std::flush;

  std::getline (std::cin, input_);

  if ( ! std::cin.good () )
  {
    return "quit";
  }


#endif

  last_input_ = input_;
  return (input_);
}


std::string shell::get_input_nb (void)
{
  return ("");
}


/////////////////////////////////////////////////////////////////
//
// split string, but at string delimiter, not at a single
// character...
//
std::vector <std::string> shell::split_input_str (std::string input,
                                                  props     & p,
                                                  std::string delim)
{
  std::vector <std::string> elems;

  size_t len = delim.length ();
  size_t pos = input.find   (delim, 0);

  while ( pos != input.npos )
  {
    // store first element
    std::string elem = input.substr (0, pos - 1);
    elems.push_back (elem);

    // erase first element
    input.erase  (0, pos + len);

    pos = input.find   (delim, 0);
  }

  elems.push_back (input);

  return elems;
}

/////////////////////////////////////////////////////////////////
//
// split line in command and arguments, expand shell vars
// on the fly.
//
std::vector <std::string> shell::split_input (std::string input,
                                              props     & p,
                                              std::string delim)
{
  std::vector <std::string> elems;

  bool echo = false;

  if ( input == "" )
  {
    return (elems);
  }

  std::string tmp     = "";
  bool        quoted  = false;
  bool        escaped = false;

  for ( unsigned int i = 0; i < input.length (); i++ )
  {
    bool add_char = false;

    if ( escaped )
    {
      add_char = true;
      escaped  = false;
    }
    else
    {
      if ( input[i] == '\\' )
      {
        escaped = true;
      }

      else if ( input[i] == '\'' )
      {
        quoted = ! quoted;
      }

      else
      {
        if ( quoted )
        {
          add_char = true;
        }
        else
        {
          add_char = true;

          for ( unsigned int j = 0; j < delim.length (); j++ )
          {
            if ( add_char && input[i] == delim[j] )
            {
              if ( tmp != "" )
              {
                elems.push_back (tmp);
                tmp = "";
              }
              add_char = false;
            }
          }
        }
      }
    }

    if ( add_char )
    {
      tmp += input[i];
    }
  }

  // add trailing element
  if ( tmp != "" )
  {
    if ( tmp != "" )
    {
      elems.push_back (tmp);
    }
  }


  // expand and trim all elements
  std::vector <std::string> expanded;

  for ( unsigned int k = 0; k < elems.size (); k++ )
  {
    std::string tmp = expand (elems[k], p);
    
    // std::cout << "'" << elems[k] << "' -> '" << tmp << "'\n"; 

    if ( tmp != elems[k] )
    {
      echo = true;
    }

    expanded.push_back (tmp);
  }


  // print command line again if we expanded any element...
  if ( echo )
  {
    std::vector <std::string> :: iterator begin = expanded.begin ();
    std::vector <std::string> :: iterator end   = expanded.end ();
    std::vector <std::string> :: iterator iter;

    std::cout << "> ";

    for ( iter = begin; iter != end; ++iter )
    {
      std::string::size_type found = iter->find_first_of(" ");

      if ( std::string::npos != found )
      {
        std::cout << "\"" << *iter << "\" ";
      }
      else
      {
        std::cout << *iter << " ";
      }
    }

    std::cout << std::endl;
  }

  return expanded;
}

template <class T>
inline std::string shell::to_string (const T & t)
{
  std::stringstream ss;
  ss << t;
  return ss.str ();
}

