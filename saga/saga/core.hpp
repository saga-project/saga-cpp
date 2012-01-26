//  Copyright (c) 2005-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_UTILITIES_HPP
#define SAGA_UTILITIES_HPP

#include <saga/saga/url.hpp>
#include <saga/saga/uuid.hpp>
#include <saga/saga/buffer.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/context.hpp>
#include <saga/saga/metric.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/task_container.hpp>
#include <saga/saga/attribute.hpp>
#include <saga/saga/permissions.hpp>

#if !defined(SAGA_ENGINE_EXPORTS) || defined(SAGA_USE_AUTO_LINKING)
# define SAGA_AUTOLINK_LIB_NAME "saga_core"
# include <saga/saga/autolink.hpp>
#endif

#endif

