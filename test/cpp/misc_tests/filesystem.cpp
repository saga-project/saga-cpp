//  Copyright (c) 2008 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/adaptors/utils/filesystem.hpp>

#include <string>

void test()
{
    std::string name;

    // change from: path.branch_path()
    //        from: path.parent_path()
    //        to  : parent_path(path)
    boost::filesystem::path
        path = name,
        path2 = path,
        path3 = "/some/path",
        path4 = boost::filesystem::parent_path(path),
        path5,
        path6 = system_complete(path);

    path = name;
    path2 = path;
    path3 = "/some/path";
    path4 = boost::filesystem::parent_path(path);
    path5 = path3 / path;
    path6 = system_complete(path);

    std::string(path.string());
    path.string().c_str();

    //  change from: path.leaf()
    //         from: path.filename()
    //         to  : filename(path)
    std::string leaf = boost::filesystem::filename(path);
    (bool)(boost::filesystem::filename(path) != boost::filesystem::filename(path3));

    (bool)(path.empty());
    (bool)(is_empty(path));
    (bool)(is_directory(path));

    typedef boost::system::error_code error_type;
    error_type error; // uninitialized

    boost::filesystem::file_status
        status = boost::filesystem::status(path),
        status2 = boost::filesystem::status(path, error);

    if (error)
        /**/;

    (void)status2;

    (bool)(status_known(status));
    (bool)(exists(status));
    (bool)(is_directory(status));

    // change from: is_regular(status)
    //        to  : is_regular_file(status)
    (bool)(is_regular_file(status));
}

int main()
{
    typedef void (*test_ptr_type)();
    test_ptr_type test_ptr = &test;
    (void)test_ptr;
}
