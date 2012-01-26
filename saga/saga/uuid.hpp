//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_UUID_HPP
#define SAGA_SAGA_UUID_HPP

// include stl
#include <string>
#include <iosfwd>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { 
  /*! \brief Brief %description starts here     
   *
   *
   */
  class SAGA_EXPORT uuid 
  {
    // FIXME-AM: how can I otherwise implement the operators in the cpp file?
    private:
      /// @cond
      /** These methods are not within API scope */
      TR1::shared_ptr <saga::impl::uuid> impl_;
     /// @endcond

    protected:
      /// @cond
      /** These methods are not within API scope */
      void create (void);
      void create (char const * str);
     /// @endcond

    public:
     /*! \brief Brief %description starts here     
      *
      *
      */
      uuid (void);
      
     /*! \brief Brief %description starts here     
      *
      *
      */
      uuid (char const * uuid_str);
      
     /*! \brief Brief %description starts here     
      *
      *
      */
      std::string string (void) const;

     /*! \brief Brief %description starts here     
      *
      *
      */
      friend SAGA_EXPORT bool operator== (const uuid & lhs, const uuid & rhs);
      
     /*! \brief Brief %description starts here     
      *
      *
      */
      friend SAGA_EXPORT bool operator!= (const uuid & lhs, const uuid & rhs);
      
     /*! \brief Brief %description starts here     
      *
      *
      */
      friend SAGA_EXPORT bool operator<  (const uuid & lhs, const uuid & rhs);

     /*! \brief Brief %description starts here     
      *
      *
      */
      friend SAGA_EXPORT bool operator>  (const uuid & lhs, const uuid & rhs);

     /*! \brief Brief %description starts here     
      *
      *
      */
      friend SAGA_EXPORT bool operator<= (const uuid & lhs, const uuid & rhs);
      
     /*! \brief Brief %description starts here     
      *
      *
      */
      friend SAGA_EXPORT bool operator>= (const uuid & lhs, const uuid & rhs);
     
     /*! \brief Brief %description starts here     
      *
      *
      */
      friend SAGA_EXPORT std::ostream & operator<< (std::ostream & ostrm, 
                                        const uuid   & rhs);

     /*! \brief Brief %description starts here     
      *
      *
      */
      friend SAGA_EXPORT std::istream & operator>> (std::istream & istrm, 
                                        uuid         & rhs);
  };

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_SAGA_UUID_HPP

