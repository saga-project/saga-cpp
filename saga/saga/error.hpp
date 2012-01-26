//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_ERROR_HPP)
#define SAGA_ERROR_HPP

///////////////////////////////////////////////////////////////////////////////
namespace saga {

  /*! \brief Brief %description starts here     
   *
   *
   */
  enum error {
    //  This list is sorted, with the most specific exceptions are listed first 
    //  and least specific last.
    //  The most specific exception possible (i.e., applicable) MUST be thrown 
    //  on all error conditions.
    NotImplemented        = 1,
    IncorrectURL          = 2,
    BadParameter          = 3,
    AlreadyExists         = 4,
    DoesNotExist          = 5,
    IncorrectState        = 6,
    PermissionDenied      = 7,
    AuthorizationFailed   = 8,
    AuthenticationFailed  = 9,
    Timeout               = 10,
    NoSuccess             = 11
  };

  /////////////////////////////////////////////////////////////////////////////
  namespace adaptors 
  {
      enum error {
        // these are not in the SAGA spec!
        Success           = 0,
        FirstAdaptorSpecificException = 12,
        AdaptorDeclined   = FirstAdaptorSpecificException,
        NoAdaptor         = 13,
        NoAdaptorInfo     = 14,
        Unexpected        = 15
      };
  }

  /// @cond
  /** These methods are not within API scope */
  char const* const error_names[] = {
    "Success",
    "NotImplemented",
    "IncorrectURL",
    "BadParameter",
    "AlreadyExists",
    "DoesNotExist",
    "IncorrectState",
    "PermissionDenied",
    "AuthorizationFailed",
    "AuthenticationFailed",
    "Timeout",
    "NoSuccess",
    // these are not in the SAGA spec! 
    // pretend to be NoSuccess
    "NoSuccess", // "AdaptorDeclined",
    "NoSuccess", // "NoAdaptor",
    "NoSuccess", // "NoAdaptorInfo",
    "NoSuccess" // "Unexpected"
  };
  /// @endcond
  
///////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif // SAGA_ERROR_HPP

