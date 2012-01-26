//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_CPR_CHECKPOINT_HPP
#define ADAPTORS_DEFAULT_CPR_CHECKPOINT_HPP

#include <fstream>
#include <string>

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/packages/cpr_checkpoint_cpi_instance_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/cpr/cpr_checkpoint_cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace cpr
{
    ///////////////////////////////////////////////////////////////////////////////
    //  This adaptor implements the functionality of the Saga API "cpr".
    // 
    //  @note This adaptor is written for test purposes only; it is possible that
    //        it does not function exactly like the Saga API dictates.
    //  @see The documentation of the Saga API
    class cpr_checkpoint_cpi_impl 
        : public saga::adaptors::v1_0::cpr_checkpoint_cpi< cpr_checkpoint_cpi_impl>
    {
    private:
        typedef saga::adaptors::v1_0::cpr_checkpoint_cpi <cpr_checkpoint_cpi_impl> base_cpi;

        saga::advert::entry * ae_;

        void check_lf (void);

    public:
        typedef base_cpi::mutex_type mutex_type;
        
        // constructor of the cpr_checkpoint adaptor 
        cpr_checkpoint_cpi_impl (proxy                * p, 
                                 cpi_info       const & info, 
                                 saga::ini::ini const & glob_ini, 
                                 saga::ini::ini const & adap_ini,
                                 TR1::shared_ptr <saga::adaptor> adaptor);

        // destructor of the cpr checkpoint adaptor 
        ~cpr_checkpoint_cpi_impl  (void);

        // attribute functions
        void sync_attribute_exists      (bool & test, std::string key);
        void sync_attribute_is_readonly (bool & test, std::string key);
        void sync_attribute_is_writable (bool & test, std::string key);
        void sync_attribute_is_vector   (bool & test, std::string key);
        void sync_attribute_is_extended (bool & test, std::string key);
        void sync_get_attribute         (std::string               & val, std::string key);
        void sync_set_attribute         (saga::impl::void_t        & nul, std::string key, 
                                                                          std::string val);
        void sync_get_vector_attribute  (std::vector <std::string> & val, std::string key);
        void sync_set_vector_attribute  (saga::impl::void_t        & nul, std::string key, 
                                                                          std::vector <std::string> val);
        void sync_remove_attribute      (saga::impl::void_t        & nul, std::string key);
        void sync_list_attributes       (std::vector<std::string>  & keys);
        void sync_find_attributes       (std::vector<std::string>  & keys, std::string pat);

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

        // cpr_checkpoint functions
        void sync_get_parent (saga::url & file,              int generation);
        void sync_get_file   (saga::url & file,              int idx);
        void sync_open_file_idx  (saga::filesystem::file & file, int idx, int flags);
        

    // monitoring thread
        boost::thread    * thrd_;
        boost::condition * cond_;
        boost::condition   cancel_thrd_;
        
        bool thread_alive_;
        bool cancel_thread_;

    protected:
        void setup_monitoring_thread (void);
        void monitor_entry (void);
    };  // class cpr_checkpoint_cpi_impl

///////////////////////////////////////////////////////////////////////////////
}   // namespace cpr_checkpoint

#endif // ADAPTORS_DEFAULT_CPR_CHECKPOINT_HPP

