
#ifndef SAGA_UTIL_SINGLETON
#define SAGA_UTIL_SINGLETON

#include <boost/spirit/core/non_terminal/impl/static.hpp>

namespace saga
{
  namespace adaptors
  {
    namespace utils
    {
      struct singleton_tag {};

      // avoid to be optimized away
      template <class Singleton>
      Singleton & get_singleton (void)
      {
        boost::spirit::static_ <Singleton, singleton_tag> singleton_;
        return singleton_.get ();
      }
    }
  }
}

#endif // SAGA_UTIL_SINGLETON

