//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_METRIC_HPP
#define SAGA_METRIC_HPP

// include stl
#include <map>
#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/monitorable.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/context.hpp>
#include <saga/saga/detail/attribute.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace attributes 
  {
    ///////////////////////////////////////////////////////////////////////////
    //  attribute names for metric
    char const* const metric_name         = "Name";
    char const* const metric_description  = "Description";
    char const* const metric_mode         = "Mode";
    char const* const metric_unit         = "Unit";
    char const* const metric_type         = "Type";
    char const* const metric_value        = "Value";

    ///////////////////////////////////////////////////////////////////////////
    //  attribute values for metric's type attribute
    char const* const metric_type_string  = "String";
    char const* const metric_type_int     = "Int";
    char const* const metric_type_enum    = "Enum";
    char const* const metric_type_float   = "Float";
    char const* const metric_type_bool    = "Bool";
    char const* const metric_type_time    = "Time";
    char const* const metric_type_trigger = "Trigger";

    ///////////////////////////////////////////////////////////////////////////
    //  attribute values for metric's mode attribute
    char const* const metric_mode_readonly   = "ReadOnly";
    char const* const metric_mode_readwrite  = "ReadWrite";
    char const* const metric_mode_final      = "Final";

  }

  ///@cond
  /////////////////////////////////////////////////////////////////////////////
  namespace metrics
  {
      /////////////////////////////////////////////////////////////////////////
      //  initialization data for list of metrics
      struct init_data
      {
          char const* const name;
          char const* const description;
          char const* const mode;
          char const* const unit;
          char const* const type;
          char const* const value;
      };
  }      
  ///@endcond
    
  /*! \brief Brief %description starts here     
   *
   *
   */
  class SAGA_EXPORT metric 
    : public saga::object,
      public saga::detail::attribute<metric>
  {
   protected:
     /// @cond
     TR1::shared_ptr <saga::impl::metric> get_impl_sp (void) const;
     saga::impl::metric* get_impl (void) const;

     friend struct saga::detail::attribute<metric>;  // needs to access get_impl()
     friend class saga::impl::metric;

     explicit metric(saga::impl::metric *impl);
     /// @endcond

   public:
     /*! \brief Brief %description starts here     
      *
      *
      */
     enum frequency 
     {
       Unknown  = -1,
       Cont     =  0,  // always available
       Descreet =  1,  // available at specific intervals
       Pull     =  2,  // available when asked for
       Push     =  3,  // available when set
       Event    =  4   // available when a specific event occurs
     };

     /*! \brief Brief %description starts here     
      *
      *
      */
     typedef saga::monitorable::cookie_handle metric_cookie;

     /*! \brief Brief %description starts here     
      *
      *
      */
     metric (void);
     
     /*! \brief Brief %description starts here     
      *
      *
      */
     explicit metric (saga::object const& o);

     /*! \brief Brief %description starts here     
      *
      *
      */
     // FIXME: the spec does not say anything about a target object parameter.
     // In fact, a metric should be usable on multiple objects.  For example, an
     // application may want to add the same metrics to several steerable
     // job instances.
     metric (saga::object target, 
             std::string name, std::string desc, std::string mode, 
             std::string unit, std::string type, std::string val);
             
     /*! \brief Brief %description starts here     
      *
      *
      */
    ~metric (void);

     /*! \brief Brief %description starts here     
      *
      *
      */
     metric &operator= (saga::object const& o);

     /*! \brief Brief %description starts here     
      *
      *
      */
     void fire (saga::context ctx = saga::context());

     /*! \brief Brief %description starts here     
      *
      *
      */
     metric_cookie add_callback (saga::callback f);
     
     /*! \brief Brief %description starts here     
      *
      *
      */
     void remove_callback (metric_cookie cookie);
     
     /*! \brief Brief %description starts here     
      *
      *
      */
     friend SAGA_EXPORT 
     bool operator== (metric const & lhs, metric const & rhs);

  };
   /// @cond
   /** These methods are not within API scope */
  typedef std::vector <saga::metric> metric_list_type;
  /// @endcond

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_METRIC_HPP


