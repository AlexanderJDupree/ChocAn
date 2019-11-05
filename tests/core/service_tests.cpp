/* 

File: service_tests.cpp

Brief: Unit tests for chocan_service base class

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/core/chocan_service.hpp>


TEST_CASE("Creating ChocAn_Service Objects", "[constructors], [service]")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    SECTION("ChocAn_Services are constructed with a valid Database pointer")
    {
        REQUIRE_NOTHROW(ChocAn_Service(db));
    }
    SECTION("ChocAn_Service throws if the databse pointer is null")
    {
        REQUIRE_THROWS(ChocAn_Service(nullptr));
    }
}
