//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_PACKAGES_JOB_JOB_DESCRIPTION_HPP
#define SAGA_IMPL_PACKAGES_JOB_JOB_DESCRIPTION_HPP

#include <saga/saga/packages/job/config.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/attribute.hpp>
#include <saga/impl/engine/object.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    // job description implementation
    class SAGA_JOB_PACKAGE_EXPORT description 
      : public saga::impl::object, 
        public saga::impl::attribute
    {
      typedef saga::impl::attribute attribute_base;
      
      explicit description(attribute const& attr);


    protected:
      explicit description(saga::object::type t);


     public:
      /**
       * Constructor of description
       */
      description ();

      /*! destructor of description, destroys description 
       */
      ~description ();

      saga::impl::attribute* get_attributes() { return this; }
      saga::impl::attribute const* get_attributes() const { return this; }
      
      // Generate a exact deep copy of this object
      saga::object clone() const;
    };

  } // namespace impl

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_IMPL_PACKAGES_JOB_JOB_DESCRIPTION_HPP)

