//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_DETAIL_ATTRIBUTE_HPP
#define SAGA_SAGA_DETAIL_ATTRIBUTE_HPP

// include stl
#include <map>
#include <vector>
#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/task.hpp>

// For symbol import/export macros
#include <saga/saga-defs.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
// for modules not belonging to the engine attributes should not be exported
#if !defined (SAGA_CREATE_PREPROCESSED_FILES) && !defined(SAGA_EXPORT_ATTRIBUTE)

# if defined(SAGA_ENGINE_EXPORTS) || defined(SAGA_ATTRIBUTE_EXPORTS)
#   define SAGA_EXPORT_ATTRIBUTE SAGA_SYMBOL_EXPORT
# elif !defined (SAGA_NO_IMPORT_ATTRIBUTE) && !defined(BUILD_SAGA_LITE)
#   define SAGA_EXPORT_ATTRIBUTE SAGA_SYMBOL_IMPORT
# else
#   define SAGA_EXPORT_ATTRIBUTE /* empty */
# endif

#endif // !SAGA_CREATE_PREPROCESSED_FILES

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace detail
{
    /// @cond
    template <typename Derived, typename Tag>
    struct SAGA_EXPORT_ATTRIBUTE attribute_priv
    {
        typedef std::vector<std::string> strvec_type;

        static saga::task get_attributepriv (Derived const& this_, 
            std::string const&);
        static saga::task set_attributepriv (Derived& this_, 
            std::string const&, std::string const&);

        static saga::task get_vector_attributepriv(Derived const& this_, 
            std::string const&);
        static saga::task set_vector_attributepriv(Derived& this_, 
            std::string const&, strvec_type const&);
        static saga::task remove_attributepriv(Derived& this_, 
            std::string const&);

        static saga::task list_attributespriv(Derived const& this_);
        static saga::task find_attributespriv(Derived const& this_, 
            std::string const&);
        static saga::task attribute_existspriv(Derived const& this_, 
            std::string const&);
        static saga::task attribute_is_readonlypriv(Derived const& this_, 
            std::string const&);
        static saga::task attribute_is_writablepriv(Derived const& this_, 
            std::string const&);
        static saga::task attribute_is_vectorpriv(Derived const& this_, 
            std::string const&);
        static saga::task attribute_is_removablepriv(Derived const& this_, 
            std::string const&);
    };

    template <typename Derived>
    struct SAGA_EXPORT_ATTRIBUTE attribute_sync
      : attribute_priv<Derived, saga::task_base::Sync>
    {
        typedef std::vector<std::string> strvec_type;

        static std::string get_attribute(Derived const& this_, 
            std::string const&);
        static void set_attribute(Derived& this_, 
            std::string const&, std::string const&);

        static void get_vector_attribute(Derived const& this_, 
            std::string const&, strvec_type&);
        static void set_vector_attribute(Derived& this_, 
            std::string const&, strvec_type const&);
        static void remove_attribute(Derived& this_, 
            std::string const&);

        static void list_attributes(Derived const& this_,
            strvec_type&);
        static void find_attributes(Derived const& this_, 
            std::string const&, strvec_type&);

        static bool attribute_exists(Derived const& this_, 
            std::string const&);
        static bool attribute_is_readonly(Derived const& this_, 
            std::string const&);
        static bool attribute_is_writable(Derived const& this_, 
            std::string const&);
        static bool attribute_is_vector(Derived const& this_, 
            std::string const&);
        static bool attribute_is_removable(Derived const& this_, 
            std::string const&);
    };
    /// @endcond

    /*! \brief The SAGA %attribute interface     
     *
     *   Classes implementing this interface maintain a set of %attributes. These
     *   %attributes can be considered as a pair of string-based key-value pairs 
     *   attached to the %object. This interface provies methods for querying,
     *   adding, and removing %attributes.<br>
     *   <br><b>Usage example:</b><br><br>
     *   <code> 
     *   saga::url lfn("rls://localhost//lfn.dat");<br><br>
     *   //logical_file implements the %attribute interface<br>
     *   logical_file::logical_file lf (lfn);<br>
     *
     *   std::vector<std::string> keys(lf.list_attributes());<br>
     *   std::vector<std::string>::const_iterator end = keys.end();<br>
     *   std::vector<std::string>::const_iterator key_it  = keys.begin();<br>
     *
     *   for(; key_it != end; ++key_it) {
     *     std::cout << lf.get_attribute(*key_it) << std::endl };
     *
     *</code> 
     *   
     */
    template <typename Derived>
    struct SAGA_EXPORT_ATTRIBUTE attribute 
    {
    public:
        /*! \brief Attribute string vector type     
         *
         */
        typedef std::vector <std::string>              strvec_type;

        /*! \brief Attribute string map type    
         *
         */
        typedef std::map <std::string, std::string> strmap_type;

    private:
        template <typename Tag>
        saga::task get_attributepriv (std::string const& p, Tag) const
        { return attribute_priv<Derived, Tag>::get_attributepriv(derived(), p); }

        template <typename Tag>
        saga::task set_attributepriv (std::string const& p1, std::string const& p2, Tag)
        { return attribute_priv<Derived, Tag>::set_attributepriv(derived(), p1, p2); }

        template <typename Tag>
        saga::task get_vector_attributepriv(std::string const& p, Tag) const
        { return attribute_priv<Derived, Tag>::get_vector_attributepriv(derived(), p); }

        template <typename Tag>
        saga::task set_vector_attributepriv(std::string const& p1, strvec_type const& p2, Tag)
        { return attribute_priv<Derived, Tag>::set_vector_attributepriv(derived(), p1, p2); }

        template <typename Tag>
        saga::task remove_attributepriv(std::string const& p, Tag)
        { return attribute_priv<Derived, Tag>::remove_attributepriv(derived(), p); }

        template <typename Tag>
        saga::task list_attributespriv(Tag) const
        { return attribute_priv<Derived, Tag>::list_attributespriv(derived()); }

        template <typename Tag>
        saga::task find_attributespriv(std::string const& p1, Tag) const
        { return attribute_priv<Derived, Tag>::find_attributespriv(derived(), p1); }

        template <typename Tag>
        saga::task attribute_existspriv(std::string const& p, Tag) const
        { return attribute_priv<Derived, Tag>::attribute_existspriv(derived(), p); }

        template <typename Tag>
        saga::task attribute_is_readonlypriv(std::string const& p, Tag) const
        { return attribute_priv<Derived, Tag>::attribute_is_readonlypriv(derived(), p); }

        template <typename Tag>
        saga::task attribute_is_writablepriv(std::string const& p, Tag) const
        { return attribute_priv<Derived, Tag>::attribute_is_writablepriv(derived(), p); }

        template <typename Tag>
        saga::task attribute_is_vectorpriv(std::string const& p, Tag) const
        { return attribute_priv<Derived, Tag>::attribute_is_vectorpriv(derived(), p); }

        template <typename Tag>
        saga::task attribute_is_removablepriv(std::string const& p, Tag) const
        { return attribute_priv<Derived, Tag>::attribute_is_removablepriv(derived(), p); }

    public:
        ///@cond
        impl::attribute_interface* get_attr (void);
        impl::attribute_interface* get_attr (void) const;

        template <typename ImplType>
        ImplType *get_target_object() const;
        ///@endcond

    protected:
        ///@cond
        Derived& derived() 
            { return static_cast<Derived&>(*this); }
        Derived const& derived() const 
            { return static_cast<Derived const&>(*this); }

        void init (strmap_type const& scalar_ro  = strmap_type (),
                   strmap_type const& scalar_rw  = strmap_type (),
                   strmap_type const& vector_ro  = strmap_type (),
                   strmap_type const& vector_rw  = strmap_type ());
        void init (char const* const* scalar_ro, char const* const* scalar_rw,
                   char const* const* vector_ro, char const* const* vector_rw);
        void init (bool extensible, bool cache_only = false);

        void init_keynames(strvec_type const& keynames);
        void init_keynames(char const* const* keynames);
        ///@endcond

    public:
        /*! \brief Gets an %attribute value.    
         * \param key Attribute key name
         * \return String value of the %attribute.
         *
         */  
        SAGA_SYMBOL_INTERNAL
        std::string get_attribute(std::string const& key) const
        {
            return attribute_sync<Derived>::
                get_attribute(derived(), key);
        }

        SAGA_CALL_CONST_PUB_1_DEF_0(get_attribute, std::string)

        /*! \brief Sets an %attribute to a value 
         * \param key Attribute key name
         * \param value String value to set the %attribute to
         * 
         */  
        SAGA_SYMBOL_INTERNAL
        void set_attribute(std::string const& key, std::string const& value)
        {
            attribute_sync<Derived>::set_attribute(derived(), key, value);
        }

        SAGA_CALL_PUB_2_DEF_0(set_attribute, std::string, std::string)

        /*! \brief Gets the array of values associated with an %attribute. 
         * \param key Attribute key name
         * \return String array of %attribute values
         *
         */  
        SAGA_SYMBOL_INTERNAL
        strvec_type get_vector_attribute(std::string const& key) const
        {
            strvec_type retval;
            attribute_sync<Derived>::get_vector_attribute(derived(), key, retval);
            return retval;
        }

        SAGA_CALL_CONST_PUB_1_DEF_0(get_vector_attribute, std::string)

        /*! \brief Sets an %attribute to an array of values.    
         * \param key Attribute key name
         * \param values String array of %attribute values
         * 
         */  
        SAGA_SYMBOL_INTERNAL
        void set_vector_attribute(std::string const& key, strvec_type const& value)
        {
            attribute_sync<Derived>::set_vector_attribute(derived(), key, value);
        }

        SAGA_CALL_PUB_2_DEF_0(set_vector_attribute, std::string, strvec_type)

        /*! \brief Removes an %attribute.   
         * \param key Attribute key to be removed
         *
         */  
        SAGA_SYMBOL_INTERNAL
        void remove_attribute(std::string const& key)
        {
            attribute_sync<Derived>::remove_attribute(derived(), key);
        }

        SAGA_CALL_PUB_1_DEF_0(remove_attribute, std::string)

        /*! \brief Gets the list of %attribute keys. 
         * \return Existing %attribute keys
         *
         */   
        SAGA_SYMBOL_INTERNAL
        strvec_type list_attributes() const
        {
            strvec_type retval;
            attribute_sync<Derived>::list_attributes(derived(), retval);
            return retval;
        }

        SAGA_CALL_CONST_PUB_0_DEF_0(list_attributes)

        /*! \brief Finds matching %attributes.
         * \param pattern Key name search pattern
         * \return Matching %attribute keys
         *
         *
         */
        SAGA_SYMBOL_INTERNAL
        strvec_type find_attributes(std::string const& pattern) const
        {
            strvec_type retval;
            attribute_sync<Derived>::find_attributes(derived(), pattern, retval);
            return retval;
        }

        SAGA_CALL_CONST_PUB_1_DEF_0(find_attributes, std::string)

        /*! \brief Checks if the %attribute exists.   
         *  \param key Attribute key name
         * \return true if the %attribute exists, false otherwise 
         *
         */
        SAGA_SYMBOL_INTERNAL
        bool attribute_exists(std::string const& key) const
        {
            return attribute_sync<Derived>::attribute_exists(derived(), key);
        }

        SAGA_CALL_CONST_PUB_1_DEF_0(attribute_exists, std::string)

        /*! \brief Checks the %attribute mode.
         * \param key Attribute key name.
         * \return true if the %attribute is read-only, false otherwise  
         *
         */
        SAGA_SYMBOL_INTERNAL
        bool attribute_is_readonly(std::string const& key) const
        {
            return attribute_sync<Derived>::attribute_is_readonly(derived(), key);
        }

        SAGA_CALL_CONST_PUB_1_DEF_0(attribute_is_readonly, std::string)

        /*! \brief Checks the %attribute mode.
         * \param key Attribute key name.
         * \return true if the %attribute is writeable, false otherwise 
         *         
         */
        SAGA_SYMBOL_INTERNAL
        bool attribute_is_writable(std::string const& key) const
        {
            return attribute_sync<Derived>::attribute_is_writable(derived(), key);
        }

        SAGA_CALL_CONST_PUB_1_DEF_0(attribute_is_writable, std::string)

        /*! \brief Checks if it is vector.    
         * \param key Attribute key name.
         * \return true if the %attribute is a vector, false otherwise 
         *
         */
        SAGA_SYMBOL_INTERNAL
        bool attribute_is_vector(std::string const& key) const
        {
            return attribute_sync<Derived>::attribute_is_vector(derived(), key);
        }

        SAGA_CALL_CONST_PUB_1_DEF_0(attribute_is_vector, std::string)
        
        /*! \brief Checks the attribute mode.
         * \param key Attribute key.
         * \return true if the %attribute is removable, false otherwise 
         *    
         */
        SAGA_SYMBOL_INTERNAL
        bool attribute_is_removable(std::string const& key) const
        {
            return attribute_sync<Derived>::attribute_is_removable(derived(), key);
        }

        SAGA_CALL_CONST_PUB_1_DEF_0(attribute_is_removable, std::string)
    };

///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga::detail

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_SAGA_DETAIL_ATTRIBUTE_HPP

