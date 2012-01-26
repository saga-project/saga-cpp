//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_OBJECT_HPP
#define SAGA_SAGA_OBJECT_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <boost/mpl/bool.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  SAGA spec:
//
//  package saga.object
//  {
//    enum object_type 
//    {
//       Exception = 1,
//       URL = 2,
//       Buffer = 3,
//       Session = 4,
//       Context = 5,
//       Task = 6,
//       TaskContainer = 7,
//       Metric = 8,
//       NSEntry = 9,
//       NSDirectory = 10,
//       IOVec = 11,
//       File = 12,
//       Directory = 13,
//       LogicalFile = 14,
//       LogicalDirectory = 15,
//       JobDescription = 16,
//       JobService = 17,
//       Job = 18,
//       JobSelf = 19,
//       StreamServer = 20,
//       Stream = 21,
//       Parameter = 22,
//       RPC = 23,
//    }
// 
//    interface object 
//    {  
//      get_id (out string      id     );
//      get_type (out object_type type   );
//      get_session (out session     session);
//      clone (out object clone );
//    }
//  }

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  /*! \brief Brief %description starts here     
   *
   *
   */
  class SAGA_EXPORT object 
  {
    public:

      enum type 
      {
          Unknown          =  -1,
          Exception        =   1,
          URL              =   2,
          Buffer           =   3,
          Session          =   4,
          Context          =   5,
          Task             =   6,
          TaskContainer    =   7,
          Metric           =   8,
          NSEntry          =   9,
          NSDirectory      =  10,
          IOVec            =  11,
          File             =  12,
          Directory        =  13,
          LogicalFile      =  14,
          LogicalDirectory =  15,
          JobDescription   =  16,
          JobService       =  17,
          Job              =  18,
          JobSelf          =  19,
          StreamServer    =  20,
          Stream           =  21,
          Parameter        =  22,
          RPC              =  23,
        
      // SAGA Message API extension
          Msg              =  24,
          Endpoint         =  25,
        
      // SAGA Information services extension
          Advert           =  26,
          AdvertDirectory  =  27,

      // SAGA Service Discovery extension
          ServiceDescription = 28,
          ServiceDiscoverer  = 29,
          ServiceData        = 30,

      // SAGA Checkpoint and Recovery extension
          CPRJobDescription  = 31,
          CPRJobService      = 32,
          CPRJob             = 33,
          CPRJobSelf         = 34,
          CPRCheckpoint      = 35,
          CPRDirectory       = 36,

      // SAGA Information System Navigator extension
          EntityData         = 37,
          EntityDataSet      = 38,
          EntityNavigator    = 39
      };

    private:
      /// @cond
      /** These methods are not within API scope */
      TR1::shared_ptr <saga::impl::object> impl_;

      friend struct impl::runtime;
      friend class  attribute;
      friend class  adaptors::attribute;
      friend class  monitorable;
      friend class  permissions;
      /// @endcond

    public:
      typedef saga::impl::object implementation_base_type;

    protected:
      /// @cond
      /** These methods are not within API scope */
      TR1::shared_ptr<saga::impl::object> get_impl_sp (void) const
      { 
        return impl_;
      }
      saga::impl::object* get_impl (void) const
      { 
        return impl_.get();
      }
      bool is_impl_valid() const
      {
        return impl_ ? true : false;
      }

      explicit object (saga::impl::object  * init);
      explicit object (TR1::shared_ptr<saga::impl::object> init);
      /// @endcond

    public:
      /*! \brief Brief %description starts here     
       *
       *
       */
      object();

      /*! \brief Brief %description starts here     
       *
       *
       */
      saga::object::type  get_type     (void) const;
      
      /*! \brief Brief %description starts here     
       *
       *
       */
      saga::uuid          get_id       (void) const;

      /*! \brief Brief %description starts here     
       *
       *
       */
      saga::session& get_session();
      saga::session const& get_session() const;

      /*! \brief Brief %description starts here     
       *
       *
       */
      saga::object clone() const;

  }; // object

  /// @cond
  /** These methods are not within API scope TODO wrong namespace?!? */
  SAGA_EXPORT std::string get_object_type_name(saga::object::type t);
  /// @endcond

  /// @cond
  namespace detail
  {
    // meta function to tell whether T is a saga::object
    template <typename T>
    struct is_saga_object : TR1::is_base_of<saga::object, T> {};

    template <>
    struct is_saga_object<saga::object> : boost::mpl::true_ {};
  }
  /// @endcond

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#  pragma warning(pop)
#endif

#endif // SAGA_SAGA_OBJECT_HPP

