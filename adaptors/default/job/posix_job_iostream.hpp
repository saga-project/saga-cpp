//  Copyright (c) 2007 Ole Christian Weidner (oweidner@cct.lsu.edu)
//  Copyright (c) 2007 Andre Merzky          (andre@merzky.net)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga-defs.hpp>

#ifndef SAGA_WINDOWS

#ifndef SAGA_POSIX_JOB_IOSTREAM_HPP
#define SAGA_POSIX_JOB_IOSTREAM_HPP

// C++ includes
#include <iostream>
#include <streambuf>
#include <iosfwd>

// Boost includes
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>

// SAGA includes
#include <saga/impl/packages/job/istream.hpp>
#include <saga/impl/packages/job/ostream.hpp>
#include <saga/saga.hpp>

////////////////////////////////////////////////////////////////////////
namespace impl
{
  template <class Device, class Base> struct posix_job_adaptor_stream;

  typedef posix_job_adaptor_stream<
      boost::iostreams::file_descriptor_sink,
      saga::adaptors::ostream_ptr
    > posix_job_adaptor_ostream;

  typedef posix_job_adaptor_stream<
      boost::iostreams::file_descriptor_source,
      saga::adaptors::istream_ptr
    > posix_job_adaptor_istream;

  template <class Device, class Base>
  struct posix_job_adaptor_stream : Base
  {
      posix_job_adaptor_stream (saga::impl::v1_0::job_cpi * cpi, int fd)
        : Base        (&buf_, fd)
        #if SAGA_BOOST_IOSTREAMS_VERSION < 104400
        , buf_        (fd, false)  // NO auto close on destruct
        #else
        , buf_        (fd, boost::iostreams::never_close_handle)  // NO auto close on destruct
        #endif
        , job_proxy_  (cpi->get_proxy ()->shared_from_this ())
        , job_cpi_    (cpi->shared_from_this ())
      {
      }

      ~posix_job_adaptor_stream ()
      {
        boost::iostreams::flush (buf_);
      }

      std::streambuf * get_streambuf (void)
      {
        return &buf_;
      }

    private:
      boost::iostreams::stream_buffer <Device> buf_;

      // Keep job proxy and cpi instance alive, for file descriptor in buf_
      TR1::shared_ptr <saga::impl::proxy>     job_proxy_;
      TR1::shared_ptr <saga::impl::v1_0::cpi> job_cpi_;
  };

}   // namespace impl
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////

template <class StreamImpl, class Base> struct posix_job_adaptor_stream;

typedef posix_job_adaptor_stream<
    impl::posix_job_adaptor_istream,
    saga::job::istream
  > posix_job_adaptor_istream;

typedef posix_job_adaptor_stream<
    impl::posix_job_adaptor_ostream,
    saga::job::ostream
  > posix_job_adaptor_ostream;

template <class StreamImpl, class Base>
struct posix_job_adaptor_stream : Base
{
    posix_job_adaptor_stream (saga::impl::v1_0::job_cpi * cpi, int fd)
      : Base (new StreamImpl (cpi, fd))
    {
    }
};

#endif // SAGA_POSIX_JOB_IOSTREAM_HPP

#endif // SAGA_WINDOWS

