//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#ifdef  ADAPTORS_###SUITE###_###TYPE###_OSTREAM_HPP
#define ADAPTORS_###SUITE###_###TYPE###_OSTREAM_HPP


// saga includes
#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

// saga engine includes
#include <saga/impl/job.hpp>

// adaptor includes
#include "###suite###_###type###_stream.hpp"


////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{
  class ostream : public saga::job::ostream
  {
    private:
      typedef ###suite###_###type###_stream <saga::adaptors::ostream_ptr> 
              impl_type;

    public:
      template <typename Stream>
      ostream (saga::impl::v1_0::job_cpi * cpi, 
               Stream                    & child_ostream)
        : saga::job::ostream (new impl_type (cpi, child_ostream.rdbuf ()))
      {
      }
  };

} // namespace ###suite###_###type###
////////////////////////////////////////////////////////////////////////

#endif // ADAPTORS_###SUITE###_###TYPE###_OSTREAM_HPP

