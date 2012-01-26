//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_METRIC_HPP
#define SAGA_ADAPTORS_METRIC_HPP

// include stl
#include <vector>
#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/metric.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    ///////////////////////////////////////////////////////////////////////////
    //  this is the same as the saga::metric class, except it is possible to 
    //  set read only metric values
    class SAGA_EXPORT metric 
      : public saga::object,
        public saga::detail::attribute<metric>
    {
    protected:
        saga::impl::metric* get_impl (void) const;
        TR1::shared_ptr <saga::impl::metric> get_impl_sp (void) const;

        friend struct saga::detail::attribute<metric>;  // needs to access get_impl()
        friend class saga::impl::metric;

        explicit metric(saga::impl::metric *impl);

    public:
        typedef saga::metric::frequency frequency;
        typedef saga::monitorable::cookie_handle metric_cookie;

        // ctor/dtor
        metric ();
        explicit metric (saga::object const& o);
        metric (saga::metric const& m);
        ~metric ();

        // default operator=() and copy ctor are ok
        metric &operator= (saga::object const& o);
        metric &operator= (saga::metric const& m);
      
        friend SAGA_EXPORT 
        bool operator== (metric const& lhs, metric const& rhs);

         // read-write metrics can update their values, read only metrics can't.
         // Updating means: setting new values via the attribute interface, and
         // calling 'fire' to push the current state to the implementation.
         void fire (saga::context ctx = saga::context());

         // on changes, a metric can be notified.  
         metric_cookie add_callback (saga::callback f);
         void remove_callback (metric_cookie cookie);

    public:
        // just defined for brevity
        typedef std::vector <std::string>              strvec_type;
        typedef std::map    <std::string, std::string> strmap_type;

    private:
        friend struct saga::detail::attribute<attribute>;  // needs to access get_impl()
      
        template <typename Tag> 
        saga::task set_attributepriv (std::string const&, std::string const&, Tag);
        template <typename Tag> 
        saga::task set_vector_attributepriv(std::string const&, strvec_type const&, Tag);
        template <typename Tag> 
        saga::task remove_attributepriv(std::string const&, Tag);

    public:
        saga::impl::attribute_interface* get_attr() const; 

        /*! \brief Brief %description starts here    
         *
         *
         */  
        void set_attribute(std::string key, std::string value) 
        {
            set_attributepriv(key, value, saga::task_base::Sync());
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

#endif // SAGA_ADAPTORS_METRIC_HPP

