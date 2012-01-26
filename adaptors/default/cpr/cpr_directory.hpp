//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_CPR_DIRECTORY_HPP
#define ADAPTORS_DEFAULT_CPR_DIRECTORY_HPP

#include <fstream>
#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/packages/cpr_directory_cpi_instance_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/cpr/cpr_directory_cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace cpr
{
    ///////////////////////////////////////////////////////////////////////////////
    //  This adaptor implements the functionality of the Saga API "cpr".
    // 
    //  @note This adaptor is written for test purposes only; it is possible that
    //        it does not function exactly like the Saga API dictates.
    //  @see The documentation of the Saga API
    class cpr_directory_cpi_impl 
        : public saga::adaptors::v1_0::cpr_directory_cpi <cpr_directory_cpi_impl>
    {
    private:
        typedef saga::adaptors::v1_0::cpr_directory_cpi <cpr_directory_cpi_impl> base_cpi;

        saga::advert::directory * ad_;

        void check_ld (void);

    public:    
        // constructor of the file adaptor 
        cpr_directory_cpi_impl (proxy                * p, 
                                cpi_info       const & info, 
                                saga::ini::ini const & glob_ini, 
                                saga::ini::ini const & adap_ini,
                                TR1::shared_ptr <saga::adaptor> adaptor);

        // destructor of the file adaptor 
        ~cpr_directory_cpi_impl  (void);

        // attribute functions
        void sync_attribute_exists      (bool & test, std::string key);
        void sync_attribute_is_readonly (bool & test, std::string key);
        void sync_attribute_is_writable (bool & test, std::string key);
        void sync_attribute_is_vector   (bool & test, std::string key);
        void sync_attribute_is_extended (bool & test, std::string key);
        void sync_get_attribute         (std::string               & val,  std::string key);
        void sync_set_attribute         (saga::impl::void_t        & nul,  std::string key, std::string val);
        void sync_get_vector_attribute  (std::vector <std::string> & val,  std::string key);
        void sync_set_vector_attribute  (saga::impl::void_t        & nul,  std::string key, 
                                                                           std::vector <std::string> val);
        void sync_remove_attribute      (saga::impl::void_t        & nul,  std::string key);
        void sync_list_attributes       (std::vector<std::string>  & keys);
        void sync_find_attributes       (std::vector<std::string>  & keys, std::string);

        // namespace_entry functions
        void sync_get_url   (saga::url & url);
        void sync_get_cwd   (saga::url & url);
        void sync_get_name  (saga::url & url);
//      void sync_read_link (saga::url & url);
        void sync_is_dir    (bool & test);
        void sync_is_entry  (bool & test);
        void sync_is_link   (bool & test);
//      void sync_copy      (saga::impl::void_t & nul, saga::url url, int flags);
//      void sync_link      (saga::impl::void_t & nul, saga::url url, int flags);
//      void sync_move      (saga::impl::void_t & nul, saga::url url, int flags);
        void sync_remove    (saga::impl::void_t & nul,                int flags);
        void sync_close     (saga::impl::void_t & nul, double timeout);

        // namespace_dir functions
        void sync_change_dir      (saga::impl::void_t & nul, saga::url url);
        void sync_list            (std::vector<saga::url> & urls, std::string pat, int flags);
        void sync_find            (std::vector<saga::url> & urls, std::string pat, int flags);
//      void sync_read_link       (saga::url & tgt, saga::url name);
        void sync_exists          (bool & test, saga::url name);
        void sync_is_dir          (bool & test, saga::url name);
        void sync_is_entry        (bool & test, saga::url name);
        void sync_is_link         (bool & test, saga::url name);
        void sync_get_num_entries (std::size_t & num);
        void sync_get_entry       (saga::url    & url, std::size_t idx);
//      void sync_copy            (saga::impl::void_t & nul, saga::url name, saga::url url, int flags);
//      void sync_link            (saga::impl::void_t & nul, saga::url name, saga::url url, int flags);
//      void sync_move            (saga::impl::void_t & nul, saga::url name, saga::url url, int flags);
        void sync_remove          (saga::impl::void_t & nul, saga::url name, int flags);
        void sync_make_dir        (saga::impl::void_t & nul, saga::url name, int flags);
        void sync_open            (saga::name_space::entry     & out, saga::url name, int flags);
        void sync_open_dir        (saga::name_space::directory & out, saga::url name, int flags);

        // cpr_directory functions
        void sync_is_checkpoint   (bool                    & test, saga::url name);
        void sync_open            (saga::cpr::checkpoint   & out,  saga::url name, int flags);
        void sync_open_dir        (saga::cpr::directory    & out,  saga::url name, int flags);
        void sync_find            (std::vector <saga::url> & urls, std::string name_pat, 
                                                                   std::vector<std::string> key_pat, 
                                                                   int flags, std::string);
 };  // class cpr_directory_cpi_impl

///////////////////////////////////////////////////////////////////////////////
}   // namespace cpr

#endif // ADAPTORS_DEFAULT_CPR_DIRECTORY_HPP

