#ifndef SAGA_PACKAGES_DATA_FILESYSTEM_FILE_HPP
#define SAGA_PACKAGES_DATA_FILESYSTEM_FILE_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/file.hpp")
#endif
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>

#include <saga/saga/packages/namespace/namespace_entry.hpp>
#include <saga/saga/packages/filesystem/config.hpp>
#include <saga/saga/packages/filesystem/iovec.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/filesystem/preprocessed/file.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/file.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{ 
    /*! \brief The <I>File Management</I> API package 
     *
     *   Filesystems provide more than just a namespace: they also provide access 
     *   to the contents of files: the %filesystem package extends the namespace 
     *   package by adding read(), write() and seek() to the entries.
     */   
    namespace filesystem {
    /*! \brief The flags enum is inherited from the namespace package.     
     *
     *  A number of file specific flags are added to it. All added flags 
     * are used for the opening of file and directory instances, and are 
     * not applicable to the operations inherited from the namespace package. 
     */
    enum flags 
    {
        Unknown         = /*  -1, */  saga::name_space::Unknown       ,
        None            = /*   0, */  saga::name_space::None          ,
        Overwrite       = /*   1, */  saga::name_space::Overwrite     ,
        Recursive       = /*   2, */  saga::name_space::Recursive     ,
        Dereference     = /*   4, */  saga::name_space::Dereference,
        Create          = /*   8, */  saga::name_space::Create        ,
        Exclusive       = /*  16, */  saga::name_space::Exclusive     ,
        Lock            = /*  32, */  saga::name_space::Lock          ,
        CreateParents   = /*  64, */  saga::name_space::CreateParents ,
        Truncate        =    128, 
        Append          =    256,
        Read            = /* 512, */  saga::name_space::Read,
        Write           = /*1024, */  saga::name_space::Write,
        ReadWrite       = /*Read | Write, */  saga::name_space::ReadWrite,
        Binary          =   2048 
    };       

    /*! \brief Brief %description starts here     
     *
     *
     */
    enum seek_mode
    {
        Start = 1,
        Current = 2,
        End = 3
    }; 
  
    /*! \brief The SAGA %file package %file API   
     *
     *   This class represents an open %file descriptor for read/write 
     *   operations on a physical %file. Its concept is similar to the %file 
     *   descriptor returned by the open (2) call in POSIX. 
     *
     *
     *   <B>Example:</B>  \include packages/filesystem/file.cpp
     */
    class SAGA_FILESYSTEM_PACKAGE_EXPORT file 
      : public saga::name_space::entry
    {
                      
    private:
        /// @cond
        // factory
        SAGA_CALL_CREATE_PRIV_3(session const&, saga::url, int)

        /** These types are not within API scope */
        SAGA_CALL_PRIV_0 (get_size)
        SAGA_CALL_PRIV_2 (read, saga::mutable_buffer, saga::ssize_t)
        SAGA_CALL_PRIV_2 (write, saga::const_buffer, saga::ssize_t)
        SAGA_CALL_PRIV_2 (seek, saga::off_t, seek_mode)

        // scattered I/O
        SAGA_CALL_PRIV_1 (read_v, std::vector<saga::filesystem::iovec>)
        SAGA_CALL_PRIV_1 (write_v, std::vector<saga::filesystem::const_iovec>)

        // pattern based I/O
        SAGA_CALL_PRIV_1 (size_p, std::string)
        SAGA_CALL_PRIV_2 (read_p, std::string, saga::mutable_buffer)
        SAGA_CALL_PRIV_2 (write_p, std::string, saga::const_buffer)

        // extended based I/O
        SAGA_CALL_PRIV_0 (modes_e)
        SAGA_CALL_PRIV_2 (size_e, std::string, std::string)
        SAGA_CALL_PRIV_3 (read_e, std::string, std::string, saga::mutable_buffer)
        SAGA_CALL_PRIV_3 (write_e, std::string, std::string, saga::const_buffer)
        /// @endcond

    protected:     
        /// @cond
        /** These types are not within API scope */
        TR1::shared_ptr <saga::impl::file> get_impl_sp (void) const;
        saga::impl::file* get_impl (void) const;
        friend class saga::impl::file;
        explicit file (saga::impl::file * init);
        file (session const& s, saga::url url, int mode, saga::off_t pointer);
        /// @endcond

    public:
        /*! \brief Creates the object.
         *
         *
         */
        file (session const& s, saga::url url, int mode = Read);
 
        /*! \brief Creates the object.     
         *
         *
         */
        explicit file (saga::url url, int mode = Read);
 
        /*! \brief Creates the object.     
         *
         *
         */
        explicit file (saga::object const& o);
 
        /*! \brief Creates the object.     
         *
         *
         */
        file (void);

        /*! \brief Destroys the object.     
         *
         *
         */
        ~file (void);

        /*! \brief Factory function resembling file(session, url, int) 
         *         constructor
         */
        static file create(session const & s, saga::url url, int mode = Read)
        {
            return file(s, url, mode);
        }
        SAGA_CALL_CREATE_3_DEF_1(session const&, saga::url, int, Read)

        /*! \brief Factory function resembling file(url, int) constructor
         */
        static file create(saga::url url, int mode = Read)
        {
            return file(url, mode);
        }
        template <typename Tag>
        static saga::task create(saga::url url, int mode = Read)
        {
            return create<Tag>(detail::get_the_session(), url, mode);
        }

        /*! \brief Brief %description starts here     
         *
         *
         */
        file &operator= (saga::object const& o);

        /*! \brief Returns the number of bytes in the file.  
         *
         *
         */
        saga::off_t get_size(void)
        {
            saga::task t = get_sizepriv(saga::task_base::Sync());
            return t.get_result<saga::off_t>();
        }
        SAGA_CALL_PUB_0_DEF_0(get_size)

        /*! \brief Reads up to length bytes from the file into 
         *         the buffer.     
         *  \param length Number of bytes to be read 
         *  \param buffer Buffer to read into
         *  \return Number of bytes successfully read
         *         
         *
         *
         */
        saga::ssize_t read(saga::mutable_buffer buffer, 
            saga::ssize_t length = 0)
        {
            saga::task t = readpriv(buffer, length, saga::task_base::Sync());
            return t.get_result<saga::ssize_t>();
        }
        SAGA_CALL_PUB_2_DEF_1(read, saga::mutable_buffer, saga::ssize_t, 0)

        /*! \brief Writes up to length bytes from buffer into 
         *         the file at the current file position. 
         *  \param length Number of bytes to write
         *  \param buffer Data to write
         *  \return Number of bytes successfully written 
         *
         *
         *
         */
        saga::ssize_t write(saga::const_buffer buffer, 
            saga::ssize_t length = 0)
        {
            saga::task t = writepriv(buffer, length, saga::task_base::Sync());
            return t.get_result<saga::ssize_t>();
        }
        SAGA_CALL_PUB_2_DEF_1(write, saga::const_buffer, saga::ssize_t, 0)

        /*! \brief Reposition the file pointer.    
         *  \param offset Offset in bytes to move pointer 
         *  \param mode Offset is relative to mode
         *  \return position of pointer after seek 
         *
         *
         *
         */
        saga::off_t seek(saga::off_t offset, seek_mode mode)
        {
            saga::task t = seekpriv(offset, mode, saga::task_base::Sync());
            return t.get_result<saga::off_t>();
        }
        SAGA_CALL_PUB_2_DEF_0(seek, saga::off_t, seek_mode)

        /*! \brief Gather/scatter read     
         *  \param buffer_vec Array of buffer_vec structs defining 
         *         start (offset) and length (length) of each 
         *         individual read, buffer to read into, and integer 
         *         to store result into. 
         *         
         *	
         *
         */
        void read_v(std::vector<saga::filesystem::iovec> buffer_vec)
        {
            saga::task t = read_vpriv(buffer_vec, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_1_DEF_0(read_v, std::vector<saga::filesystem::iovec>)

        /*! \brief Gather/scatter write     
         *  \param buffer_vec Array of buffer_vec structs defining start (offset) 
         *         and length (length) of each individual write, and 
         *         buffers containing the data to write 
         *         
         * 
         *
         */
        void write_v(std::vector<saga::filesystem::const_iovec> buffer_vec)
        {
            saga::task t = write_vpriv(buffer_vec, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_1_DEF_0(write_v, std::vector<saga::filesystem::const_iovec>)

       /*! \brief Determine the storage size required for a pattern I/O operation 
        *  \param pattern Pattern to determine size for 
        *  \return Size required for I/O operation with that pattern 
        *
        *
        */
        saga::ssize_t size_p(std::string pattern)
        {
            saga::task t = size_ppriv(pattern, saga::task_base::Sync());
            return t.get_result<saga::ssize_t>();
        }
        SAGA_CALL_PUB_1_DEF_0(size_p, std::string)

       /*! \brief Pattern-based read     
        *
        * \param pattern Pattern specification for read operation 
        * \param buffer Buffer to store read bytes into 
        * \return Number of successfully read bytes
        *
        *
         */
        saga::ssize_t read_p(std::string pattern, saga::mutable_buffer buffer)
        {
            saga::task t = read_ppriv(pattern, buffer, saga::task_base::Sync());
            return t.get_result<saga::ssize_t>();
        }
        SAGA_CALL_PUB_2_DEF_0(read_p, std::string, saga::mutable_buffer)

        /*! \brief Pattern-based write
        *
        * \param pattern Pattern specification for read operation 
        * \param buffer Buffer to store read bytes into 
        * \return number of bytes successfully written 
        *
        *
        */
        saga::ssize_t write_p(std::string pattern, saga::const_buffer buffer)
        {
            saga::task t = write_ppriv(pattern, buffer, saga::task_base::Sync());
            return t.get_result<saga::ssize_t>();
        }
        SAGA_CALL_PUB_2_DEF_0(write_p, std::string, saga::const_buffer)

       /*! \brief List the extended modes available in this implementation, 
        *         and/or on server side    
        *
        * \param ret Pattern to determine size for 
        * \return Size required for I/O operation with that pattern 
        *
        *
        */     
        std::vector<std::string> modes_e()
        {
            saga::task t = modes_epriv(saga::task_base::Sync());
            return t.get_result<std::vector<std::string> >();
        }
        SAGA_CALL_PUB_0_DEF_0(modes_e)

        /*! \brief determine the storage size required for an extended 
         *         I/O operation 
         * 
         *
         */
        saga::size_t size_e(std::string ext_mode, std::string specification)
        {
            saga::task t = size_epriv(ext_mode, specification, saga::task_base::Sync());
            return t.get_result<saga::ssize_t>();
        }
        SAGA_CALL_PUB_2_DEF_0(size_e, std::string, std::string)

        /*! \brief Extended read     
        * \param ext_mode Extended mode to use 
        * \param specification Specification of read operation 
        * \param buffer Buffer to store read bytes into 
        * \return Number of successfully read bytes 
        *
        *
        */
        saga::ssize_t read_e(std::string ext_mode, std::string specification,
            saga::mutable_buffer buffer)
        {
            saga::task t = read_epriv(ext_mode, specification, buffer, 
                saga::task_base::Sync());
            return t.get_result<saga::ssize_t>();
        }
        /// @cond
        SAGA_CALL_PUB_3_DEF_0(read_e, std::string, std::string, 
            saga::mutable_buffer)
        /// @endcond

        /*! \brief Extended write     
         *
         * \param ext_mode Extended mode to use 
         * \param specification Specification of write operation 
         * \param buffer Buffer to store read bytes into 
         * \return Number of successfully read bytes 
         *
         *
         */     
        saga::ssize_t write_e(std::string ext_mode, std::string specification,
            saga::const_buffer buffer)
        {
            saga::task t = write_epriv(ext_mode, specification, buffer, 
                saga::task_base::Sync());
            return t.get_result<saga::ssize_t>();
        }
        /// @cond
        SAGA_CALL_PUB_3_DEF_0(write_e, std::string, std::string, 
            saga::const_buffer)
        /// @endcond
        
            
    }; // class file

    } // namespace file
} // namespace saga

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_DATA_FILESYSTEM_FILE_HPP)

