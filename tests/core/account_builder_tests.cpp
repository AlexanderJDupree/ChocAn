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


//will need to be updated if changes are made to the address, name, or account_builder error messages
struct mock_errors{

        const std::string get_street_error()const{ return "Street address must be less than 25 characters";};
        const std::string get_city_error()const{ return "Cities must be less than 14 character";};
        const std::string get_state_error(const std::string st)const{ return "State must be in abbreviated 2 character format\n \"" + st + " \" is not a U.S. state";};
        const std::string get_zip_error()const{return "Zip code must be 5 digit number";};
        const std::string get_invalid_address(){ return "Invalid address values";};
        const std::string get_invalid_first(){ return "First Name must be 1 to 24 characters long";};
        const std::string get_invalid_last(){return "Last Name must be 1 to 24 characters long";};
        const std::string get_invalid_full(){ return "Full Name must be 1 to 25 characters long";};
        const std::string get_invalid_name(){ return "Invalid name length";};
        const std::string get_build_premature(){ return "Attempt made to build prematurely";};
        const std::string get_build_failed(){ return "Failed to build account";};
        const std::string get_build_(){ return "Issues with account";};
};

Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();
mock_errors error;
 
TEST_CASE("Account builds with valid inputs", "[account_builder]")
{
    Account_Builder builder(db);

    SECTION("Account builder contructs")
    {
    }

}
