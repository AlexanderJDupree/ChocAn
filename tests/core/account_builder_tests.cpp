/* 
File: account_builder_tests.cpp

Brief:  Account builder unit tests

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <cstring>
#include <stack>
#include <catch.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/core/utils/account_builder.hpp>

Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

TEST_CASE("Account builds with valid inputs", "[account_builder]")
{
    Account_Builder builder(db);

    SECTION("Account builder contructs with build phase set to Account_Type")
    {
        REQUIRE(std::holds_alternative(builder.get_current_field(),Account_Type));
    }

}
