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

#include <stack>
#include <catch.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/core/utils/account_builder.hpp>


TEST_CASE("Account builds with valid inputs", "[account_builder]")
{
    const std::string valid_type("Member");
    const std::string valid_name("firstName,lastName");
    const std::string valid_address("123 Street,Portland,OR,97080");
    const std::string no_msg("");
    
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    Account_Builder builder(db);

    Building_Phases building_phase;


    SECTION("Build Member Account")
    {
        
        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.account_type);
        REQUIRE_NOTHROW(builder.set_current_field(valid_type));

        REQUIRE(builder.get_current_field() == building_phase.full_name);
        REQUIRE_NOTHROW(builder.set_current_field(valid_name));

        REQUIRE(builder.get_current_field() == building_phase.street_address);
        REQUIRE_NOTHROW(builder.set_current_field(valid_address));

    }
}
