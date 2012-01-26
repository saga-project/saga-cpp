//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// STL includes
#include <iostream>
#include <vector>

// SAGA includes
#include <saga/saga.hpp>

// system includes
#if SAGA_HAVE_READLINE == 1
# include <readline/readline.h>
# include <readline/history.h>
#endif

// local includes
#include "joblist.hpp"

// local defines
#define SUCCESS 0
#define FAILURE 1


class props
{
  public:
    props (std::string)
    {
      in    = out = err = "";
      bg    = false;
      piped = false;
    }

    props (void)
    {
      in    = out = err = "";
      bg    = false;
      piped = false;
    }

    std::string in;
    std::string out;
    std::string err;
    bool        bg;
    bool        piped;
};

class shell 
{
  private:
    // the shell we are working in
    saga::session               session_;

    // shell environment      
    std::map <std::string, 
              std::string>      env_;

    // the directory we operate on
    saga::url                   home_;
    saga::name_space::directory cwd_;

    // the job management facility
    saga::url                   contact_;
    std::string                 host_;
    saga::job::service          js_;
    std::string                 history_;

    // map of active job pids to saga job ids
    joblist                     jobs_;

    // prompt
    std::string                 prompt_;
    std::string                 input_;
    std::string                 last_input_;

    // the commands
    // - namespace related
    int  c_pwd           (std::vector <std::string> & args, props & p);
    int  c_cd            (std::vector <std::string> & args, props & p);
    int  c_touch         (std::vector <std::string> & args, props & p);
    int  c_ls            (std::vector <std::string> & args, props & p);
    int  c_cp            (std::vector <std::string> & args, props & p);
    int  c_mv            (std::vector <std::string> & args, props & p);
    int  c_rm            (std::vector <std::string> & args, props & p);
    int  c_ln            (std::vector <std::string> & args, props & p);
    int  c_mkdir         (std::vector <std::string> & args, props & p);
    int  c_rmdir         (std::vector <std::string> & args, props & p);

    // file related      
    int  c_cat           (std::vector <std::string> & args, props & p);
    int  c_wc            (std::vector <std::string> & args, props & p);

    // logical_file related
    int  c_rep_find      (std::vector <std::string> & args, props & p);
    int  c_rep_list      (std::vector <std::string> & args, props & p);
    int  c_rep_add       (std::vector <std::string> & args, props & p);
    int  c_rep_remove    (std::vector <std::string> & args, props & p);
    int  c_rep_update    (std::vector <std::string> & args, props & p);
    int  c_rep_replicate (std::vector <std::string> & args, props & p);
    
    // job related
    int  c_run           (std::vector <std::string> & args, props & p);
    int  c_run2          (std::vector <std::string> & args, props & p);
    int  c_exec          (std::vector <std::string> & args, props & p);
    int  c_ps            (std::vector <std::string> & args, props & p);
    int  c_suspend       (std::vector <std::string> & args, props & p);
    int  c_resume        (std::vector <std::string> & args, props & p);
    int  c_kill          (std::vector <std::string> & args, props & p);
    int  c_status        (std::vector <std::string> & args, props & p);

    // core shell things
    int  c_init          (std::vector <std::string> & args, props & p);
    int  c_history       (std::vector <std::string> & args, props & p);
    int  c_echo          (std::vector <std::string> & args, props & p);
    int  c_env           (std::vector <std::string> & args, props & p);
    int  c_setenv        (std::vector <std::string> & args, props & p);
    int  c_getenv        (std::vector <std::string> & args, props & p);
    int  c_unsetenv      (std::vector <std::string> & args, props & p);
    int  c_source        (std::vector <std::string> & args, props & p);
    int  c_help          (std::vector <std::string> & args, props & p);
    int  c_add_proxy     (std::vector <std::string> & args, props & p);
    int  c_remove_proxy  (std::vector <std::string> & args, props & p);


    // print shell prompt and history
    void set_prompt             (void);

    // print an error message
    void carp                   (std::string text, 
                                 props     & p,
                                 bool        is_error = true);
    void carp                   (std::string text, 
                                 bool        is_error = true);

    // re-initialize some shell settings
    void init                   (void);

    // helper
    std::string state_to_string (saga::job::state state);
    void usleep                 (long int usec);
    template <typename T>
    std::string to_string       (const T & in);

    // command parsing and execution
    std::string get_input       (std::string prompt = "");
    std::string get_input_nb    (void); // non-blocking
    std::vector <std::string>
                split_input     (std::string input, 
                                 props     & p, 
                                 std::string delim = " \t\n");
    std::vector <std::string>
                split_input_str (std::string input, 
                                 props     & p, 
                                 std::string delim = "&&");

    std::string expand          (std::string in, props & p);
    std::string escape          (std::string in, props & p);
    int         execute         (std::string input, props & p);
    int         execute_ands    (std::string input, props & p);
    int         execute_ors     (std::string input, props & p);
    int         execute_pipes   (std::string input, props & p);
    int         execute_command (std::string input, props & p);
    int         execute_command (std::vector <std::string> args, props & p);

    std::string init_contact    (void);
    std::string init_home       (void);
    std::string init_host       (const saga::url & url);

    void        init_env        (char* envp[]);
    void        init_history    (void);
    void        close_history   (void);

  public:
    // c'tor/d'tor
    shell (void);
   ~shell (void);

    // the main event loop
    void run (int argc, char * argv[], char * envp[]);
};


