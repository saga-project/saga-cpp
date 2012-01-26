//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_ATTRIBUTE_HPP
#define SAGA_ADAPTORS_ATTRIBUTE_HPP

// include stl
#include <vector>
#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/attribute.hpp>
#include <saga/saga/adaptors/attribute_cpi.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

  /////////////////////////////////////////////////////////////////////////////
  //  this is the same as the saga::adaptor class, except it is possible to set
  //  read only attributes
  class SAGA_EXPORT attribute 
  :   public saga::detail::attribute<attribute>
  {
    public:
      typedef saga::impl::object implementation_base_type;

      // just defined for brevity
      typedef std::vector <std::string>              strvec_type;
      typedef std::map    <std::string, std::string> strmap_type;

    private:
      friend struct saga::detail::attribute<attribute>;  // needs to access get_impl()

      // This members get initialized differently depending on the constructor 
      // used:
      //  - if this object got constructed from a saga::object, then the impl_ 
      //    member gets initialized directly with the corresponding 
      //    implementation object (saga::impl::object), and the attr_ member
      //    is initialized from the saga::impl::attribute corresponding to the
      //    given impl_.
      //  - if this object got constructed from a saga::impl::v1_0::cpi*,
      //    we assume a cpi instance wants to get access to _its_own_
      //    attributes. We initialize the impl_ from the corresponding proxy,
      //    and the attr_ from the (default) attribute_cache_cpi implementation 
      //    our cpi type has been derived from.
      saga::impl::object* impl_;
      saga::impl::attribute_interface* attr_;
      bool must_delete;

      template <typename Tag> 
        saga::task set_attributepriv (std::string const&, std::string const&, Tag);
      template <typename Tag> 
        saga::task set_vector_attributepriv(std::string const&, strvec_type const&, Tag);
      template <typename Tag> 
        saga::task remove_attributepriv(std::string const&, Tag);

      void set_attribute_sync(std::string const&, std::string const&);
      void set_vector_attribute_sync(std::string const&, strvec_type const&);
      void remove_attribute_sync(std::string const&);

    protected:
      bool is_impl_valid() const { return impl_ ? true : false; }

    public:
      saga::impl::object* get_impl() const { return impl_; }
      saga::impl::attribute_interface* get_attr() const { return attr_; }

      // ctor/dtor
      explicit attribute (saga::impl::v1_0::attribute_cpi *impl);           // adaptor interface
      explicit attribute (saga::impl::v1_0::attribute_cpi const* impl);     // adaptor interface
      attribute (saga::object rhs);
      attribute (saga::impl::object const* rhs);
      ~attribute (void);

      /*! \brief Brief %description starts here    
       *
       *
       */  
      void set_attribute(std::string const& key, std::string const& value) 
      {
          set_attribute_sync(key, value);
      }
      SAGA_CALL_PUB_2_DEF_0(set_attribute, std::string, std::string)

      /*! \brief Brief %description starts here    
       *
       *
       */  
      void set_vector_attribute(std::string key, strvec_type value) 
      {
          set_vector_attributepriv(key, value, saga::task_base::Sync());
      }
      SAGA_CALL_PUB_2_DEF_0(set_vector_attribute, std::string, strvec_type)

      /*! \brief Brief %description starts here    
       *
       *
       */  
      void remove_attribute(std::string key) 
      {
          remove_attributepriv(key, saga::task_base::Sync());
      }
      SAGA_CALL_PUB_1_DEF_0(remove_attribute, std::string)
  };

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::adaptors

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_ADAPTORS_ATTRIBUTE_HPP

