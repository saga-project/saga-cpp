#ifndef SAGA_PACKAGES_DATA_ADVERT_ADVERT_HPP
#define SAGA_PACKAGES_DATA_ADVERT_ADVERT_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/advert.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <string>
#include <vector>

#include <boost/utility/enable_if.hpp>
#include <boost/lexical_cast.hpp>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/detail/attribute.hpp>
#include <saga/saga/detail/monitorable.hpp>

#include <saga/saga/packages/namespace/namespace_entry.hpp>
#include <saga/saga/packages/advert/config.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/advert/preprocessed/advert.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/advert.hpp")
#endif

#if !defined(SAGA_ADVERT_NO_DEPRECATED)
#define SAGA_ADVERT_DEPRECATED_MSG "This function is deprecated and will be removed in the future."
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    /*! \brief The <I>%Advert Service</I> API package 
     *
     */   
    namespace advert 
    {
        /*! \brief Available %metrics defined for the <I>%Advert Service</I> API package 
         *
         */   
        namespace metrics 
        {
            /*! \brief Brief description here 
             *
             */
            char const* const advert_modified = "advert.Modified";

            /*! \brief Brief description here 
             *
             */
            char const* const advert_deleted = "advert.Deleted";
        }

        /*! \brief The available mode flags for %advert package %objects 
         *
         */   
        enum flags 
        {
            Unknown         = /*  -1, */  saga::name_space::Unknown, 
            None            = /*   0, */  saga::name_space::None, 
            Overwrite       = /*   1, */  saga::name_space::Overwrite, 
            Recursive       = /*   2, */  saga::name_space::Recursive, 
            Dereference     = /*   4, */  saga::name_space::Dereference, 
            Create          = /*   8, */  saga::name_space::Create, 
            Exclusive       = /*  16, */  saga::name_space::Exclusive, 
            Lock            = /*  32, */  saga::name_space::Lock, 
            CreateParents   = /*  64, */  saga::name_space::CreateParents, 
            // 256, reserved for Truncate
            // 512,  reserved for Append 
            Read            =    512, 
            Write           =   1024, 
            ReadWrite       =   Read | Write 
            // 2048, reserved for Binary
        }; 

        /*! \brief The %advert class represents an <b>entry</b> in a 
         *         saga::advert::directory. An %advert %directory is a meta data 
         *         %directory with a hierarchical namespace attached. This class
         *         inherits all methods from saga::name_space::namespace_entry and provides 
         *         additional methods to publish new and query existing adverts.
         *
         */
        class SAGA_ADVERT_PACKAGE_EXPORT entry 
          : public saga::name_space::entry,
            public saga::detail::attribute<entry>
        {
        protected:
            /// @cond
            /** These types are not within API scope */
            friend class saga::impl::advert;

            friend struct saga::detail::attribute<entry>; 
            typedef saga::detail::attribute<entry> attribute_base;
            typedef saga::name_space::entry::monitorable_base monitorable_base;

            TR1::shared_ptr <saga::impl::advert> get_impl_sp(void) const;
            saga::impl::advert* get_impl (void) const;
            explicit entry(saga::impl::advert *impl);
            /// @endcond

        private:
            /// @cond
            // factory
            SAGA_CALL_CREATE_PRIV_3(session const&, saga::url const&, int)

            SAGA_CALL_PRIV_1(store_object, saga::object)
            SAGA_CALL_PRIV_0(retrieve_object)
            SAGA_CALL_PRIV_1(retrieve_object, saga::session)
            SAGA_CALL_PRIV_1(store_string, std::string)
            SAGA_CALL_PRIV_0(retrieve_string)
            /// @endcond

            template <typename Tag, typename T>
            saga::task store_object_or_string (Tag, T const& object, boost::mpl::true_)
            {
                return store_objectpriv(object, Tag());
            }

            template <typename Tag, typename T>
            saga::task store_object_or_string (Tag, T const& object, boost::mpl::false_)
            {
                return store_stringpriv(
                    boost::lexical_cast<std::string>(object), Tag()); 
            }

        public:
            /*! \brief Constructs an %advert %object that points to %url, opens 
             *         it in mode and associates it with the %session s.
             */
            entry (saga::session const& s, saga::url url, int mode = Read);

            /*! \brief Constructs an %advert %object that points to %url and opens 
             *         it in mode. 
             */
            explicit entry (saga::url url, int mode = Read);

            /*! \brief Constructs a copy of other.
             */
            explicit entry (saga::object const& other);

            /*! \brief Constructs a default %advert %object.
             */
            entry (void);

            /*! \brief Destroys the %advert %object and frees all allocated 
             *         resources.
             */
            ~entry (void);

            /*! \brief Factory function resembling entry(session, url, int) 
             *         constructor
             */
            static entry create(session const& s, saga::url const& url, 
                int mode = Read)
            {
                return entry(s, url, mode);
            }
            /// @cond
            SAGA_CALL_CREATE_3_DEF_1(session const&, saga::url const&, int, Read)
            /// @endcond

            /*! \brief Factory function resembling entry(url, int) constructor
             */
            static entry create(saga::url const& url, int mode = Read)
            {
                return entry(url, mode);
            }
            template <typename Tag>
            static saga::task create(saga::url const& url, int mode = Read)
            {
                return create<Tag>(detail::get_the_session(), url, mode);
            }

            ///@cond 
            entry &operator= (saga::object const& object);
            ///@endcond

            /*! \brief Stores a %saga %object in this %advert.
             *
             *
             *  @param object The object to store
             */
            void store_object (saga::object object) 
            { 
                saga::task t = store_objectpriv(object, saga::task_base::Sync()); 
                t.get_result ();
            }

            template <typename T>
            typename boost::disable_if<detail::has_sync_tag<T> >::type
            store_object (T const& object) 
            { 
                typedef boost::mpl::bool_<
                    detail::is_saga_object<T>::value> is_saga_object;
                saga::task t = store_object_or_string(saga::task_base::Sync(), 
                    object, is_saga_object());
                t.get_result();
            }
            /// @cond
            template <typename Tag, typename T>
            typename boost::enable_if<detail::has_sync_tag<Tag>, saga::task>::type
            store_object (T const& object)
            {
                typedef boost::mpl::bool_<
                    detail::is_saga_object<T>::value> is_saga_object;
                return store_object_or_string(Tag(), object, is_saga_object());
            }
            /// @endcond

            /*! \brief Retrieves the %saga %object stored in this %advert.
             *
             *
             *  @return The object stored in this advert
             */
            saga::object retrieve_object() 
            { 
                saga::task t = retrieve_objectpriv (saga::task_base::Sync()); 
                return t.get_result<saga::object>();
            }
            /// @cond
//            SAGA_CALL_PUB_0_DEF_0(retrieve_object)
            template <typename Tag>
            typename boost::enable_if<detail::has_sync_tag<Tag>, saga::task>::type
            retrieve_object()
            { 
                return retrieve_objectpriv(Tag()); 
            }
            /// @endcond

            template <typename T>
            T& retrieve_object(
                typename boost::disable_if<detail::has_sync_tag<T> >::type* = 0) 
            { 
                saga::task t = retrieve_stringpriv (saga::task_base::Sync()); 
                return t.get_result<T>();
            }
            /// @cond
            template <typename T, typename Tag>
            typename boost::disable_if<detail::has_sync_tag<T>, saga::task>::type
            retrieve_object() 
            {
                return retrieve_stringpriv(Tag());
            }
            /// @endcond

            /*! \brief Retrieves the %saga %object stored in this %advert and 
             *         associates it with the %session s.
             *
             *
             *  @param s a %session %object
             *  @return The object stored in this advert
             */
            saga::object retrieve_object (saga::session const& s) 
            { 
                saga::task t = retrieve_objectpriv(s, saga::task_base::Sync()); 
                return t.get_result<saga::object>();
            }
            /// @cond
            template <typename Tag>
            typename boost::enable_if<detail::has_sync_tag<Tag>, saga::task>::type
            retrieve_object(saga::session const& s)
            { 
                return retrieve_objectpriv(s, Tag()); 
            }
            /// @endcond

#if !defined(SAGA_ADVERT_NO_DEPRECATED)
            /// obsolete API below, don't use this anymore
            /*! \brief Stores a string in this %advert.
             *
             *
             *  @param str The string to store
             */
            SAGA_DEPRECATED(SAGA_ADVERT_DEPRECATED_MSG) 
            void store_string (std::string str) 
            { 
                saga::task t = store_stringpriv(str, saga::task_base::Sync()); 
                t.get_result ();
            }
//             SAGA_CALL_PUB_1_DEF_0(store_string, std::string)
            /// @cond
            template <typename Tag>
            SAGA_DEPRECATED(SAGA_ADVERT_DEPRECATED_MSG) 
            saga::task store_string(std::string str) 
            {
                return store_stringpriv(str, Tag());
            }
            /// @endcond

            /*! \brief Retrieves the string stored in this %advert.
             *
             *
             *  @return The string stored in this advert
             */
            SAGA_DEPRECATED(SAGA_ADVERT_DEPRECATED_MSG) 
            std::string retrieve_string (void) 
            { 
                saga::task t = retrieve_stringpriv (saga::task_base::Sync()); 
                return t.get_result<std::string>();
            }
//             SAGA_CALL_PUB_0_DEF_0(retrieve_string)
            /// @cond
            template <typename Tag>
            SAGA_DEPRECATED(SAGA_ADVERT_DEPRECATED_MSG) 
            saga::task retrieve_string()
            { 
                return retrieve_stringpriv(Tag()); 
            }
            /// @endcond
#endif

        }; // class advert
    } // namespace advert
} // namespace saga

#undef SAGA_ADVERT_DEPRECATED_MSG

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_DATA_ADVERT_ADVERT_HPP)

