//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PACKAGES_CPR_CONFIG_HPP
#define SAGA_PACKAGES_CPR_CONFIG_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif 

#include <saga/saga-defs.hpp>

#if !defined (SAGA_CREATE_PREPROCESSED_FILES)

# ifdef   SAGA_CPR_PACKAGE_EXPORTS
#   define SAGA_CPR_PACKAGE_EXPORT SAGA_SYMBOL_EXPORT
#   define SAGA_CPR_PACKAGE_EXPORT_REPEAT SAGA_SYMBOL_EXPORT_REPEAT
#   define SAGA_CPR_PACKAGE_LOCAL  SAGA_SYMBOL_INTERNAL
# elif !defined(BUILD_SAGA_LITE)
#   define SAGA_CPR_PACKAGE_EXPORT SAGA_SYMBOL_IMPORT
#   define SAGA_CPR_PACKAGE_EXPORT_REPEAT   /**/
#   define SAGA_CPR_PACKAGE_LOCAL  SAGA_SYMBOL_INTERNAL
# else
#   define SAGA_CPR_PACKAGE_EXPORT /* empty */
#   define SAGA_CPR_PACKAGE_EXPORT_REPEAT   /**/
#   define SAGA_CPR_PACKAGE_LOCAL  /* empty */
# endif

#endif

#endif // SAGA_PACKAGES_CPR_CONFIG_HPP


