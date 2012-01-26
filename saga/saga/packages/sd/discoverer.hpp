//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_PACKAGES_SD_DISCOVERER_HPP

#define SAGA_PACKAGES_SD_DISCOVERER_HPP

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp> 
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>

#include <saga/saga/packages/sd/config.hpp>
#include <saga/saga/packages/sd/service_description.hpp>

namespace saga
{
  namespace sd {

/**
 * <p>
 * Provides the entry point for service discovery. Apart from the constructor
 * and destructor it has one method: <code>list_services</code> which returns
 * the list of descriptions of services matching the specified filter strings.
 * </p>
 * <p>
 * An implementation SHOULD return the results in a random order if there is
 * more than one result to avoid any tendency to overload particular services
 * while leaving others idle.
 * </p>
 * <p>
 * There are three filter strings: <code>svc_filter</code>,
 * <code>authz_filter</code> and <code>data_filter</code> which act together
 * to restrict the set of services returned. Each of the filter strings uses
 * SQL92 syntax as if it were part of a <code>WHERE</code> clause acting to
 * select from a single table that includes columns as described below for that
 * filter type. SQL92 has been chosen because it is widely known and has the
 * desired expressive power. Multi-valued attributes are treated as a set of
 * values.
 * </p>
 * <p>
 * Three strings are used, rather than one, as this clarifies the description of
 * the functionality, avoids problems with key values being themselves existing
 * GLUE attributes, and facilitates implementation as it makes it impossible to
 * specify constraints that correlate, for example, service and authz
 * information. Only the following operators are permitted in expressions not
 * involving multi-valued attributes: <code>IN</code>, <code>LIKE</code>,
 * <code>AND</code>, <code>OR</code>, <code>NOT</code>, <code>=</code>,
 * <code>&gt;=</code>, <code>&gt;</code>, <code>&lt;=</code>,
 * <code>&lt;</code>, <code>&gt;&lt;</code> in addition to column names,
 * parentheses, column values as single quoted strings, numeric values and the
 * comma. For a multi-valued attribute, the name of the attribute MUST have the
 * keyword <code>ALL</code> or <code>ANY</code> immediately before it,
 * unless comparison with a set literal is intended. For each part of the
 * expression, the attribute name MUST precede the literal value. An
 * implementation SHOULD try to give an informative error message if the filter
 * string does not conform. It is, however, sufficient to report in which filter
 * string the syntax error was found.
 * </p>
 * <dl>
 * <dt>The <code>LIKE</code> operator matches string patterns:</dt>
 * <dd><code>'%xyz'</code> matches all entries with trailing xyz</dd>
 * <dd><code>'xyz%'</code> matches all entries with leading xyz </dd>
 * <dd><code>'%xyz%'</code> matches all entries with xyz being a substring</dd>
 * </dl>
 * <p>
 * The <code>ESCAPE</code> keyword can be used with <code>LIKE</code> in the
 * normal way.
 * </p>
 * <p>
 * Column names are not case sensitive but values are.
 * </p>
 * <p>
 * No use-case has been identified for the operators <code>&g./saga/saga/call.hppt=</code>,
 * <code>&gt;</code>, <code>&lt;=</code>, <code>&gt;</code> to be
 * applied to strings. An Implementation wishing to support these comparison
 * operators on strings MUST select a collation sequence. Alternatively, an
 * implementation CAN treat all string comparisons as true, or reject them as
 * invalid SQL.
 * </p>
 * <h2>Service Filter</h2>
 * <p>
 * Column names in the <code>serviceFilter</code> are:
 * </p>
 * <dl>
 * <dt><code>type</code></dt>
 * <dd>type of service. This API does not restrict values of the service type --
 * it might be a DNS name, a URN or any other string.</dd>
 * <dt><code>name</code></dt>
 * <dd>name of service (not necessarily unique)</dd>
 * <dt><code>uid</code></dt>
 * <dd>unique identifier of service</dd>
 * <dt><code>site</code></dt>
 * <dd>name of site the service is running at</dd>
 * <dt><code>url</code></dt>
 * <dd>the endpoint to contact the service - will normally be used with the
 * LIKE operator</dd>
 * <dt><code>implementor</code></dt>
 * <dd>name of the organisation providing the implementation of the service</dd>
 * <dt><code>relatedService</code></dt>
 * <dd>the uid of a service related to the one being looked for</dd>
 * </dl>
 * <dl>
 * <dt>Some examples are:</dt>
 * <dd><code>type = 'org.ogf.saga.service.job'</code></dd>
 * <dd><code>site IN ('INFN-CNAF', 'RAL-LCG2')</code></dd>
 * <dd><code>type = 'org.glite.ResourceBroker' AND Site LIKE '%.uk' AND implementor = 'EGEE'</code></dd>
 * <dd><code>ANY relatedService = 'someServiceUID'</code></dd>
 * </dl>
 * <p>
 * Note the use of the <code>ANY</code> keyword in the last example as
 * <code>relatedService</code> is multi-valued.
 * </p>
 * <h2>Data Filter</h2>
 * <p>
 * Column names in the the <code>data_filter</code> string are matched against
 * the service data key/value pairs. No keys are predefined by this
 * specification.
 * </p>
 * <p>
 * If values are specified as numeric values and not in single quotes, the
 * service data will be converted from string to numeric for comparison.
 * </p>
 * <p>
 * Data attributes may be multi-valued. If a <code>data_filter</code> string
 * does not have the correct syntax to accept multi-valued attributes, and a
 * service has more than one value for an attribute mentioned in the filter,
 * that service MUST be rejected.
 * </p>
 * <dl>
 * <dt>Some examples are:</dt>
 * <dd><code>source = 'RAL-LCG2' OR destination = 'RAL-LCG2'</code></dd>
 * <dd><code>RunningJobs >= 1 AND RunningJobs <= 5</code></dd>
 * </dl>
 * <h2>Authz Filter</h2>
 * <p>
 * The set of column names in the <code>authz_filter</code> is not defined.
 * Instead the list below shows a possible set of names and how they might be
 * interpreted. Each of these column names could reasonably be related to an
 * authorization decision. Implementations MAY reuse the attribute names defined
 * for the saga::context class.
 * </p>
 * <dl>
 * <dt>vo</dt>
 * <dd>virtual organization - will often be used with the IN operator</dd>
 * <dt>dn</dt>
 * <dd>an X.509 ``distinguished name''</dd>
 * <dt>group</dt>
 * <dd>a grouping of people within a Virtual Organization</dd>
 * <dt>role</dt>
 * <dd>values might include ``Administrator'' or ``ProductionManager''</dd>
 * </dl>
 * <p>
 * It is expected that many of the attributes used in then
 * <code>authzFilter</code> will be multi-valued.
 * </p>
 * <dl>
 * <dt>Some examples, where <code>VO</code> is assmed to be multi-valued are:</dt>
 * <dd><code>ANY VO IN ('cms', 'atlas')</code></dd>
 * <dd><code>VO = ('dteam')</code></dd>
 * </dl>
 * <p>
 * Note the use of the set constructor in both examples. Being a set,
 * ('aaa','bbbb') is of course the same as ('bbb', 'aaa').
 * </p>
 * <p>
 * The <code>listServices</code> method is overloaded: the last parameter the
 * <code>authzFilter</code> may be omitted. If it is omitted the authorization
 * filtering is performed on the contexts in the session. This is quite
 * different from including the <code>authzFilter</code> parameter with an
 * empty string which means that there is <b>no</b> authz filtering.
 * </p>
 */
  class SAGA_SD_PACKAGE_EXPORT discoverer 
       : public saga::object
  {

   public:

   private:
      // factory
      SAGA_CALL_CREATE_PRIV_2(session const&, saga::url)

      SAGA_CALL_PRIV_2 (list_services, std::string, std::string)
      SAGA_CALL_PRIV_3 (list_services, std::string, std::string, std::string)

   protected:
      TR1::shared_ptr <saga::impl::discoverer> get_impl_sp (void) const;
      saga::impl::discoverer* get_impl (void) const;
      friend class saga::impl::discoverer;
      explicit discoverer(saga::impl::discoverer* impl)
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::does_not_exist,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success));

