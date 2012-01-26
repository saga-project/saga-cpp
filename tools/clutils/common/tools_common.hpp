
#include <map>

#include <saga/saga.hpp>

// the 'common' class performs some extend of command line parsing, for two
// reasons: 
//
//  - provide means to implement cl args common to all sl tools 
//  - avoid arg parsing code duplication in all cl tools.
//
//  The class 'common' provides a 'init()' method, which accepts argc/argv, and
//  from those filters the common cl args, and puts the remaining args into
//  a vector<string>.  The cl tools are usually used like
//
//    saga-tool <command> <cmd-arguments> [...]
//
//  The args vector this contains the cmd string args args[0], and the
//  cmd-arguments beyond that.
//
//  The methods 'need_args_min(n)', 'need_args_max(n)' and 'need_args(n)' check
//  the number of 'cmd-arguments'.  For example, 'saga-file copy' would require
//  exactly two args (source and target of the copy operation), and would thus
//  call
//
//    c.need_args (2)
//
//  which is equivalent to 
//
//    c.need_args_man (2)
//    c.need_args_max (2)
//  
//  c.nargs () gives the number of cmd-arguments, thus would give '2' for
//
//    saga-file copy src tgt
//
//  c.args () returns a *reference* to the vector<string>, which is thus of 
//  size "c.nargs () + 1"
//

namespace saga_tools
{
  class common;
  class usage_printer;
}


namespace saga_tools
{
  class usage_printer 
  {
    private:
      std::string cmd_;
      std::string name_;
      std::string short_;
      std::string long_;
      std::string common_short_;
      std::string common_long_;

      std::map <std::string, std::string> map_;


    public:
      usage_printer (void);
     ~usage_printer (void);

      void set_cmd          (std::string s);
      void set_name         (std::string s);
      void set_short_common (std::string s);
      void set_long_common  (std::string s);
      void set_short        (std::string s);
      void set_long         (std::string s);

      void print            (std::string err);

      std::map <std::string, std::string> &
           map              (void) { return map_; };
  };

  class common
  {
    private:
      usage_printer             usage_;
      std::string               session_name_;
      std::string               config_file_;
      std::vector <std::string> args_;

      saga::session             session_;

      void eval_args           (unsigned int argc, 
                                char**       argv);

      // tool and session state is stored in an advert database, in
      // $HOME/.saga/saga_shell.db
      /// saga::advert::directory state_basedir_;

    public:
      common  (void);
      ~common (void);

      void          init                 (unsigned int argc, 
                                          char**       argv);
      std::vector <std::string> &
                    args                 (void);
      int           nargs                (void);
      void          need_args            (unsigned int n);
      void          need_args_min        (unsigned int n);
      void          need_args_max        (unsigned int n);

      void          print_usage          (std::string  err = "");

      void          fail                 (int n = -1);
      void          fail                 (const std::string     & e, int n = -1);
      void          fail                 (const saga::exception & e, int n = -1);
      void          fail                 (const std::exception  & e, int n = -1);

      void          dump                 (void);

      void          load_state           (std::string session_name = "default");
      void          save_state           (std::string session_name = "default");
      void          dump_state           (std::string session_name = "default");


      void          session              (saga::session s);
      saga::session session              (void);

      usage_printer & 
                    usage                (void);

      void          add_history          (void);
      void          get_history          (int idx);
      void          dump_history         (void);
  };
}

