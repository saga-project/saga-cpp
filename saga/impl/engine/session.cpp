//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/shared_ptr.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/session.hpp>
#include <saga/impl/context.hpp>
#include <saga/impl/context_serialization.hpp>
#include <saga/impl/runtime.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    ///////////////////////////////////////////////////////////////////////////
    struct base_context_list
    {
        typedef 
            std::vector<std::pair<std::string, std::string> > 
        proto_context_type;
        typedef 
            std::vector<std::pair<std::string, std::vector<std::string> > > 
        vector_proto_context_type;

        base_context_list(saga::impl::session const* s)
          : session_(s)
        {}
        virtual ~base_context_list() {}

        virtual bool is_proto_context() = 0;

        virtual void 
        add_context (saga::context const& c)
        {
            SAGA_THROW_VERBATIM(session_, "Not implemented", saga::NotImplemented);
        }
        virtual void 
        remove_context (saga::context const& c)
        {
            SAGA_THROW_VERBATIM(session_, "Not implemented", saga::NotImplemented);
        }
        virtual void 
        remove_all_contexts (void)
        {
            SAGA_THROW_VERBATIM(session_, "Not implemented", saga::NotImplemented);
        }
        virtual void 
        add_proto_context(proto_context_type const& entries)
        {
            SAGA_THROW_VERBATIM(session_, "Not implemented", saga::NotImplemented);
        }
        virtual void 
        add_proto_context(proto_context_type const& entries,
            vector_proto_context_type const& vector_entries)
        {
            SAGA_THROW_VERBATIM(session_, "Not implemented", saga::NotImplemented);
        }

        virtual void 
        fill_contexttypes(v1_0::preference_type& prefs) const
        {
            SAGA_THROW_VERBATIM(session_, "Not implemented", saga::NotImplemented);
        }

        saga::impl::session const* session_;
    };

    ///////////////////////////////////////////////////////////////////////////
    struct proto_context_list : public base_context_list
    {
        proto_context_list(saga::impl::session const* s)
          : base_context_list(s)
        {}
        ~proto_context_list()
        {}

        bool is_proto_context() 
        { 
            return true; 
        }

        // make a temporary copy of the keys and values
        void add_proto_context(proto_context_type const& entries)
        {
            entries_.push_back(entries);
            vector_entries_.push_back(vector_proto_context_type());
        }

        // make a temporary copy of the keys and values
        void add_proto_context(proto_context_type const& entries,
            vector_proto_context_type const& vector_entries)
        {
            entries_.push_back(entries);
            vector_entries_.push_back(vector_entries);
        }

        void fill_contexttypes(v1_0::preference_type& prefs) const
        {
            for (std::size_t i = 0; i < entries_.size(); ++i)
            {
                std::vector<std::pair<std::string, std::string> > const& entry = 
                    entries_[i];

                bool has_type = false;
                for (std::size_t k = 0; k < entry.size(); ++k) 
                {
                    if (entry[k].first == saga::attributes::context_type) {
                        prefs.set(saga::attributes::context_type, entry[k].second);
                        has_type = true;
                    }
                }
                if (!has_type) {
                    SAGA_THROW_VERBATIM(session_, 
                        "List of attribute keys does not contain the context's "
                        "'type'", saga::BadParameter);
                }
            }
        }

        std::vector<proto_context_type> entries_;
        std::vector<vector_proto_context_type> vector_entries_;
    };

    ///////////////////////////////////////////////////////////////////////////
    struct context_list : public base_context_list
    {
        typedef std::vector <saga::context> context_list_type;

        context_list(saga::impl::session const* s)
          : base_context_list(s)
        {}

        context_list(context_list const& rhs)
          : base_context_list(rhs.session_), contexts_(rhs.contexts_)
        {}

        context_list(saga::impl::session const* s, proto_context_list* protolist)
          : base_context_list(s)
        {
            for (std::size_t i = 0; i < protolist->entries_.size(); ++i)
            {
                saga::context c;
                try {
                    // add non-vector attributes
                    std::vector<std::pair<std::string, std::string> >& entries = 
                        protolist->entries_[i];

                    bool has_type = false;
                    for (std::size_t k = 0; k < entries.size(); ++k) 
                    {
                        if (entries[k].first == saga::attributes::context_type)
                            has_type = true;

                        c.set_attribute(entries[k].first, entries[k].second);
                    }
                    if (!has_type) {
                        SAGA_THROW_VERBATIM(session_, 
                            "List of attribute keys does not contain the context's "
                            "'type'", saga::BadParameter);
                    }

                    // add vector_attributes
                    vector_proto_context_type& vector_entries = 
                        protolist->vector_entries_[i];
                    for (std::size_t v = 0; v < vector_entries.size(); ++v) 
                    {
                        c.set_vector_attribute(vector_entries[v].first, 
                            vector_entries[v].second);
                    }

                    // add context to session
                    add_context(c);
                }
                catch (saga::exception const&) {
                    continue;     // ignore any saga related errors
                }
            }
        }
        ~context_list()
        {}

        bool is_proto_context() 
        { 
            return false; 
        }

        void add_context (saga::context const& c)
        {
            context_list_type::iterator it = 
                std::find(contexts_.begin(), contexts_.end(), c);
            if (it == contexts_.end())
            {
                saga::context ctx(c.clone());

                saga::task t = impl::runtime::get_impl(ctx)->set_defaults(true);
                t.rethrow();                // report errors

                contexts_.push_back (ctx);
            }
        }

        void remove_context (saga::context const& c)
        {
            context_list_type::iterator it = 
                std::find(contexts_.begin(), contexts_.end(), c);
            if (it == contexts_.end())
            {
                SAGA_THROW("remove_context: context does not exist", 
                    saga::DoesNotExist);
            }
            contexts_.erase (it);
        }

        void remove_all_contexts (void)
        {
            contexts_.clear ();
        }

        void fill_contexttypes(v1_0::preference_type& prefs) const
        {
            std::vector<saga::context>::const_iterator end = contexts_.end();
            for (std::vector<saga::context>::const_iterator it = contexts_.begin(); 
                 it != end; ++it)
            {
                if ((*it).attribute_exists(saga::attributes::context_type))
                {
                    prefs.set(saga::attributes::context_type,
                        (*it).get_attribute(saga::attributes::context_type));
                }
            }
        }

        context_list_type contexts_;
    };

    ///////////////////////////////////////////////////////////////////////////
    session::~session() 
    { 
        mutex_type::scoped_lock lock(mtx_);
        engine_.tidy();
        workitems_.tidy();
        delete contexts_;
        preferences_.clear();
    }

    // saga:: object interface: clone this session instance
    saga::object session::clone() const
    {
        if (!is_opened_)
        {
            SAGA_THROW("close: session has been closed", 
                saga::IncorrectState)
            return saga::object();
        }

        if (!engine_.has_been_initialized()) 
        {
            mutex_type::scoped_lock lock(mtx_);
            engine_.late_initialize(this);
        }

        saga::session s;

        typedef std::vector<saga::context> context_list_type;
        context_list_type ctxlist = list_contexts();

        context_list_type::const_iterator end = ctxlist.end();
        for (context_list_type::const_iterator it = ctxlist.begin();
             it != end; ++it)
        {
            s.add_context(*it);
        }

        return s;
    }

    // maintain internal list of context's
    std::vector<saga::context> session::list_contexts() const
    {
        if (!is_opened_)
        {
            SAGA_THROW("list_contexts: session has been closed", 
                saga::IncorrectState)
            return std::vector<saga::context>();
        }

        mutex_type::scoped_lock lock(mtx_);
        engine_.late_initialize(this);

        if (NULL == contexts_) {
            contexts_ = new context_list(this);
        }
        else if (contexts_->is_proto_context()) {
            base_context_list* oldctxs = contexts_;
            contexts_ = new context_list(this, (proto_context_list*)contexts_);
            delete oldctxs;
        }

        return ((context_list*)contexts_)->contexts_;   // make a copy
    }

    // if 'use_defaults' is flagged, the session is stripped of all default
    // contexts, and only the given context is added.  Later additions will not
    // strip any context
    void session::add_context (saga::context const& c)
    {
        if (!is_opened_)
        {
            SAGA_THROW("add_context: session has been closed", 
                saga::IncorrectState)
            return;
        }

        // create context list if needed
        mutex_type::scoped_lock lock(mtx_);
        engine_.late_initialize(this);

        if (NULL == contexts_) {
            contexts_ = new context_list(this);
        }
        else if (contexts_->is_proto_context()) {
            base_context_list* oldctxs = contexts_;
            contexts_ = new context_list(this, (proto_context_list*)contexts_);
            delete oldctxs;
        }

        // keep defaults, simply add this context
        contexts_->add_context(c);

        has_preferences_ = false;
        is_default_ = false;
        preferences_.clear();
    }

    void session::remove_context (saga::context const& c)
    {
        if (!is_opened_)
        {
            SAGA_THROW("remove_context: session has been closed", 
                saga::IncorrectState)
            return;
        }

        mutex_type::scoped_lock lock(mtx_);
        engine_.late_initialize(this);

        if (NULL == contexts_) {
            SAGA_THROW("remove_context: context does not exist", 
                saga::DoesNotExist);
        }
        else if (contexts_->is_proto_context()) {
            base_context_list* oldctxs = contexts_;
            contexts_ = new context_list(this, (proto_context_list*)contexts_);
            delete oldctxs;
        }

        contexts_->remove_context(c);

        has_preferences_ = false;
        is_default_ = false;
        preferences_.clear();
    }

    void session::add_proto_context (
        std::vector<std::pair<std::string, std::string> > const& entries)
    {
        if (!is_opened_)
        {
            SAGA_THROW("add_proto_context: session has been closed", 
                saga::IncorrectState)
            return;
        }

        if (NULL == contexts_)
            contexts_ = new proto_context_list(this);

        if (!contexts_->is_proto_context()) {
            SAGA_THROW("Out of order execution of 'add_proto_context'. This "
                "should be called during adaptor construction only.",
                saga::NoSuccess);
        }
        contexts_->add_proto_context(entries);
    }

    void session::add_proto_context (
        std::vector<std::pair<std::string, std::string> > const& entries,
        std::vector<std::pair<std::string, std::vector<std::string> > > const& vector_entries)
    {
        if (!is_opened_)
        {
            SAGA_THROW("add_proto_context: session has been closed", 
                saga::IncorrectState)
            return;
        }

        if (NULL == contexts_)
            contexts_ = new proto_context_list(this);

        if (!contexts_->is_proto_context()) {
            SAGA_THROW("Out of order execution of 'add_proto_context'. This "
                "should be called during adaptor construction only.",
                saga::NoSuccess);
        }
        contexts_->add_proto_context(entries, vector_entries);
    }

    void session::release_contexts()
    {
        mutex_type::scoped_lock lock(mtx_);
        delete contexts_;
        contexts_ = NULL;

        has_preferences_ = false;
        is_default_ = false;
        preferences_.clear();
    }

    void session::close(double)
    {
        mutex_type::scoped_lock lock(mtx_);

        is_opened_ = false;
        release_contexts();
        engine_.tidy();
    }

    ///////////////////////////////////////////////////////////////////////////
    void session::save(boost::archive::text_oarchive& ar, const unsigned int version) const
    {
        if (!is_opened_)
        {
            SAGA_THROW("save: session has been closed", saga::IncorrectState)
            return;
        }

        mutex_type::scoped_lock lock(mtx_);
        engine_.late_initialize(this);

        if (NULL == contexts_) {
            contexts_ = new context_list(this);
        }
        else if (contexts_->is_proto_context()) {
            base_context_list* oldctxs = contexts_;
            contexts_ = new context_list(this, (proto_context_list*)contexts_);
            delete oldctxs;
        }

        std::vector<saga::context> const& ctxs (((context_list*)contexts_)->contexts_);

        using namespace boost::serialization;
        ar & make_nvp("is_default", is_default_);

        std::size_t size = ctxs.size();
        ar & make_nvp("listsize", size);

        std::vector<saga::context>::const_iterator end = ctxs.end();
        for (std::vector<saga::context>::const_iterator it = ctxs.begin();
             it != end; ++it)
        {
            ar & make_nvp("context", *it);
        }
    }

    void session::load(boost::archive::text_iarchive & ar, const unsigned int version)
    {
        mutex_type::scoped_lock lock(mtx_);
        engine_.late_initialize(this);

        if (NULL == contexts_) {
            contexts_ = new context_list(this);
        }
        else if (contexts_->is_proto_context()) {
            base_context_list* oldctxs = contexts_;
            contexts_ = new context_list(this);
            delete oldctxs;
        }

        // first delete all contexts, we need to do that as default constructed 
        // sessions are now 'default' sessions
        ((context_list*)contexts_)->remove_all_contexts();

        std::vector<saga::context>& ctxs (((context_list*)contexts_)->contexts_);

        using namespace boost::serialization;
        ar & make_nvp("is_default", is_default_);

        std::size_t size = 0;
        ar & make_nvp("listsize", size);

        for (std::size_t i = 0; i < size; ++i)
        {
            saga::context ctx;
            ar & make_nvp("context", ctx);
            ctxs.push_back(ctx);
        }
    }

    // fill context related preferences from this session instance
    v1_0::preference_type const& session::get_preferences() const
    {
        if (!is_opened_)
        {
            SAGA_THROW("get_preferences: session has been closed", 
                saga::IncorrectState)
            return preferences_;
        }


        if (!has_preferences_) {
            mutex_type::scoped_lock lock(mtx_);

            engine_.late_initialize(this);
            if (NULL != contexts_) 
                contexts_->fill_contexttypes(preferences_);

            has_preferences_ = true;
        }
        return preferences_;
    }

    ///////////////////////////////////////////////////////////////////////////
    // work item registration
    adaptors::workitem_cookie_handle session::register_workitem(
        adaptors::workitem_function f, adaptors::time_type const& expire_at)
    {
        if (!engine_.has_been_initialized()) 
        {
            mutex_type::scoped_lock lock(mtx_);
            engine_.late_initialize(this);
        }
        return workitems_.register_workitem(f, expire_at);
    }

    adaptors::workitem_cookie_handle session::register_workitem(
        adaptors::workitem_function f, 
        adaptors::duration_type const& expire_from_now)
    {
        if (!engine_.has_been_initialized()) 
        {
            mutex_type::scoped_lock lock(mtx_);
            engine_.late_initialize(this);
        }

        return workitems_.register_workitem(f, expire_from_now);
    }

    void session::unregister_workitem(adaptors::workitem_cookie_handle h)
    {
        if (!engine_.has_been_initialized()) 
        {
            mutex_type::scoped_lock lock(mtx_);
            engine_.late_initialize(this);
        }

        workitems_.unregister_workitem(h);
    }

    ///////////////////////////////////////////////////////////////////////
    // logging support
    bool session::module_needs_logging(char const* modulename) const
    {
        if (!engine_.has_been_initialized()) 
        {
            mutex_type::scoped_lock lock(mtx_);
            engine_.late_initialize(this);
        }

        return engine_.module_needs_logging(modulename);
    }
}} // namespace impl

