
#include <iostream>
#include <sstream>
#include <set>

#include <boost/thread.hpp>

#include <saga/saga.hpp>

#define UMAX  1000
#define UBASE "file://localhost/tmp/data."

class U
{
  private:
    saga::url u_;

  public:
    U (saga::url u)
      : u_ (u)
    { }

    saga::url get (void) 
    {
      return u_;
    }
};

class A
{
  private:
    std::set <saga::url> uset_;

    int cnt_;

    saga::url create_url (int i)
    {
        std::stringstream ss;
        ss << UBASE << i;

        return saga::url (ss.str ());
    }

  public:
    A (void)
      : cnt_ (0)
    {
    }

    ~A (void)
    {
      std::cout  << cnt_ << std::endl;
    }

    void fill (void)
    {
      for ( int i = 0; i < UMAX; i++ )
      {
        U u (create_url (i));
        uset_.insert (u.get ());
        std::cout << "+" << std::flush;
        cnt_++;
      }
    }

    void check (void)
    {
      for ( int i = UMAX - 1; i >= 0; i-- )
      {
        U u (create_url (i));

        if ( uset_.find (u.get ()) != uset_.end () )
        {
          std::cout << "-" << std::flush;
          uset_.erase (u.get ());
          cnt_--;
        }
        else
        {
          std::cout << "." << std::flush;
        }
      }
    }
};

int main ()
{
  A a;

  boost::thread c (&A::check, &a);
  boost::thread f (&A::fill,  &a);

  f.join ();
  c.join ();

  return 0;
}

