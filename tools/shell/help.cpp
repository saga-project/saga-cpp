//  Copyright (c) 2005-2008 Andre Merzky
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "saga-shell.hpp"

// print a short help: list available commands and their syntax
int shell::c_help (std::vector <std::string> & args, 
                   props & p)
{
  std::cout << "                                                " << std::endl 
            << " Available commands are:                        " << std::endl 
            << "                                                " << std::endl 
            << "    # pwd                                       " << std::endl 
            << "    # cd            <dir>                       " << std::endl 
            << "    # touch         <file>                      " << std::endl 
            << "    # ls                                        " << std::endl 
            << "    # cp            <source> <target>           " << std::endl 
            << "    # mv            <source> <target>           " << std::endl 
            << "    # rm            <file>                      " << std::endl 
            << "    # ln            <source> <target>           " << std::endl 
            << "    # mkdir         <dir>                       " << std::endl 
            << "    # rmdir         <dir>                       " << std::endl 
            << "                                                " << std::endl 
            << "    # cat           <file>                      " << std::endl 
            << "    # wc            <file>                      " << std::endl 
            << "                                                " << std::endl 
            << "    # rep_find      <pat> [<key=val> ...]       " << std::endl
            << "    # rep_list                                  " << std::endl
            << "    # rep_add       <logical file> <url>        " << std::endl
            << "    # rep_remove    <logical file> <url>        " << std::endl
            << "    # rep_update    <logical file> <old> <new>  " << std::endl
            << "    # rep_replicate <logical file> <target>     " << std::endl
            << "                                                " << std::endl 
            << "    # run           <exe>                       " << std::endl 
            << "    # ps                                        " << std::endl 
            << "    # suspend       <job>                       " << std::endl 
            << "    # resume        <job>                       " << std::endl 
            << "    # kill          <job>                       " << std::endl 
            << "    # status        <job>                       " << std::endl 
            << "                                                " << std::endl 
            << "    # env                                       " << std::endl 
            << "    # getenv        <key>                       " << std::endl 
            << "    # setenv        <key> <val>                 " << std::endl 
            << "    # unsetenv      <key>                       " << std::endl 
            << "    # source                                    " << std::endl 
            << "    # .                                         " << std::endl 
            << "                                                " << std::endl 
            << "    # add_proxy     <type>                      " << std::endl 
            << "    # remove_proxy  <type>                      " << std::endl 
            << "                                                " << std::endl 
            << "    # help                                      " << std::endl 
            << "    # exit/quit                                 " << std::endl 
            << "                                                " << std::endl;
  
  return SUCCESS;
}

