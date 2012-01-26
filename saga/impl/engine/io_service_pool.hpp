//  Copyright (c) 2005-2010 Hartmut Kaiser
//
//  Parts of this code were taken from the Boost.Asio library
//  Copyright (c) 2003-2007 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_IMPL_ENGINE_IO_SERVICE_POOL_HPP)
#define SAGA_IMPL_ENGINE_IO_SERVICE_POOL_HPP

#include <vector>

#include <saga/saga/util.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    /// A pool of io_service objects.
    class io_service_pool : private boost::noncopyable
    {
    public:
        /// \brief Construct the io_service pool.
        /// \param pool_size
        ///                 [in] The number of threads to run to serve incoming
        ///                 requests
        /// \param start_thread
        ///                 [in] 
        explicit io_service_pool(std::size_t pool_size = 1);

        /// \brief Run all io_service objects in the pool. If join_threads is true
        ///        this will also wait for all threads to complete
        bool run(bool join_threads = false);

        /// \brief Stop all io_service objects in the pool.
        void stop();

        /// \brief Join all io_service threads in the pool.
        void join();

        /// \brief Clear all internal data structures
        void clear();

        /// \brief 
        bool is_running() const { return !threads_.empty(); }

        /// \brief Get an io_service to use.
        boost::asio::io_service& get_io_service();

    protected:
        ///
        void thread_run(int index);

    private:
        typedef TR1::shared_ptr<boost::asio::io_service> io_service_ptr;
        typedef TR1::shared_ptr<boost::asio::io_service::work> work_ptr;

        /// The pool of io_services.
        std::vector<io_service_ptr> io_services_;
        std::vector<TR1::shared_ptr<boost::thread> > threads_;

        /// The work that keeps the io_services running.
        std::vector<work_ptr> work_;

        /// The next io_service to use for a connection.
        std::size_t next_io_service_;

        /// set to true if stopped
        bool stopped_;

        /// initial number of OS threads to execute in this pool
        std::size_t pool_size_;
    };
}}

#endif 
