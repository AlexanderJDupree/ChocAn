/* 

File: login_tests.cpp

Brief: Unit tests for login_manager

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/core/chocan.hpp>
#include <ChocAn/data/mock_db.hpp>

TEST_CASE("Using the Login_Manager", "[login], [service]")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    // Create main chocan service
    ChocAn chocan(db);

    SECTION("Login returns true when given a valid ID")
    {
        REQUIRE(chocan.login_manager.login(1234));
    }
    SECTION("Login returns false when given an invlalid ID")
    {
        REQUIRE_FALSE(chocan.login_manager.login(0));
    }
    SECTION("Session owner exists if logged in")
    {
        chocan.login_manager.login(1234);

        REQUIRE(chocan.login_manager.session_owner() != nullptr);
    }
    SECTION("Session owner matches the Account in the DB")
    {
        chocan.login_manager.login(1234);

        REQUIRE(*(chocan.login_manager.session_owner()) == Mock_DB::account_table.at(1234));
    }
    SECTION("Session owner does not exist after user is logged out")
    {
        chocan.login_manager.login(5678);
        chocan.login_manager.logout();

        REQUIRE(chocan.login_manager.session_owner() == nullptr);
    }
}