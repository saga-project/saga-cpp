//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_CPI_INFO_HPP
#define SAGA_IMPL_ENGINE_CPI_INFO_HPP

#include <string>
#include <set>
#include <vector>

#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/preferences.hpp>
#include <saga/saga/uuid.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 
{
  // information about one method from adaptor
  class op_info
  {
    public:
      typedef void       (cpi::*sync_func)();
      typedef saga::task (cpi::*async_func)();
      typedef bool       (cpi::*prep_func)();

    private:
      std::string     name_;
      preference_type prefs_;
      sync_func       sync_;
      async_func      async_;
      prep_func       prep_;

    public:
      op_info (std::string n, preference_type p, sync_func sync, 
              async_func async = NULL, prep_func prep = NULL)
        : name_(n), prefs_(p), sync_(sync), async_(async), prep_(prep)
      {}
      explicit op_info (std::string name)
        : name_(name), sync_(NULL), async_(NULL), prep_(NULL)
      {}

      std::string            get_name()  const { return name_;  }
      preference_type const& get_prefs() const { return prefs_; }

      sync_func  get_sync_func()  const { return sync_;  }
      async_func get_async_func() const { return async_; }
      prep_func  get_prep_func()  const { return prep_;  }

      friend bool operator< (op_info const& lhs, op_info const& rhs)
      {
          return lhs.name_ < rhs.name_;
      }
  };

  // information about one cpi implementation 
  class cpi_info
  {
    public:
      typedef std::set<op_info> op_list_type;

    private:
      std::string       ops_cpi_name_;  // cpi type the functions belongs to
      std::string       cpi_name_;      // cpi type implementing the function
      op_list_type      ops_;           // function definitions
      cpi::maker_type   maker_;         // factory function for this cpi
      preference_type   prefs_;         // cpi preferences
      saga::uuid        adaptor_uuid_;  // uuid of associated adaptor
      saga::uuid        cpi_uuid_;      // uuid of this cpi_info

    public:
      cpi_info() : maker_(NULL) {}
      cpi_info (std::string     const& ops_cpi_name, 
                std::string     const& cpi_name, 
                cpi::maker_type        maker,
                preference_type const& prefs,
                saga::uuid      const& cpi_uuid,
                saga::uuid      const& adaptor_uuid)
        : ops_cpi_name_ (ops_cpi_name),
          cpi_name_     (cpi_name), 
          maker_        (maker), 
          prefs_        (prefs),
          adaptor_uuid_ (adaptor_uuid),
          cpi_uuid_     (cpi_uuid)
      {
      }

   // friend bool operator== (cpi_info const& lhs, cpi_info const& rhs) 
   // {
   //     return lhs.cpi_name_     == rhs.cpi_name_     &&
   //            lhs.ops_cpi_name_ == rhs.ops_cpi_name_ && 
   //            lhs.prefs_        == rhs.prefs_;
   // }

      // this is used for set sorting
      friend bool operator< (cpi_info const& lhs, cpi_info const& rhs) 
      {
          return lhs.cpi_name_     < rhs.cpi_name_     &&
                 lhs.ops_cpi_name_ < rhs.ops_cpi_name_ && 
                 lhs.prefs_        < rhs.prefs_;
      }

      void add_op (op_info const& i) { ops_.insert(i); }

      op_list_type const    & get_ops          (void) const { return ops_;          }
      cpi::maker_type         get_maker        (void) const { return maker_;        }
      preference_type const & get_prefs        (void) const { return prefs_;        }
      std::string             get_cpi_name     (void) const { return cpi_name_;     }
      std::string             get_ops_cpi_name (void) const { return ops_cpi_name_; }
      saga::uuid const      & get_adaptor_id   (void) const { return adaptor_uuid_; }
      saga::uuid const      & get_cpi_id       (void) const { return cpi_uuid_;     }
  };

  //  this functor is used to test, whether a given adaptor info describes
  //  the same cpi type as the one stored inside the functor
  struct same_cpi
  {
      explicit same_cpi(v1_0::cpi_info const& info) 
      :   info_(info) 
      {}

      bool operator()(v1_0::cpi_info const& info) const
      {
          return info_.get_cpi_id() == info.get_cpi_id();
      }

      v1_0::cpi_info const& info_;
  };

///////////////////////////////////////////////////////////////////////////////
}}}  // namespace saga::impl::v1_0

#endif // SAGA_IMPL_ENGINE_CPI_INFO_HPP

