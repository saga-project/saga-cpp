//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// include spec sections
#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp> // boost::assign::insert()
#include <boost/assign/std/vector.hpp>

#include <saga/saga/adaptors/metric.hpp>
#include <saga/impl/engine/metric.hpp>
#include <saga/impl/engine/context_base.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors
{
    // delegate all action to the implementation
    metric::metric()
      : saga::object (new saga::impl::metric ())
    {
    }
  
    metric::~metric ()
    {
    }   
  
    ///////////////////////////////////////////////////////////////////////////
    metric::metric (saga::object const& o)
      : saga::object (o)
    {
        if (this->get_type() != saga::object::Metric)
        {
            SAGA_THROW("Bad type conversion.", saga::BadParameter);
        }
    }

    metric &metric::operator= (saga::object const& o)
    {
        saga::object::operator=(o);
        if (this->get_type() != saga::object::Metric)
        {
            SAGA_THROW("Bad type conversion.", saga::BadParameter);
        }
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    metric::metric (saga::metric const& m)
      : saga::object (m)
    {
    }

    metric &metric::operator= (saga::metric const& m)
    {
        saga::object::operator=(m);
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    saga::impl::metric* metric::get_impl (void) const
    {
      typedef saga::object base_type;
      return static_cast<saga::impl::metric*>(this->base_type::get_impl());
    }

    TR1::shared_ptr<saga::impl::metric> metric::get_impl_sp (void) const
    {
      typedef saga::object base_type;
      return TR1::static_pointer_cast<saga::impl::metric>(
          this->base_type::get_impl_sp());
    }

    saga::impl::attribute_interface* metric::get_attr() const
    { 
        return get_impl()->get_attributes(); 
    }

    ///////////////////////////////////////////////////////////////////////////
    bool operator== (metric const& lhs, metric const& rhs)
    {
        return (lhs.get_impl() == rhs.get_impl()); 
    }

    metric::metric_cookie 
    metric::add_callback(saga::callback f)
    {
        return get_impl()->add_callback(f);
    }
  
    void metric::remove_callback(metric_cookie cookie)
    {
        get_impl()->remove_callback(cookie);
    }
  
    void metric::fire (saga::context ctx)
    {
        get_impl()->fire (ctx);
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
    metric::set_attributepriv (std::string const& key, 
        std::string const& val, Tag) 
    {
        return detail::dispatch_priv<Tag>::
            template call<set_attribute_priv>(derived(), key, val);
    }

    template saga::task 
    metric::set_attributepriv<saga::task_base::Sync>(
        std::string const& key, std::string const& val, saga::task_base::Sync);
    template saga::task 
    metric::set_attributepriv<saga::task_base::Async>(
        std::string const& key, std::string const& val, saga::task_base::Async);
    template saga::task 
    metric::set_attributepriv<saga::task_base::Task>(
        std::string const& key, std::string const& val, saga::task_base::Task);
    
    ///////////////////////////////////////////////////////////////////////////
    struct set_vector_attribute_priv
    {
        template <typename Derived>
        static saga::task
        call (Derived& this_, std::string const& key, 
            typename metric::strvec_type const& val, bool sync)
        { 
            return this_.get_attr()->set_vector_attribute (key, val, sync);
        }
     };
     
    template <typename Tag>
    inline saga::task
    metric::set_vector_attributepriv (
        std::string const& key, strvec_type const& val, Tag) 
    {
        return detail::dispatch_priv<Tag>::
            template call<set_vector_attribute_priv>(derived(), key, val);
    }

    template saga::task 
    metric::set_vector_attributepriv<saga::task_base::Sync>(
        std::string const& key, strvec_type const& val, saga::task_base::Sync);
    template saga::task 
    metric::set_vector_attributepriv<saga::task_base::Async>( 
        std::string const& key, strvec_type const& val, saga::task_base::Async);
    template saga::task 
    metric::set_vector_attributepriv<saga::task_base::Task>(
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
    metric::remove_attributepriv (std::string const& key, Tag) 
    {
        return detail::dispatch_priv<Tag>::
            template call<remove_attribute_priv>(derived(), key);
    }

    template saga::task 
    metric::remove_attributepriv<saga::task_base::Sync>(
        std::string const& key, saga::task_base::Sync);
    template saga::task 
    metric::remove_attributepriv<saga::task_base::Async>(
        std::string const& key, saga::task_base::Async);
    template saga::task 
    metric::remove_attributepriv<saga::task_base::Task>(
        std::string const& key, saga::task_base::Task);

}   // namespace adaptors

///////////////////////////////////////////////////////////////////////////////
//  implement the attribute functions (we need to explicitly specialize 
//  the template because the functions are not implemented inline)
template struct SAGA_EXPORT_REPEAT saga::detail::attribute<adaptors::metric>;

template struct SAGA_EXPORT saga::detail::attribute_priv<adaptors::metric, saga::task_base::Sync>;
template struct SAGA_EXPORT saga::detail::attribute_priv<adaptors::metric, saga::task_base::Async>;
template struct SAGA_EXPORT saga::detail::attribute_priv<adaptors::metric, saga::task_base::Task>;

template struct SAGA_EXPORT saga::detail::attribute_sync<adaptors::metric>;

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

