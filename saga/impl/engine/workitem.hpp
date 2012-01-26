//  Copyright (c) 2005-2010 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_WORKITEM_HPP
#define SAGA_IMPL_ENGINE_WORKITEM_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/register_workitem.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace saga { namespace impl
{
    ///////////////////////////////////////////////////////////////////////////
    class workitem
    {
    public:
        workitem(boost::asio::io_service& io_service,
                adaptors::workitem_function f, 
                adaptors::duration_type const& expire_from_now,
                TR1::function<void()> on_invoke)
          : f_(f), on_invoke_(on_invoke), timer_(io_service, expire_from_now),
            invoked_(false), valid_(false)
        {
            timer_.async_wait(TR1::bind(&workitem::invoke, this, 
                TR1::placeholders::_1));
            valid_ = true;
        }

        workitem(boost::asio::io_service& io_service,
                adaptors::workitem_function f, 
                adaptors::time_type const& expire_at,
                TR1::function<void()> on_invoke)
          : f_(f), on_invoke_(on_invoke), timer_(io_service, expire_at),
            invoked_(false), valid_(false)
        {
            timer_.async_wait(TR1::bind(&workitem::invoke, this, 
                TR1::placeholders::_1));
            valid_ = true;
        }

        ~workitem()
        {
            boost::system::error_code ec;
            timer_.cancel(ec);      // don't throw
        }

        void invoke(boost::system::error_code const& e) 
        {
            invoked_ = true;
            if (valid_) 
                f_(e);
            on_invoke_();
        }

        void invalidate()
        {
            valid_ = false;

            boost::system::error_code ec;
            timer_.cancel(ec);      // don't throw
            timer_.wait(ec);
        }

    private:
        adaptors::workitem_function f_;
        TR1::function<void()> on_invoke_;
        boost::asio::deadline_timer timer_;
        bool invoked_;
        bool valid_;
    };
}}

#endif
