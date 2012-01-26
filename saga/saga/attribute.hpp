//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ATTRIBUTE_HPP
#define SAGA_ATTRIBUTE_HPP

// include stl
#include <map>
#include <vector>
#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>

#include <saga/saga/detail/attribute.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
//
// This is the relevant SIDL section from the SAGA spec.
//
///////////////////////////////////////////////////////////////////////////////
//
//  package saga.attribute
//  {
//    interface attribute 
//    {
//      // interface - no CONSTRUCTOR/DESTRUCTOR
//
//      // setter / getters
//      set_attribute           (in  string          key, 
//                               in  string          value);
//      get_attribute           (in  string          key, 
//                               out string          value);
//      set_vector_attribute    (in  string          key, 
//                               in  array<string,1> values);
//      get_vector_attribute    (in  string          key,
//                               out array<string,1> values);
//      remove_attribute        (in  string          key);
//
//      // inspection methods   
//      list_attributes         (out array<string,1> keys);
//      find_attributes         (in  string          pattern,
//                               out array<string,1> keys);
//      attribute_exists        (in  string          key,
//                               out bool            test);
//      attribute_is_readonly   (in  string          key,
//                               out bool            test);
//      attribute_is_writable   (in  string          key,
//                               out bool            test);
//      attribute_is_vector     (in  string          key,
//                               out bool            test);
//    }
//  }
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  /////////////////////////////////////////////////////////////////////////////
  namespace attributes
  {
    // common attribute values
    char const* const common_true  = "True";
    char const* const common_false  = "False";
  }

  /*! \brief Brief %description starts here
   *
   *
   */
  class SAGA_EXPORT attribute 
  :   public saga::detail::attribute<attribute>
  {
    private:
      friend struct saga::detail::attribute<attribute>;  // needs to access get_impl()
      TR1::shared_ptr <saga::impl::object> impl_;

    public:
      /// @cond
      /** These methods are not within API scope */
      typedef saga::impl::object implementation_base_type;
      typedef std::vector <std::string> strvec_type;
      typedef std::map <std::string, std::string> strmap_type;
      /// @endcond

    protected:
      /// @cond
      /** These methods are not within API scope */
      TR1::shared_ptr<saga::impl::object> get_impl_sp (void) const { return impl_; }
      saga::impl::object* get_impl (void) const { return impl_.get(); }
      bool is_impl_valid() const { return impl_ ? true : false; }
      /// @endcond

    public:
      /*! \brief Brief %description starts here
       *
       *
       */
      explicit attribute (saga::impl::object *impl);    // adaptor interface

      /*! \brief Brief %description starts here
       *
       *
       */
      attribute (saga::object rhs);

      /*! \brief Brief %description starts here
       *
       *
       */
      attribute (void);

      /*! \brief Brief %description starts here
       *
       *
       */
      ~attribute (void);
  };

///////////////////////////////////////////////////////////////////////////////
} // namespace saga
                                                                             
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_ATTRIBUTE_HPP

