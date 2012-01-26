//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// include spec sections
#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp> // boost::assign::insert()
#include <boost/assign/std/vector.hpp>

#include <saga/saga/metric.hpp>
#include <saga/impl/engine/metric.hpp>
#include <saga/impl/engine/context_base.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  // delegate all action to the implementation
  metric::metric (void)
    : saga::object (new saga::impl::metric ())
  {
  }

  metric::metric (saga::impl::metric *impl)
    : saga::object (impl)
  {
  }

  metric::metric (saga::object target, 
          std::string name, std::string desc, std::string mode, 
          std::string unit, std::string type, std::string val)
    : saga::object (new saga::impl::metric(target))
  {
    if (mode != attributes::metric_mode_readonly  &&
        mode != attributes::metric_mode_readwrite &&
        mode != attributes::metric_mode_final)
    {
      SAGA_THROW("metric::metric: bad 'mode' parameter", BadParameter);
    }

    // initialize attribute maps
    std::map <std::string, std::string> attributes_scalar_ro;
    std::map <std::string, std::string> attributes_scalar_rw;
    std::vector<std::string> valid_keys;

    // value is read only depending on given mode
    using namespace boost::assign;

    valid_keys += 
        attributes::metric_name,
        attributes::metric_description,
        attributes::metric_mode,
        attributes::metric_unit,
        attributes::metric_type,
        attributes::metric_value
      ;
    this->init_keynames(valid_keys);

    insert(attributes_scalar_ro)
      (attributes::metric_name,        name)
      (attributes::metric_description, desc)
      (attributes::metric_mode,        mode)
      (attributes::metric_unit,        unit)
      (attributes::metric_type,        type);
      
    if (mode != attributes::metric_mode_readwrite)
    {
      insert(attributes_scalar_ro) (attributes::metric_value, val);
    }
    else
    {
      insert(attributes_scalar_rw) (attributes::metric_value, val);
    }

    // initialize attribute implementation
    this->init (attributes_scalar_ro, attributes_scalar_rw);
    this->init (false, true);   // metrics have a cache only attribute implementation
  }

  metric::~metric (void)
  {
  }   

  /////////////////////////////////////////////////////////////////////////////
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

  /////////////////////////////////////////////////////////////////////////////
  saga::impl::metric* metric::get_impl (void) const
  {
    typedef saga::object base_type;
    return static_cast<saga::impl::metric*>(this->base_type::get_impl());
  }

  TR1::shared_ptr <saga::impl::metric> metric::get_impl_sp (void) const
  {
    typedef saga::object base_type;
    return TR1::static_pointer_cast<saga::impl::metric>(
        this->base_type::get_impl_sp());
  }

  /// @cond 
  /** Doxygen generates namespace function from friend impls... */
  bool operator== (metric const & lhs,
                   metric const & rhs)
  {
    return (lhs.get_impl() == rhs.get_impl()); 
  }
  /// @endcond

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
    std::string mode (get_attribute(saga::attributes::metric_mode));
    if (mode == saga::attributes::metric_mode_readonly)
    {
      std::string name(get_attribute(saga::attributes::metric_name));
      SAGA_THROW("Metric: '" + name + "' is readonly.", saga::PermissionDenied);
    }
    get_impl()->fire (ctx);
  }

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the attribute functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_EXPORT_REPEAT attribute<metric>;

    template struct SAGA_EXPORT attribute_priv<metric, task_base::Sync>;
    template struct SAGA_EXPORT attribute_priv<metric, task_base::Async>;
    template struct SAGA_EXPORT attribute_priv<metric, task_base::Task>;

    template struct SAGA_EXPORT attribute_sync<metric>;
  }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

