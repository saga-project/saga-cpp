//  Copyright (c) 2007-2009 Hartmut Kaiser
//  Copyright (c) Christopher Diggins 2005
//  Copyright (c) Pablo Aguilar 2005
//  Copyright (c) Kevlin Henney 2001
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  The class saga::detail::hold_any is built based on the any class
//  published here: http://www.codeproject.com/cpp/dynamic_typing.asp. 

#if !defined(SAGA_HOLD_ANY_DEC_01_2007_0133PM)
#define SAGA_HOLD_ANY_DEC_01_2007_0133PM

#include <boost/config.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/throw_exception.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_const.hpp>

#include <stdexcept>
#include <typeinfo>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
/// @cond
/** Hide from Doxygen */
namespace saga { namespace detail
{
    struct bad_any_cast
      : std::bad_cast
    {
        bad_any_cast(std::type_info const& src, std::type_info const& dest)
          : from(src.name()), to(dest.name())
        {}

        virtual const char* what() const throw() { return "bad any cast"; }

        const char* from;
        const char* to;
    };

    ///@cond
    namespace internals
    {
        // function pointer table
        struct fxn_ptr_table
        {
            std::type_info const& (*get_type)();
            void (*static_delete)(void**);
            void (*destruct)(void**);
            void (*clone)(void* const*, void**);
            void (*move)(void* const*, void**);
        };

        // static functions for small value-types
        template<typename Small>
        struct fxns;

        template<>
        struct fxns<boost::mpl::true_>
        {
            template<typename T>
            struct type
            {
                static std::type_info const& get_type()
                {
                    return typeid(T);
                }
                static void static_delete(void** x)
                {
                    reinterpret_cast<T*>(x)->~T();
                }
                static void destruct(void** x)
                {
                    reinterpret_cast<T*>(x)->~T();
                }
                static void clone(void* const* src, void** dest)
                {
                    new (dest) T(*reinterpret_cast<T const*>(src));
                }
                static void move(void* const* src, void** dest)
                {
                    reinterpret_cast<T*>(dest)->~T();
                    *reinterpret_cast<T*>(dest) =
                        *reinterpret_cast<T const*>(src);
                }
            };
        };

        // static functions for big value-types (bigger than a void*)
        template<>
        struct fxns<boost::mpl::false_>
        {
            template<typename T>
            struct type
            {
                static std::type_info const& get_type()
                {
                    return typeid(T);
                }
                static void static_delete(void** x)
                {
                    // destruct and free memory
                    delete (*reinterpret_cast<T**>(x));
                }
                static void destruct(void** x)
                {
                    // destruct only, we'll reuse memory
                    (*reinterpret_cast<T**>(x))->~T();
                }
                static void clone(void* const* src, void** dest)
                {
                    *dest = new T(**reinterpret_cast<T* const*>(src));
                }
                static void move(void* const* src, void** dest)
                {
                    (*reinterpret_cast<T**>(dest))->~T();
                    **reinterpret_cast<T**>(dest) =
                        **reinterpret_cast<T* const*>(src);
                }
            };
        };

        template<typename T>
        struct get_table
        {
            typedef boost::mpl::bool_<(sizeof(T) <= sizeof(void*))> is_small;

            static fxn_ptr_table* get()
            {
                static fxn_ptr_table static_table =
                {
                    fxns<is_small>::template type<T>::get_type,
                    fxns<is_small>::template type<T>::static_delete,
                    fxns<is_small>::template type<T>::destruct,
                    fxns<is_small>::template type<T>::clone,
                    fxns<is_small>::template type<T>::move
                };
                return &static_table;
            }
        };

        ///////////////////////////////////////////////////////////////////////
        struct empty {};

    }
    ///@endcond

    ///////////////////////////////////////////////////////////////////////////
    // We need to be able to default construct any type stored in a hold_any
    // Some of the saga API objects do have a default constructor doing more
    // than just creating an empty API (facade object), which is evil. So we
    // use the create_default template. This template is specialized for those 
    // picky types elsewhere.
    template <typename T>
    struct create_default
    {
        static T* call() { return new T; }
        template <typename T_> static void call(T_* obj) { new (obj) T; }
    };

    ///////////////////////////////////////////////////////////////////////////
    class hold_any
    {
    public:
        // constructors
        template <typename T>
        hold_any(T const& x)
          : table(saga::detail::internals::get_table<T>::get()), object(0)
        {
            if (saga::detail::internals::get_table<T>::is_small::value)
                new (&object) T(x);
            else
                object = new T(x);
        }

        hold_any()
          : table(saga::detail::internals::get_table<saga::detail::internals::empty>::get()),
            object(0)
        {
        }

        hold_any(hold_any const& x)
          : table(saga::detail::internals::get_table<saga::detail::internals::empty>::get()),
            object(0)
        {
            assign(x);
        }

        ~hold_any()
        {
            table->static_delete(&object);
        }

        // assignment
        hold_any& assign(hold_any const& x)
        {
            if (&x != this) {
                // are we copying between the same type?
                if (table == x.table) {
                    // if so, we can avoid reallocation
                    table->move(&x.object, &object);
                }
                else {
                    reset();
                    x.table->clone(&x.object, &object);
                    table = x.table;
                }
            }
            return *this;
        }

