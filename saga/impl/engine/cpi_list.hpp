//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_CPI_LIST_HPP
#define SAGA_IMPL_ENGINE_CPI_LIST_HPP

#include <saga/impl/engine/cpi.hpp>
#include <saga/saga/util.hpp>

#include <boost/assert.hpp>
#include <list>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

  class cpi_list 
  {
  public:
    typedef TR1::shared_ptr<v1_0::cpi> cpi_type;
    typedef std::list<cpi_type> cpi_list_type;
    typedef cpi_list_type::iterator iterator;
    typedef cpi_list_type::const_iterator const_iterator;
    typedef cpi_list_type::size_type size_type;
    typedef cpi_list_type::value_type value_type;
    
    cpi_list() {}
    ~cpi_list() {}

    //  do we have at least one cpi instance cached?
    bool empty() const { return cpis_.empty(); }
    size_type size() const { return cpis_.size(); }
    
    // return the currently selected cpi instance
    value_type get_current() const 
    { 
      BOOST_ASSERT(!empty());
      return cpis_.front();
    }
    
    //  release the current cpi instance
    void release_current() 
    {
      BOOST_ASSERT(!empty());
      cpis_.erase(cpis_.begin());
    }

    // splice the given cpi instance to the front of the list
    void make_current(iterator it)
    {
      if (it != cpis_.begin())
          cpis_.splice(cpis_.begin(), cpis_, it);
    }
    
    // add a new cpi instance to the list
    void add_new(v1_0::cpi* cpi)
    {
      cpis_.push_front(cpi_type(cpi));
    }
    
    // free all elements
    void clear()
    {
      cpis_.clear();
    }
    
    // iterator access
    iterator begin() { return cpis_.begin(); }
    iterator end() { return cpis_.end(); }
    const_iterator begin() const { return cpis_.begin(); }
    const_iterator end() const { return cpis_.end(); }
    
  private:
    cpi_list_type cpis_;
  };

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

#endif  // !SAGA_IMPL_ENGINE_CPI_LIST_HPP
