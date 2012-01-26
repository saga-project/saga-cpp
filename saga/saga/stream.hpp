//  Copyright (c) 2005-2006 Hartmut Kaiser
//  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_STREAM_HPP
#define SAGA_STREAM_HPP

// stream comes from the comm package
#include <saga/saga/packages/stream/version.hpp>
#include <saga/saga/packages/stream/stream.hpp>
#include <saga/saga/packages/stream/stream_server.hpp>
#include <saga/saga/packages/stream/adaptors/stream.hpp>

// auto linking is disabled while compiling the stream package
#if !defined(SAGA_STREAM_PACKAGE_EXPORTS) || defined(SAGA_USE_AUTO_LINKING)
#define SAGA_AUTOLINK_LIB_NAME "stream"
#include <saga/saga/autolink.hpp>
#endif

#endif //  SAGA_STREAM_HPP

