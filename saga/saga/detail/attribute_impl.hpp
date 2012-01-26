//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_DETAIL_ATTRIBUTE_IMPL_HPP
#define SAGA_SAGA_DETAIL_ATTRIBUTE_IMPL_HPP

// include stl
#include <vector>
#include <string>

#include <saga/saga/impl_base.hpp>
#include <saga/impl/engine/metric.hpp>

// include dependent spec sections
#include <saga/saga/detail/attribute.hpp>
#include <saga/saga/detail/dispatch_priv.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace detail
{
    template <typename Derived>
    impl::attribute_interface*
    attribute<Derived>::get_attr (void) 
    { 
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        return derived().get_impl()->get_attributes(); 
    }
    
    template <typename Derived>
    impl::attribute_interface*
    attribute<Derived>::get_attr (void) const 
    { 
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        return derived().get_impl()->get_attributes(); 
    }

    template <typename Derived>
    template <typename ImplType>
    ImplType*
    attribute<Derived>::get_target_object (void) const 
    { 
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        return boost::get_pointer(derived().get_impl()); 
    }

    template <typename Derived>
    inline void 
    attribute<Derived>::init (
        char const* const* scalar_ro, char const* const* scalar_rw,
        char const* const* vector_ro, char const* const* vector_rw)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        derived().get_impl()->get_attributes()->init(scalar_ro, scalar_rw, vector_ro, vector_rw);
    }

    template <typename Derived>
    inline void 
    attribute<Derived>::init (strmap_type const& scalar_ro,
        strmap_type const& scalar_rw, strmap_type const& vector_ro,
        strmap_type const& vector_rw)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        derived().get_impl()->get_attributes()->init(scalar_ro, scalar_rw, vector_ro, vector_rw);
    }

    template <typename Derived>
    inline void 
    attribute<Derived>::init (bool extensible, bool cache_only)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        derived().get_impl()->get_attributes()->init(extensible, cache_only);
    }

    template <typename Derived>
    inline void 
    attribute<Derived>::init_keynames(char const* const* keynames)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        derived().get_impl()->get_attributes()->init_keynames(keynames);
    }

    template <typename Derived>
    inline void 
    attribute<Derived>::init_keynames (strvec_type const& keynames)
    {
        if (!derived().is_impl_valid()) {
            SAGA_THROW("The object has not been properly initialized.",
                saga::IncorrectState);
        }
        derived().get_impl()->get_attributes()->init_keynames(keynames);
    }

    /// @cond

    /////////////////////////////////////////////////////////////////////////////
    /* setters/getters */
    struct get_attribute_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& key, bool sync) 
        {
            impl::attribute_interface* attr = this_.get_attr();
            if (!attr->attribute_exists_sync(key))
            {
              // DoesNotExist
              typedef typename Derived::implementation_base_type base_type;
              SAGA_THROW_VERBATIM(
                  this_.template get_target_object<base_type>(), 
                  "attribute '" + key + "' does not exist", 
                  saga::DoesNotExist);
            }
            return attr->get_attribute (key, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::get_attributepriv (
        Derived const& this_, std::string const& key) 
    {
        return dispatch_priv<Tag>::
            template call<get_attribute_priv>(this_, key);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline std::string
    attribute_sync<Derived>::get_attribute(
        Derived const& this_, std::string const& key) 
    {
        impl::attribute_interface* attr = this_.get_attr();
        if (!attr->attribute_exists_sync(key))
        {
            // DoesNotExist
            typedef typename Derived::implementation_base_type base_type;
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(), 
                "attribute '" + key + "' does not exist", 
                saga::DoesNotExist);
        }
        return attr->get_attribute_sync(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct set_attribute_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, 
            std::string const& key, std::string const& val, bool sync)
        {
            impl::attribute_interface* attr = this_.get_attr();
            if (attr->attribute_exists_sync(key) && 
                attr->attribute_is_readonly_sync(key))
            {
                // ReadOnly
                typedef typename Derived::implementation_base_type base_type;
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(), 
                    "attribute '" + key + "' is readonly", saga::PermissionDenied);
            }
            return attr->set_attribute (key, val, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::set_attributepriv (Derived& this_,
        std::string const& key, std::string const& val) 
    {
        return dispatch_priv<Tag>::
            template call<set_attribute_priv>(this_, key, val);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline void
    attribute_sync<Derived>::set_attribute(
        Derived& this_, std::string const& key, std::string const& val) 
    {
        impl::attribute_interface* attr = this_.get_attr();
        if (attr->attribute_exists_sync(key) && 
            attr->attribute_is_readonly_sync(key))
        {
            // ReadOnly
            typedef typename Derived::implementation_base_type base_type;
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(), 
                "attribute '" + key + "' is readonly", saga::PermissionDenied);
        }
        attr->set_attribute_sync(key, val);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct get_vector_attribute_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& key, bool sync) 
        {
            impl::attribute_interface* attr = this_.get_attr();
            if (!attr->attribute_exists_sync(key))
            {
                // DoesNotExist
                typedef typename Derived::implementation_base_type base_type;
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(),
                    "attribute '" + key + "' does not exist", 
                    saga::DoesNotExist);
            }
            return attr->get_vector_attribute (key, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::get_vector_attributepriv (
        Derived const& this_, std::string const& key) 
    {
        return dispatch_priv<Tag>::
            template call<get_vector_attribute_priv>(this_, key);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline void
    attribute_sync<Derived>::get_vector_attribute(
        Derived const& this_, std::string const& key, strvec_type& retval) 
    {
        impl::attribute_interface* attr = this_.get_attr();
        if (!attr->attribute_exists_sync(key))
        {
            // DoesNotExist
            typedef typename Derived::implementation_base_type base_type;
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(),
                "attribute '" + key + "' does not exist", 
                saga::DoesNotExist);
        }
        attr->get_vector_attribute_sync(key, retval);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct set_vector_attribute_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& key, 
            typename attribute<Derived>::strvec_type const& val, bool sync)
        { 
            impl::attribute_interface* attr = this_.get_attr();
            if (attr->attribute_exists_sync(key) && 
                attr->attribute_is_readonly_sync(key))
            {
                // ReadOnly
                typedef typename Derived::implementation_base_type base_type;
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(),
                    "attribute '" + key + "' is readonly", saga::PermissionDenied);
            }
            return attr->set_vector_attribute (key, val, sync);
        }
     };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::set_vector_attributepriv (
        Derived& this_, std::string const& key, strvec_type const& val) 
    {
        return dispatch_priv<Tag>::
            template call<set_vector_attribute_priv>(this_, key, val);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline void
    attribute_sync<Derived>::set_vector_attribute(
        Derived& this_, std::string const& key, strvec_type const& val) 
    {
        impl::attribute_interface* attr = this_.get_attr();
        if (attr->attribute_exists_sync(key) && 
            attr->attribute_is_readonly_sync(key))
        {
            // ReadOnly
            typedef typename Derived::implementation_base_type base_type;
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(),
                "attribute '" + key + "' is readonly", saga::PermissionDenied);
        }
        attr->set_vector_attribute_sync(key, val);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct remove_attribute_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& key, bool sync)
        {
            typedef typename Derived::implementation_base_type base_type;

            impl::attribute_interface* attr = this_.get_attr();
            if (!attr->attribute_exists_sync(key))
            {
                // DoesNotExist
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(), 
                    "attribute '" + key + "' does not exist", 
                    saga::DoesNotExist);
            }
            if (!this_.template get_target_object<base_type>()->get_attributes()->attributes_extensible() || 
                attr->attribute_is_readonly_sync(key))
            {
                // ReadOnly
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(),
                    "attribute '" + key + "' is readonly", saga::PermissionDenied);
            }
            if (!attr->attribute_is_extended_sync(key))    // !is_removable
            {
                // ReadOnly
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(),
                    "attribute '" + key + "' is readonly", saga::PermissionDenied);
            }
            return attr->remove_attribute(key, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::remove_attributepriv (Derived& this_,
        std::string const& key) 
    {
        return dispatch_priv<Tag>::
            template call<remove_attribute_priv>(this_, key);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline void
    attribute_sync<Derived>::remove_attribute(
        Derived& this_, std::string const& key) 
    {
        typedef typename Derived::implementation_base_type base_type;

        impl::attribute_interface* attr = this_.get_attr();
        if (!attr->attribute_exists_sync(key))
        {
            // DoesNotExist
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(), 
                "attribute '" + key + "' does not exist", 
                saga::DoesNotExist);
        }
        if (!this_.template get_target_object<base_type>()->get_attributes()->attributes_extensible() || 
            attr->attribute_is_readonly_sync(key))
        {
            // ReadOnly
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(),
                "attribute '" + key + "' is readonly", saga::PermissionDenied);
        }
        if (!attr->attribute_is_extended_sync(key))    // !is_removable
        {
            // ReadOnly
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(),
                "attribute '" + key + "' is readonly", saga::PermissionDenied);
        }
        attr->remove_attribute_sync(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    /* inspection */
    struct list_attributes_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, bool sync) 
        {
            return this_.get_attr()->list_attributes(sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::list_attributespriv (Derived const& this_) 
    {
        return dispatch_priv<Tag>::template call<list_attributes_priv>(this_);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline void
    attribute_sync<Derived>::list_attributes(
        Derived const& this_, strvec_type& retval) 
    {
        this_.get_attr()->list_attributes_sync(retval);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct find_attributes_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& pattern, bool sync) 
        {
            return this_.get_attr()->find_attributes(pattern, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::find_attributespriv (
        Derived const& this_, std::string const& pattern) 
    {
        return dispatch_priv<Tag>::
            template call<find_attributes_priv>(this_, pattern);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline void
    attribute_sync<Derived>::find_attributes(
        Derived const& this_, std::string const& pattern, strvec_type& retval) 
    {
        this_.get_attr()->find_attributes_sync(pattern, retval);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct attribute_exists_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& key, bool sync) 
        {
            return this_.get_attr()->attribute_exists(key, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::attribute_existspriv (
        Derived const& this_, std::string const& key) 
    {
        return dispatch_priv<Tag>::
            template call<attribute_exists_priv>(this_, key);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline bool
    attribute_sync<Derived>::attribute_exists(
        Derived const& this_, std::string const& key) 
    {
        return this_.get_attr()->attribute_exists_sync(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct attribute_is_readonly_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& key, bool sync) 
        {
            impl::attribute_interface* attr = this_.get_attr();
            if (!attr->attribute_exists_sync(key))
            {
                // DoesNotExist
                typedef typename Derived::implementation_base_type base_type;
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(),
                    "attribute '" + key + "' does not exist", 
                    saga::DoesNotExist);
            }
            return attr->attribute_is_readonly(key, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::attribute_is_readonlypriv (
        Derived const& this_, std::string const& key) 
    {
        return dispatch_priv<Tag>::
            template call<attribute_is_readonly_priv>(this_, key);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline bool
    attribute_sync<Derived>::attribute_is_readonly(
        Derived const& this_, std::string const& key) 
    {
        impl::attribute_interface* attr = this_.get_attr();
        if (!attr->attribute_exists_sync(key))
        {
            // DoesNotExist
            typedef typename Derived::implementation_base_type base_type;
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(),
                "attribute '" + key + "' does not exist", 
                saga::DoesNotExist);
        }
        return attr->attribute_is_readonly_sync(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct attribute_is_writable_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& key, bool sync) 
        {
            impl::attribute_interface* attr = this_.get_attr();
            if (!attr->attribute_exists_sync(key))
            {
                // DoesNotExist
                typedef typename Derived::implementation_base_type base_type;
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(),
                    "attribute '" + key + "' does not exist", 
                    saga::DoesNotExist);
            }
            return attr->attribute_is_writable (key, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::attribute_is_writablepriv (
        Derived const& this_, std::string const& key) 
    {
        return dispatch_priv<Tag>::
            template call<attribute_is_writable_priv>(this_, key);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline bool
    attribute_sync<Derived>::attribute_is_writable(
        Derived const& this_, std::string const& key) 
    {
        impl::attribute_interface* attr = this_.get_attr();
        if (!attr->attribute_exists_sync(key))
        {
            // DoesNotExist
            typedef typename Derived::implementation_base_type base_type;
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(),
                "attribute '" + key + "' does not exist", 
                saga::DoesNotExist);
        }
        return attr->attribute_is_writable_sync(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct attribute_is_vector_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& key, bool sync) 
        {
            impl::attribute_interface* attr = this_.get_attr();
            if (!attr->attribute_exists_sync(key))
            {
                // DoesNotExist
                typedef typename Derived::implementation_base_type base_type;
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(),
                    "attribute '" + key + "' does not exist", 
                    saga::DoesNotExist);
            }
            return attr->attribute_is_vector(key, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::attribute_is_vectorpriv (
        Derived const& this_, std::string const& key) 
    {
        return dispatch_priv<Tag>::
            template call<attribute_is_vector_priv>(this_, key);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline bool
    attribute_sync<Derived>::attribute_is_vector(
        Derived const& this_, std::string const& key) 
    {
        impl::attribute_interface* attr = this_.get_attr();
        if (!attr->attribute_exists_sync(key))
        {
            // DoesNotExist
            typedef typename Derived::implementation_base_type base_type;
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(),
                "attribute '" + key + "' does not exist", 
                saga::DoesNotExist);
        }
        return attr->attribute_is_vector_sync(key);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct attribute_is_removable_priv
    {
        template <typename Derived>
        static task
        call (Derived& this_, std::string const& key, bool sync) 
        {
            impl::attribute_interface* attr = this_.get_attr();
            if (!attr->attribute_exists_sync(key))
            {
                // DoesNotExist
                typedef typename Derived::implementation_base_type base_type;
                SAGA_THROW_VERBATIM(
                    this_.template get_target_object<base_type>(),
                    "attribute '" + key + "' does not exist", 
                    saga::DoesNotExist);
            }
            return attr->attribute_is_extended(key, sync);
        }
    };

    template <typename Derived, typename Tag>
    inline task
    attribute_priv<Derived, Tag>::attribute_is_removablepriv (
        Derived const& this_, std::string const& key) 
    {
        return dispatch_priv<Tag>::
            template call<attribute_is_removable_priv>(this_, key);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    inline bool
    attribute_sync<Derived>::attribute_is_removable(
        Derived const& this_, std::string const& key) 
    {
        impl::attribute_interface* attr = this_.get_attr();
        if (!attr->attribute_exists_sync(key))
        {
            // DoesNotExist
            typedef typename Derived::implementation_base_type base_type;
            SAGA_THROW_VERBATIM(
                this_.template get_target_object<base_type>(),
                "attribute '" + key + "' does not exist", 
                saga::DoesNotExist);
        }
        return attr->attribute_is_extended_sync(key);
    }

    /// @endcond

///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga::detail

#endif // SAGA_SAGA_DETAIL_ATTRIBUTE_IMPL_HPP

