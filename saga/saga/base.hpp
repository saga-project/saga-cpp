//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_BASE_HPP
#define SAGA_BASE_HPP

#include <saga/saga-defs.hpp>

#include <saga/saga/types.hpp>
#include <saga/saga/impl_base.hpp>

/// @cond
/** These methods are not within API scope */

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  namespace impl 
  { 
    namespace v1_0 
    {
      // FIXME: why these and no others? -- AM
      class cpi;
      class namespace_entry_cpi;
    }
    class attribute_interface;
  }
  
  // utility classes
  class url;
  class uuid;
  class const_buffer;
  class mutable_buffer;

  // saga core package
  class object;
  class session;
  class context;
  class exception;
  class attribute;
  class permissions;

  class task;
  class task_container;

  class adaptor;
  namespace adaptors 
  {
    class attribute;
    class exception;
    struct nocheck {};
  }
  namespace detail
  {
    template <typename Derived> struct attribute;
  }
  
  // saga data package
  // name spaces
  namespace name_space {
    class entry;
    class directory;
  }

  namespace advert {
    class entry;
    class directory;
  }

  namespace filesystem {
    class directory;
    class file;
    class const_iovec;
    class iovec;
  }

  // logical file
  namespace replica {
    class logical_directory;
    class logical_file;
  }
  
  // saga remote procedure call package
  namespace rpc {
    class rpc;
    class parameter;
  }
  
  // saga stream package
  namespace stream {
    class stream;
    class server;
  }

  // monitoring - not in spec
  class metric;

  // saga resource/jobs package 
  namespace job {
    class job;
    class self;
    class service;
    class description;
    class istream;
    class ostream;
  }
  
  // saga service discovery package
  namespace sd {
    class service_data;
    class service_description;
    class discoverer;
  }

  // saga checkpoint and recovery package
  namespace cpr {
    class job;
    class service;
    class checkpoint;
    class directory;
    class description;
  }

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

/// @endcond

#endif // SAGA_BASE_HPP

