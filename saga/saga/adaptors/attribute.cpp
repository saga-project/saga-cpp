//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/exception.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/attribute.hpp>
#include <saga/saga/context.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/saga/adaptors/attribute_cpi_wrapper.hpp> 

#include <saga/impl/exception.hpp>
#include <saga/impl/engine/object.hpp>
#include <saga/impl/engine/attribute.hpp> 
#include <saga/impl/engine/metric.hpp>
#include <saga/impl/engine/context_base.hpp> 
#include <saga/impl/engine/proxy.hpp> 

#include <saga/saga/detail/attribute_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    attribute::attribute (saga::object rhs)
    :   impl_(rhs.get_impl()), 
        attr_(rhs.get_impl()->get_attributes()),
        must_delete(false)
    {
    }

    attribute::attribute (saga::impl::object const* rhs)
    :   impl_(const_cast<saga::impl::object*>(rhs)), 
        attr_(impl_->get_attributes()),
        must_delete(false)
    {
    }

    attribute::attribute (saga::impl::v1_0::attribute_cpi *cpi_instance)
    :   impl_(cpi_instance->get_proxy()),
        attr_(new attribute_cpi_wrapper(cpi_instance)),
        must_delete(true)
    {
    }

    attribute::attribute (saga::impl::v1_0::attribute_cpi const* cpi_instance)
    :   impl_(cpi_instance->get_proxy()),
        attr_(new const_attribute_cpi_wrapper(cpi_instance)),
        must_delete(true)
    {
    }

    attribute::~attribute (void)
    {
        if (must_delete)
            delete attr_;
    }

    ///////////////////////////////////////////////////////////////////////////
    struct set_attribute_priv
    {
        template <typename Derived>
        static saga::task 
        call (Derived& this_, 
            std::string const& key, std::string const& val, bool sync)
        {
            return this_.get_attr()->set_attribute (key, val, sync);
        }
    };

    template <typename Tag>
    inline saga::task
    attribute::set_attributepriv (
        std::string const& key, std::string const& val, Tag) 
    {
        return detail::dispatch_priv<Tag>::
            template call<set_attribute_priv>(derived(), key, val);
    }

    void attribute::set_attribute_sync(std::string const& key, 
        std::string const& val)
    {
        derived().get_attr()->set_attribute_sync(key, val);
    }

    template saga::task 
    attribute::set_attributepriv<saga::task_base::Sync>(
        std::string const& key, std::string const& val, saga::task_base::Sync);
    template saga::task 
    attribute::set_attributepriv<saga::task_base::Async>(
        std::string const& key, std::string const& val, saga::task_base::Async);
    template saga::task 
    attribute::set_attributepriv<saga::task_base::Task>(
        std::string const& key, std::string const& val, saga::task_base::Task);

    ///////////////////////////////////////////////////////////////////////////
    struct set_vector_attribute_priv
    {
        template <typename Derived>
        static saga::task
        call (Derived& this_, std::string const& key, 
            typename attribute::strvec_type const& val, bool sync)
        { 
            return this_.get_attr()->set_vector_attribute (key, val, sync);
        }
     };

    template <typename Tag>
    inline saga::task
    attribute::set_vector_attributepriv (
        std::string const& key, strvec_type const& val, Tag) 
    {
        return detail::dispatch_priv<Tag>::
            template call<set_vector_attribute_priv>(derived(), key, val);
    }

    void attribute::set_vector_attribute_sync(std::string const& key, 
        strvec_type const& val)
    {
        derived().get_attr()->set_vector_attribute_sync(key, val);
    }

    template saga::task 
    attribute::set_vector_attributepriv<saga::task_base::Sync>(
        std::string const& key, strvec_type const& val, saga::task_base::Sync);
    template saga::task 
    attribute::set_vector_attributepriv<saga::task_base::Async>(
        std::string const& key, strvec_type const& val, saga::task_base::Async);
    template saga::task 
    attribute::set_vector_attributepriv<saga::task_base::Task>(
        std::string const& key, strvec_type const& val, saga::task_base::Task);

    ///////////////////////////////////////////////////////////////////////////
    struct remove_attribute_priv
    {
        template <typename Derived>
        static saga::task
        call (Derived& this_, std::string const& key, bool sync)
        {
            if (!this_.attribute_exists(key))
            {
                // DoesNotExist
                typedef typename Derived::implementation_base_type base_type;
                SAGA_THROW_VERBATIM(this_.template get_target_object<base_type>(), 
                    "attribute '" + key + "' does not exist", 
                    saga::DoesNotExist);
            }
            return this_.get_attr()->remove_attribute(key, sync);
        }
    };

    template <typename Tag>
    inline saga::task
    attribute::remove_attributepriv (std::string const& key, Tag) 
    {
        return detail::dispatch_priv<Tag>::
            template call<remove_attribute_priv>(derived(), key);
    }

    void attribute::remove_attribute_sync(std::string const& key)
    {
        derived().get_attr()->remove_attribute_sync(key);
    }

    template saga::task 
    attribute::remove_attributepriv<saga::task_base::Sync>(
        std::string const& key, saga::task_base::Sync);
    template saga::task 
    attribute::remove_attributepriv<saga::task_base::Async>(
        std::string const& key, saga::task_base::Async);
    template saga::task 
    attribute::remove_attributepriv<saga::task_base::Task>(
        std::string const& key, saga::task_base::Task);

}   // namespace adaptors

///////////////////////////////////////////////////////////////////////////
// implement the attribute functions (we need to explicitly specialize 
// the template because the functions are not implemented inline)
template struct SAGA_EXPORT_REPEAT saga::detail::attribute <adaptors::attribute>;

template struct SAGA_EXPORT saga::detail::attribute_priv<adaptors::attribute, saga::task_base::Sync>;
template struct SAGA_EXPORT saga::detail::attribute_priv<adaptors::attribute, saga::task_base::Async>;
template struct SAGA_EXPORT saga::detail::attribute_priv<adaptors::attribute, saga::task_base::Task>;

template struct SAGA_EXPORT saga::detail::attribute_sync<adaptors::attribute>;

///////////////////////////////////////////////////////////////////////////////
}  // namespace saga

