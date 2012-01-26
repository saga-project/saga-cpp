//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2009 João Abecasis
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// System Header Files 

#include <iostream>

#include <boost/version.hpp>
#include <saga/saga/version.hpp>
#include <saga/saga/adaptors/utils/ini/ini.hpp>
#include <saga/impl/engine/ini/ini.hpp>

// --------------------------------------------------------
//
//
//
saga::ini::section::section (shared_sec  impl)
    : impl_ (impl)
{
}

// --------------------------------------------------------
//
//
//
saga::ini::section::section (std::string filename)
    : impl_ (new impl_sec (filename))
{
}

// --------------------------------------------------------
//
//
//
saga::ini::section::section (const section & in)
    : impl_ (in.get_impl())
{
}

// --------------------------------------------------------
//
//
//
saga::ini::section::section (impl_sec   * in)
#if BOOST_VERSION >= 103900
    : impl_ (in)
#else
    : impl_ (in->_internal_weak_this.use_count() ?
        in->shared_from_this() : shared_sec(in))
#endif
{
}

// --------------------------------------------------------
//
//
//
saga::ini::section::~section ()
{
}

// --------------------------------------------------------
//
//
//
void saga::ini::section::read (std::string filename)
{
    impl_->read (filename);
}

// --------------------------------------------------------
//
//
//
void saga::ini::section::parse (std::string sourcename, 
                                std::vector <std::string> lines)
{
    impl_->parse (sourcename, lines);
}

// --------------------------------------------------------
//
//
//
void saga::ini::section::merge (std::string second)
{
    impl_->merge (second);
}

// --------------------------------------------------------
//
//
//
void saga::ini::section::merge (const saga::ini::section & second)
{
    impl_->merge (second.impl_);
}

// --------------------------------------------------------
//
//
//
void saga::ini::section::dump (int            ind, 
                               std::ostream & strm) const
{
    impl_->dump (ind, strm);
}

// --------------------------------------------------------
//
//
//
void saga::ini::section::add_section (std::string   sec_name, 
                                const section     & sec)
{
    impl_->get_section (sec_name)->merge (sec.impl_);
}

// --------------------------------------------------------
//
//
//
bool saga::ini::section::has_section (std::string sec_name) const
{
    return impl_->has_section (sec_name);
}

// --------------------------------------------------------
//
//
//
bool saga::ini::section::has_section_full (std::string sec_name) const
{
    return impl_->has_section_full (sec_name);
}

// --------------------------------------------------------
//
//
//
saga::ini::section saga::ini::section::get_section (std::string sec_name) const
{
    return saga::ini::section (impl_->get_section (sec_name));
}

// --------------------------------------------------------
//
//
//
saga::ini::section_map saga::ini::section::get_sections () const
{
    typedef saga::impl::ini::section_map::reference section_impl;
    typedef saga::impl::ini::section_map::const_iterator iterator;

    saga::ini::section_map out;
    iterator end = impl_->get_sections().end();
    for (iterator it = impl_->get_sections().begin(); it != end; ++it)
        out[(*it).first] = saga::ini::section((*it).second);

    return out;
}

// --------------------------------------------------------
//
//
//
void saga::ini::section::add_entry (std::string key,                 
                                    std::string val)
{
    impl_->add_entry (key, val);
}

// --------------------------------------------------------
//
//
//
bool saga::ini::section::has_entry (std::string key) const
{
    return impl_->has_entry (key);
}

// --------------------------------------------------------
//
//
//
std::string saga::ini::section::get_entry (std::string key) const
{
    return impl_->get_entry (key);
}

// --------------------------------------------------------
//
//
//
std::string saga::ini::section::get_entry (std::string key,
                                           std::string dflt_val) const
{
    return impl_->get_entry (key, dflt_val);
}

// --------------------------------------------------------
//
//
//
saga::ini::entry_map saga::ini::section::get_entries (void) const
{
    return impl_->get_entries ();
}

// --------------------------------------------------------
//
//
//
saga::ini::section saga::ini::section::get_root (void) const  
{
    return saga::ini::section (impl_->get_root ());
}

// --------------------------------------------------------
//
//
//
std::string saga::ini::section::get_name (void) const  
{
    return impl_->get_name ();
}

// --------------------------------------------------------
//
//
//
void saga::ini::section::debug (std::string msg) const
{
  std::cout << "debuggin [" 
            << impl_.use_count ()
            << "] ======= " 
            << impl_->get_name ()
            << "\" -- \"" 
            << msg 
            << "\""
            << std::endl;
}

