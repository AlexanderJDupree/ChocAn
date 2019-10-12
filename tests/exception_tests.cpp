/* 

File: exception_tests.cpp

Brief: Unit tests for DateTime class and associated utility classes.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/exception.hpp>

class test_exception : public chocan_user_exception
{
public:
    test_exception(const char* error, Info exception_info)
        : error(error), exception_info(exception_info) {}

    const char* what() const throw()
    {
        return error;
    }

    const Info& get_info() const
    {
        return exception_info;
    }

    const char* error;
    Info exception_info;
};

void throw_test_exception(test_exception err)
{
    throw err;
}

// TODO Refactor, this test is ugly
TEST_CASE("chocan_user_exception", "[exception]")
{
    chocan_user_exception::Info exception_info;

    exception_info["user input"] = "expected value";
    exception_info["bad field"] = "how to fix bad field";

    test_exception test("error msg", exception_info);

    SECTION("Accessing exception error message")
    {

        try
        {
            throw_test_exception(test);
        }
        catch (const chocan_user_exception& err)
        {
            REQUIRE(err.what() == std::string("error msg"));
            REQUIRE(err.get_info() == exception_info);
        }
    }

}