        template <typename T>
        hold_any& assign(T const& x)
        {
            // are we copying between the same type?
            saga::detail::internals::fxn_ptr_table* x_table = 
                saga::detail::internals::get_table<T>::get();
            if (table == x_table) {
            // if so, we can avoid deallocating and re-use memory
                table->destruct(&object);    // first destruct the old content
                if (saga::detail::internals::get_table<T>::is_small::value) {
                    // create copy on-top of object pointer itself
                    new (&object) T(x);
                }
                else {
                    // create copy on-top of old version
                    new (object) T(x);
                }
            }
            else {
                if (saga::detail::internals::get_table<T>::is_small::value) {
                    // create copy on-top of object pointer itself
                    table->destruct(&object); // first destruct the old content
                    new (&object) T(x); 
                }
                else {
                    reset();                  // first delete the old content
                    object = new T(x);
                }
                table = x_table;      // update table pointer
            }
            return *this;
        }

        template <typename T>
        hold_any& init()
        {
            // are we copying between the same type?
            saga::detail::internals::fxn_ptr_table* x_table = 
                saga::detail::internals::get_table<T>::get();
            if (table == x_table) {
            // if so, we can avoid deallocating and re-use memory
                table->destruct(&object);    // first destruct the old content
                if (saga::detail::internals::get_table<T>::is_small::value) {
                    // create copy on-top of object pointer itself
                    create_default<T>::call(&object);
                }
                else {
                    // create copy on-top of old version
                    create_default<T>::call(object);
                }
            }
            else {
                if (saga::detail::internals::get_table<T>::is_small::value) {
                    // create copy on-top of object pointer itself
                    table->destruct(&object); // first destruct the old content
                    create_default<T>::call(&object); 
                }
                else {
                    reset();                  // first delete the old content
                    object = create_default<T>::call();
                }
                table = x_table;      // update table pointer
            }
            return *this;
        }

        // assignment operator
        template <typename T>
        hold_any& operator=(T const& x)
        {
            return assign(x);
        }

        // utility functions
        hold_any& swap(hold_any& x)
        {
            std::swap(table, x.table);
            std::swap(object, x.object);
            return *this;
        }

        std::type_info const& type() const
        {
            return table->get_type();
        }

        template <typename T>
        T const& cast() const
        {
            if (type() != typeid(T))
              throw bad_any_cast(type(), typeid(T));

            return saga::detail::internals::get_table<T>::is_small::value ?
                *reinterpret_cast<T const*>(&object) :
                *reinterpret_cast<T const*>(object);
        }

// implicit casting is disabled by default for compatibility with boost::any
#ifdef BOOST_SPIRIT_ANY_IMPLICIT_CASTING
        // automatic casting operator
        template <typename T>
        operator T const& () const { return cast<T>(); }
#endif // implicit casting

        bool empty() const
        {
            return 0 == object;
        }

        void reset()
        {
            if (!empty())
            {
                table->static_delete(&object);
                table = saga::detail::internals::get_table<saga::detail::internals::empty>::get();
                object = 0;
            }
        }

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    private: // types
      template<typename T>
      friend T* any_cast(hold_any *);
      template<typename T>
      friend T* any_cast(hold_any *, boost::mpl::true_);
      template<typename T>
      friend T* any_cast(hold_any *, boost::mpl::false_);
#else
    public: // types (public so any_cast can be non-friend)
#endif
        // fields
        saga::detail::internals::fxn_ptr_table* table;
        void* object;
    };

    // boost::any-like casting
    template <typename T>
    inline T* any_cast (hold_any* operand, boost::mpl::true_)
    {
        if (operand && operand->type() == typeid(T)) {
            BOOST_STATIC_ASSERT(sizeof(std::size_t) >= sizeof(void*));
            return saga::detail::internals::get_table<T>::is_small::value ?
                reinterpret_cast<T*>(
                  reinterpret_cast<std::size_t>(&operand->object)) :
                reinterpret_cast<T*>(operand->object);
        }
        return 0;
    }

    template <typename T>
    inline T* any_cast (hold_any* operand, boost::mpl::false_)
    {
        if (operand) {
            if (operand->empty()) 
                operand->init<T>();
            
            if (operand->type() == typeid(T)) {
                BOOST_STATIC_ASSERT(sizeof(std::size_t) >= sizeof(void*));
                return saga::detail::internals::get_table<T>::is_small::value ?
                    reinterpret_cast<T*>(
                       reinterpret_cast<std::size_t>(&operand->object)) :
                    reinterpret_cast<T*>(operand->object);
            }
        }
        return 0;
    }

    template <typename T>
    inline T* any_cast (hold_any* operand)
    {
        return any_cast<T>(operand, boost::mpl::bool_<boost::is_const<T>::value>());
    }
    
    template <typename T>
    inline T const* any_cast(hold_any const* operand)
    {
        return any_cast<T>(const_cast<hold_any*>(operand));
    }

    template <typename T>
    T any_cast(hold_any& operand)
    {
        typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        // If 'nonref' is still reference type, it means the user has not
        // specialized 'remove_reference'.

        // Please use BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION macro
        // to generate specialization of remove_reference for your class
        // See type traits library documentation for details
        BOOST_STATIC_ASSERT(!is_reference<nonref>::value);
#endif

        nonref* result = any_cast<nonref>(&operand);
        if(!result)
            boost::throw_exception(bad_any_cast(operand.type(), typeid(T)));
        return *result;
    }

    template <typename T>
    T const& any_cast(hold_any const& operand)
    {
        typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<T>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        // The comment in the above version of 'any_cast' explains when this
        // assert is fired and what to do.
        BOOST_STATIC_ASSERT(!is_reference<nonref>::value);
#endif

        return any_cast<nonref const&>(const_cast<hold_any &>(operand));
    }

}}

///@endcond

#endif

