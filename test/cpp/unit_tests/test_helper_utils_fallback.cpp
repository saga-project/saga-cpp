#include "test_helper_utils_fallback.hpp"

test_helper_utils_fallback::test_helper_utils_fallback(
                    std::string scheme,
                    std::string host)
{
    scheme_ = scheme;
    host_   = host;
    std::cerr << "[FALLBACK] Set scheme for fallback as " << scheme_ << std::endl;
    std::cerr << "[FALLBACK] Set host for fallback as " << host_ << std::endl;
}

test_helper_utils_fallback::~test_helper_utils_fallback()
{
    //does nothing now
}

saga::url test_helper_utils_fallback::create_temp_file_name(bool create=true)
{
    int mode = saga::filesystem::Create;
    std::string unique_path = get_unique_path_name();
    saga::url unique_url(unique_path);
    unique_url.set_scheme(scheme_);
    unique_url.set_host(host_);

    if(create)
    {
       saga::filesystem::file file(unique_url, mode);
       if(file.is_entry() == false)
       {
           std::cerr << "Could't create file " << unique_path << std::endl;
       }
    }
    return unique_url;
}

saga::url test_helper_utils_fallback::create_temp_dir_name(bool create=true)
{
    int mode = saga::filesystem::CreateParents |
               saga::filesystem::Create;
    std::string unique_path = get_unique_path_name();
    saga::url unique_url(unique_path);
    unique_url.set_scheme(scheme_);
    unique_url.set_host(host_);

    if(create)
    {
        saga::filesystem::directory d(unique_url, mode);
        if(d.is_dir() == false)
        {
            std::cerr << "Could't create directory " << unique_path << std::endl;
        }
    }
    return saga::url();
}

saga::url test_helper_utils_fallback::create_temp_file_for_exception(const saga::error &e)
{
   int mode = saga::filesystem::Create;
   std::string unique_path = get_unique_path_name();
   saga::url unique_url(unique_path);
   switch(e)
   {
       case saga::adaptors::Success:
       {
           unique_url.set_scheme(scheme_);
           unique_url.set_host(host_);
           std::cerr << "[FALLBACK] Create file " << unique_url.get_string() << std::endl;
           saga::filesystem::file f(unique_url, mode);
           std::cerr << "[FALLBACK]" << std::endl;
       }
       break;
       case saga::IncorrectState:
       {
           unique_url.set_scheme(scheme_);
           unique_url.set_host(host_);
           saga::filesystem::file f(unique_url, mode);
       }
       break;
       case saga::BadParameter:
       {
           unique_url.set_scheme(scheme_);
           unique_url.set_host(host_);
           saga::filesystem::file f(unique_url, mode);
       }
       break;
       case saga::DoesNotExist:
       {
           unique_url.set_scheme(scheme_);
           unique_url.set_host(host_);
       }
       break;
       case saga::IncorrectURL:
       {
           unique_url = saga::url("fallback:remote:-host/~.../");
       }
       break;
       case saga::PermissionDenied:
       {
           unique_url = saga::url("");
       }
       break;
       default:
       {
           unique_url = saga::url("");
       }
       break;
   }
   return unique_url;
}

saga::url test_helper_utils_fallback::create_temp_dir_for_exception(const saga::error &e)
{
   int mode = saga::filesystem::Create;
   std::string unique_path = get_unique_path_name();
   saga::url unique_url(unique_path);

   switch(e)
   {
       case saga::adaptors::Success:
       {
           unique_url.set_scheme(scheme_);
           unique_url.set_host(host_);
           saga::filesystem::directory d(unique_url, mode);
       }
       break;
       case saga::IncorrectState:
       {
           unique_url.set_scheme(scheme_);
           unique_url.set_host(host_);
           saga::filesystem::directory d(unique_url, mode);
       }
       break;
       case saga::BadParameter:
       {
           unique_url.set_scheme(scheme_);
           unique_url.set_host(host_);
           saga::filesystem::directory d(unique_url, mode);
       }
       break;
       case saga::DoesNotExist:
       {
           unique_url.set_scheme(scheme_);
           unique_url.set_host(host_);
       }
       break;
       case saga::IncorrectURL:
       {
           unique_url = saga::url("fallback:remote:-host/~.../");
       }
       break;
       case saga::PermissionDenied:
       {
       }
       break;
       default:
       {
           unique_url = saga::url("");
       }
       break;
   }
   return unique_url;
}

void test_helper_utils_fallback::delete_temp_file(saga::url url)
{
    //Left blank for now
}

void test_helper_utils_fallback::delete_temp_dir(saga::url url)
{
    //Left blank for now
}