   public:
    /**
     * Default constructor.  Constructs a discoverer object using a default
     * session.
     *
     * @param loc      URL to use.
     * 
     * @throws not_implemented
     *                 if the discoverer class is not implemented
     *                 by the SAGA implementation at all.
     * @throws incorrect_url
     *                 if an implementation cannot handle the specified
     *                 protocol, or that access to the specified entity
     *                 via the given protocol is impossible.
     * @throws does_not_exist
     *                 if the url is syntactically valid, but no service
     *                 can be contacted at that URL.
     * @throws authorization_failed
     *                 if none of the available contexts of the used
     *                 session could be used for successful authorization.
     *                 That error indicates that the resource could not be
     *                 accessed at all, and not that an operation was not
     *                 available due to restricted permissions.
     * @throws authentication_failed
     *                 if none of the available session contexts could
     *                 successfully be used for authentication.
     * @throws timeout
     *                 if a remote operation did not complete
     *                 successfully because the network communication
     *                 or the remote service timed out.
     * @throws no_success
     *                 if no result can be returned because of
     *                 information system or other internal problems.
     */
     explicit discoverer(saga::url loc = saga::url())
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::does_not_exist,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success));

    /**
     * Constructor.  Constructs a discoverer object using the given
     * session.
     * 
     * @param s        Session to use.
     *
     * @param loc      URL to use.
     *
     * @throws not_implemented
     *                 if the discoverer class is not implemented
     *                 by the SAGA implementation at all.
     * @throws incorrect_url
     *                 if an implementation cannot handle the specified
     *                 protocol, or that access to the specified entity
     *                 via the given protocol is impossible.
     * @throws does_not_exist
     *                 if the url is syntactically valid, but no service
     *                 can be contacted at that URL.
     * @throws authorization_failed
     *                 if none of the available contexts of the used
     *                 session could be used for successful authorization.
     *                 That error indicates that the resource could not be
     *                 accessed at all, and not that an operation was not
     *                 available due to restricted permissions.
     * @throws authentication_failed
     *                 if none of the available session contexts could
     *                 successfully be used for authentication.
     * @throws timeout
     *                 if a remote operation did not complete
     *                 successfully because the network communication
     *                 or the remote service timed out.
     * @throws no_success
     *                 if no result can be returned because of
     *                 information system or other internal problems.
     */
      explicit discoverer(session const& s, saga::url loc = saga::url())
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::does_not_exist,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success));

      // explicit discoverer(saga::object const& o); // is this ctor necessary?
    /**
     * Destructor
     */
      ~discoverer (void) SAGA_THROW_SPEC(throw());

    /**
     * discoverer factory.  Constructs a discoverer object using
     * the given session.
     * 
     * @param s        Session to use.
     *
     * @param loc      URL to use.
     *
     * @throws not_implemented
     *                 if the discoverer class is not implemented
     *                 by the SAGA implementation at all.
     * @throws incorrect_url
     *                 if an implementation cannot handle the specified
     *                 protocol, or that access to the specified entity
     *                 via the given protocol is impossible.
     * @throws does_not_exist
     *                 if the url is syntactically valid, but no service
     *                 can be contacted at that URL.
     * @throws authorization_failed
     *                 if none of the available contexts of the used
     *                 session could be used for successful authorization.
     *                 That error indicates that the resource could not be
     *                 accessed at all, and not that an operation was not
     *                 available due to restricted permissions.
     * @throws authentication_failed
     *                 if none of the available session contexts could
     *                 successfully be used for authentication.
     * @throws timeout
     *                 if a remote operation did not complete
     *                 successfully because the network communication
     *                 or the remote service timed out.
     * @throws no_success
     *                 if no result can be returned because of
     *                 information system or other internal problems.
     */
      static discoverer create(session const& s, saga::url loc = saga::url())
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::does_not_exist,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success))
      {
          return discoverer(s, loc);
      }
      SAGA_CALL_CREATE_2_DEF_1(session const&, saga::url, saga::url())

    /*saga*
     * discoverer factory.  Constructs a discoverer object using
     * a default session and the given URL.
     * 
     * @param loc      URL to use.
     *
     * @throws not_implemented
     *                 if the discoverer class is not implemented
     *                 by the SAGA implementation at all.
     * @throws incorrect_url
     *                 if an implementation cannot handle the specified
     *                 protocol, or that access to the specified entity
     *                 via the given protocol is impossible.
     * @throws does_not_exist
     *                 if the url is syntactically valid, but no service
     *                 can be contacted at that URL.
     * @throws authorization_failed
     *                 if none of the available contexts of the used
     *                 session could be used for successful authorization.
     *                 That error indicates that the resource could not be
     *                 accessed at all, and not that an operation was not
     *                 available due to restricted permissions.
     * @throws authentication_failed
     *                 if none of the available session contexts could
     *                 successfully be used for authentication.
     * @throws timeout
     *                 if a remote operation did not complete
     *                 successfully because the network communication
     *                 or the remote service timed out.
     * @throws no_success
     *                 if no result can be returned because of
     *                 information system or other internal problems.
     */
      static discoverer create(saga::url loc = saga::url())
         SAGA_THROW_SPEC(throw(saga::not_implemented,
                               saga::incorrect_url,
                               saga::not_implemented,
                               saga::authorization_failed,
                               saga::authentication_failed,
                               saga::timeout,
                               saga::no_success))
      {
          return discoverer(loc);
      }

      template <typename Tag>
      static saga::task create(saga::url loc = saga::url())
      {
         return create<Tag>(saga::detail::get_the_session(), loc);
      }

      // default operator=() and copy ctor
      discoverer& operator= (saga::object const& o) 
          SAGA_THROW_SPEC(throw());

      // public methods for the service discoverer

    /**
     * Returns the set of services that pass the set of specified filters. A
     * service will only be included once in the returned list of services.
     * 
     * @param service_filter
     *                a string containing the filter for filtering on the basic
     *                service and site attributes and on related services
     * @param data_filter
     *                a string containing the filter for filtering on key/value
     *                pairs associated with the service
     * @param authz_filter
     *                a string containing the filter for filtering on
     *                authorization information associated with the service
     * @return list of service descriptions, in a random order, matching the
     *         filter criteria
     *
     * @throws bad_parameter
     *                 if any filter has an invalid syntax or if any filter uses
     *                 invalid keys. However the <code>dataFilter</code> never
     *                 signals invalid keys as there is no schema with
     *                 permissible key names.
     * @throws authorization_failed
     *                 if none of the available contexts of the used session
     *                 could be used for successful authorization. That error
     *                 indicates that the resource could not be accessed at all,
     *                 and not that an operation was not available due to
     *                 restricted permissions.
     * @throws authenticationfailed
     *                 if none of the available session contexts could
     *                 successfully be used for authentication
     * @throws timeout
     *                 if a remote operation did not complete successfully
     *                 because the network communication or the remote service
     *                 timed out
     * @throws no_success
     *                 if no result can be returned because of
     *                 information system or other internal problems
     */
      std::vector<saga::sd::service_description>
         list_services(std::string service_filter, 
                       std::string data_filter,
                       std::string authz_filter)
            SAGA_THROW_SPEC(throw(saga::bad_parameter,
                  saga::authorization_failed,
                  saga::authentication_failed,
                  saga::timeout,
                  saga::no_success))
      {
         saga::task t = list_servicespriv(service_filter,
                                          data_filter, 
                                          authz_filter,
                                          saga::task_base::Sync());
         return t.get_result<std::vector<saga::sd::service_description> >();
      }
      SAGA_CALL_PUB_3_DEF_0 (list_services, std::string, std::string, std::string)

    /**
     * Returns the set of services that pass the set of specified filters, an
     * implicit <code>authz_filter</code> is constructed from the contexts of
     * the session. Note that this is different from an empty
     * <code>authz_filter</code>, as that would apply no authorization filter
     * at all.
     * 
     * @param service_filter
     *                a string containing the filter for filtering on the basic
     *                service and site attributes and on related services
     * @param data_filter
     *                a string containing the filter for filtering on key/value
     *                pairs associated with the service
     * @return list of service descriptions, in a random order, matching the
     *         filter criteria
     * @throws bad_parameter
     *                 if any filter has an invalid syntax or if any filter uses
     *                 invalid keys. However the <code>dataFilter</code> never
     *                 signals invalid keys as there is no schema with
     *                 permissible key names.
     * @throws authorization_failed
     *                 if none of the available contexts of the used session
     *                 could be used for successful authorization. That error
     *                 indicates that the resource could not be accessed at all,
     *                 and not that an operation was not available due to
     *                 restricted permissions.
     * @throws authentication_failed
     *                 if none of the available session contexts could
     *                 successfully be used for authentication
     * @throws timeout
     *                 if a remote operation did not complete successfully
     *                 because the network communication or the remote service
     *                 timed out
     * @throws no_success
     *                 if no result can be returned because of information
     *                 system or other internal problems
     */
      std::vector<saga::sd::service_description>
         list_services(std::string service_filter, 
                       std::string data_filter)
            SAGA_THROW_SPEC(throw(saga::bad_parameter,
                  saga::authorization_failed,
                  saga::authentication_failed,
                  saga::timeout,
                  saga::no_success))
      {
         saga::task t = list_servicespriv(service_filter,
                                          data_filter, 
                                          saga::task_base::Sync());
         return t.get_result<std::vector<saga::sd::service_description> >();
      }
      SAGA_CALL_PUB_2_DEF_0 (list_services, std::string, std::string)
  }; // discoverer
 } // namespace sd
}  // namespace saga
#endif
