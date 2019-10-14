/* 

File: exception_tests.cpp

Brief: Unit tests for chocAn exception interfaces

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
    test_exception(const char* error, info exception_info)
        : error(error), exception_info(exception_info) {}

    const char* what() const throw()
    {
        return error;
    }

    const info& get_info() const
    {
        return exception_info;
    }

    const char* error;
    info exception_info;
};

void throw_test_exception(test_exception err)
{
    throw err;
}

TEST_CASE("chocan_user_exception", "[exception]")
{

    chocan_user_exception::info expected_values { "x should be y", "a should be b" };

    test_exception test("error msg", expected_values);
    
    SECTION("Exceptions are can be caught when thrown")
    {
        REQUIRE_THROWS_AS(throw_test_exception(test), test_exception);
    }

    SECTION("Accessing exception error message")
    {
        try
        {
            throw_test_exception(test);
        }
        catch (const test_exception& err)
        {
            REQUIRE(err.what() == std::string("error msg"));
            REQUIRE(err.get_info() == expected_values);
        }
    }
}

