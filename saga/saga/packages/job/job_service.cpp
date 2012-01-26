#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/service.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

// include service api and implementation
#include <saga/saga/job.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/impl/job.hpp>
#include <saga/saga/detail/call.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/job/preprocessed/service.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/service.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace job {

  service::service (session const &s, saga::url rm)
    : saga::object (new saga::impl::job_service (s, rm))
  {
      this->saga::object::get_impl()->init();
  }

  service::service (saga::url rm)
    : saga::object (new saga::impl::job_service(saga::detail::get_the_session(), rm))
  {
      this->saga::object::get_impl()->init();
  }

  service::service (saga::object const& o)
    : saga::object (o)
  {
      if (this->get_type() != saga::object::JobService)
      {
          SAGA_THROW("Bad type conversion.", saga::BadParameter);
      }
  }

  service::service (saga::impl::job_service *impl)
    : saga::object (impl)
  {
  }

  service::service (int)
  {
  }

  service::~service (void)
  {
  }

  service &service::operator= (saga::object const& o)
  {
      if (this != &o) 
          this->saga::object::operator=(o);
      return *this;
  }

  saga::impl::job_service* service::get_impl() const
  { 
      typedef saga::object base_type;
      return static_cast<saga::impl::job_service*>(this->base_type::get_impl()); 
  }

  TR1::shared_ptr <saga::impl::job_service> service::get_impl_sp(void) const
  { 
      // FIXME: this needs documentation
      typedef saga::object base_type;
      return TR1::static_pointer_cast <saga::impl::job_service> (
          this->base_type::get_impl_sp());
  }

  // factory
  SAGA_CALL_CREATE_IMP_2(service, impl::job_service, session const&, saga::url)

  // methods of the service
  SAGA_CALL_IMP_1    (service, create_job, description)
  SAGA_CALL_CONST_IMP_0(service, get_url)
  SAGA_CALL_IMP_5    (service, run_job, std::string, std::string,
                      saga::job::ostream&, saga::job::istream&, saga::job::istream&)
  SAGA_CALL_IMP_2_EX (service, run_job, run_job_noio, std::string, std::string)
  SAGA_CALL_IMP_0    (service, list)
  SAGA_CALL_IMP_1    (service, get_job,  std::string)
  SAGA_CALL_IMP_0    (service, get_self)
}

} // namespace saga
///////////////////////////////////////////////////////////////////////////////////

#endif

