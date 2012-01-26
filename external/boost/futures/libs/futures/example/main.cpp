#include <iostream>

#include <boost/bind.hpp>
#include <boost/futures.hpp>
#include <boost/spirit/fusion/sequence/tuple.hpp>
#include <boost/variant.hpp>

using namespace std;
using namespace boost::fusion;
using namespace boost::futures;
using namespace boost;

///////////////////////////////////////////////////////////////////////////////
//
//  fac function for testing 
//
///////////////////////////////////////////////////////////////////////////////

int fac(int n){
  if(n == 0)
    return 0;
  if(n == 1)
    return 1;
//  sleep(1);
  return n * fac(n - 1);
}

int fac4()
{
    return fac(4);
}

int fac6()
{
    return fac(6);
}

double fac8()
{
    return (double)fac(8);
}

struct fac_functor
{
    fac_functor(int n) : n(n) {}
    
    int operator()()
    {
        return fac(n);
    }
    
    int n;
};

int main(int argc, char **argv){
  simple_future<int> f1    = bind(fac, 4);
  simple_future<double> f2 = fac8;
  simple_future<int> f3    = fac_functor(6);

  future<int> f = int_[&fac4] || int_[&fac6];

  future<tuple<int, double, int> > f_and = f1 && f2 && f3;
  variant<int, double> v                 = (f1 || f2 || f3)(); 
  //variant<int, double> v                 = futurize(f1 || f2 || f3); 
  cout << (f1 || f2 || f3)() << endl;
  if(v.which() == 0)
    cout << boost::get<int>(v) << endl;
  else
    cout << boost::get<double>(v) << endl;

  tuple<int, double, int> t = f_and();
      
  cout << get<0>(t) << endl;
  cout << get<1>(t) << endl;
  cout << get<2>(t) << endl;
}
