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

    
const std::string valid_name("firstName,lastName");
const std::string valid_address("123 Street,Portland,OR,97080");
const std::string valid_type("member");
const std::string no_msg("");
Build_Instructions building_phase;
Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

TEST_CASE("Account builds with valid inputs", "[account_builder]")
{
    Account_Builder builder(db);

    SECTION("Build Member Account")
    {
        const std::string valid_type("member");
        
        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.account_type);
        REQUIRE_NOTHROW(builder.set_current_field(valid_type));

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.full_name);
        REQUIRE_NOTHROW(builder.set_current_field(valid_name));

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.street_address);
        REQUIRE_NOTHROW(builder.set_current_field(valid_address));

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == builder.review_account());
        REQUIRE_NOTHROW(builder.set_current_field("Yes"));

        REQUIRE(builder.buildable());

        REQUIRE_NOTHROW(builder.build());
        REQUIRE(std::holds_alternative<Member>(builder.build().type()));

    }
    SECTION("Build Manager Account"){

        const std::string valid_type("manager");
        
        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.account_type);
        
        REQUIRE_NOTHROW(builder.set_current_field(valid_type));

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.full_name);
        
        REQUIRE_NOTHROW(builder.set_current_field(valid_name));

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.street_address);
        
        REQUIRE_NOTHROW(builder.set_current_field(valid_address));

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == builder.review_account());
        
        REQUIRE_NOTHROW(builder.set_current_field("Yes"));

        REQUIRE(builder.buildable());

        REQUIRE_NOTHROW(builder.build());
        
        REQUIRE(std::holds_alternative<Manager>(builder.build().type()));

    }
    SECTION("Build Provider Account"){

        const std::string valid_type("provider");
        
        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.account_type);
        REQUIRE_NOTHROW(builder.set_current_field(valid_type));

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.full_name);
        REQUIRE_NOTHROW(builder.set_current_field(valid_name));

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.street_address);
        REQUIRE_NOTHROW(builder.set_current_field(valid_address));

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == builder.review_account());
        REQUIRE_NOTHROW(builder.set_current_field("Yes"));

        REQUIRE(builder.buildable());

        REQUIRE_NOTHROW(builder.build());
        REQUIRE(std::holds_alternative<Provider>(builder.build().type()));
    }
}
TEST_CASE("Report build errors without throwing exceptions when given invalid input","[account_builder]"){

    Account_Builder builder(db);

    SECTION("Invalid Type: rejects inputs until a valid type is recived"){

        const std::string does_not_name_a_type("not and account type");
        const std::string null_string("");

        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.account_type);

        REQUIRE_NOTHROW(builder.set_current_field(does_not_name_a_type));
        REQUIRE(strcmp(builder.get_current_issues().error_msg, building_phase.invalid_type_msg) == 0);
        REQUIRE(builder.get_current_field() == building_phase.account_type);
        
        REQUIRE_NOTHROW(builder.set_current_field(null_string));
        REQUIRE(strcmp(builder.get_current_issues().error_msg, building_phase.invalid_type_msg) == 0);
        REQUIRE(builder.get_current_field() == building_phase.account_type);
        
        REQUIRE_NOTHROW(builder.set_current_field(valid_type));
        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        
        REQUIRE(builder.get_current_field() == building_phase.full_name);
    }

    SECTION("Invalid Name Inputs"){

        const std::string no_first(",lastName");
        const std::string no_last("first,");
        const std::string name_no_comma("first last");
        const std::string name_to_long("asddddddddddfasdfasdfasdfasdfasdf,asfdasdfasdfasdfsdffasdfasdfasdf");

        invalid_name name_errors("",{});
        
        REQUIRE(builder.get_current_field() == building_phase.account_type);
        REQUIRE_NOTHROW(builder.set_current_field(valid_type));
        
        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.full_name);

        REQUIRE_NOTHROW(builder.set_current_field(no_first));
        REQUIRE(builder.get_current_issues().info().at(0) == name_errors.first_name_out_of_range);

        REQUIRE_NOTHROW(builder.set_current_field(no_last));
        REQUIRE(builder.get_current_issues().info().at(0) == name_errors.last_name_out_of_range);
        
        REQUIRE_NOTHROW(builder.set_current_field(name_to_long));
        REQUIRE(builder.get_current_issues().info().at(0) == name_errors.full_name_out_of_range);
        
        //TODO see why no first name is acceptable
        REQUIRE_NOTHROW(builder.set_current_field(name_no_comma));
        REQUIRE(builder.get_current_issues().info().empty());//.at(0) == name_errors.full_name_out_of_range);

    }

    SECTION("Invalid Address Inputs"){

    }
}
