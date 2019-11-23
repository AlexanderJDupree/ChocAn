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
    SECTION("Build Manager Account")
    {

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
    SECTION("Build Provider Account")
    {

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
TEST_CASE("Handles invalid user inputs without breaking build process", "[account_builder]")
{
    //this test case is structured as a user interaction scenario in which the user
    //simple cannot enter the correct inputs. the sections are nested to ensure that 
    //the actual run time process of building an account is simulated accuratly, otherwise
    //the account builder would have to start over for every build phase and that is not how
    //it will operate in production

    Account_Builder builder(db);

    SECTION("Invalid Account Type Inputs")
    {

        //Inputs teseted
        const std::string does_not_name_a_type("not and account type");
        const std::string null_string("");

        //begin in account type phase 
        REQUIRE(builder.get_current_issues().error_msg == no_msg);
        REQUIRE(builder.get_current_issues().info().empty());
        REQUIRE(builder.get_current_field() == building_phase.account_type);

        //reject invalid input
        REQUIRE_NOTHROW(builder.set_current_field(does_not_name_a_type));
        REQUIRE(strcmp(builder.get_current_issues().error_msg, building_phase.invalid_type_msg) == 0);
        REQUIRE(builder.get_current_field() == building_phase.account_type);

        //reject invalid input
        REQUIRE_NOTHROW(builder.set_current_field(null_string));
        REQUIRE(strcmp(builder.get_current_issues().error_msg, building_phase.invalid_type_msg) == 0);
        REQUIRE(builder.get_current_field() == building_phase.account_type);

        //accept valid input
        REQUIRE_NOTHROW(builder.set_current_field(valid_type));

        //Transition to name field
        SECTION("Invalid Name Inputs")
        {
            //Inputs teseted
            const std::string no_first(",lastName");
            const std::string no_last("first,");
            const std::string name_no_comma("first last");
            const std::string name_to_long("asddddddddddfasdfasdfasdfasdfasdf,asfdasdfasdfasdfsdffasdfasdfasdf");
            
            //for access to name class specific errors
            invalid_name name_errors("", {});

            //no issues present at begining of full name build phase
            REQUIRE(builder.get_current_issues().error_msg == no_msg);
            REQUIRE(builder.get_current_issues().info().empty());
            REQUIRE(builder.get_current_field() == building_phase.full_name);

            //reject invalid input
            REQUIRE_NOTHROW(builder.set_current_field(no_first));
            REQUIRE(builder.get_current_issues().info().at(0) == name_errors.first_name_out_of_range);
            REQUIRE(builder.get_current_field() == building_phase.full_name);

            //reject invalid input
            REQUIRE_NOTHROW(builder.set_current_field(no_last));
            REQUIRE(builder.get_current_issues().info().at(0) == name_errors.last_name_out_of_range);
            REQUIRE(builder.get_current_field() == building_phase.full_name);

            //reject invalid input
            REQUIRE_NOTHROW(builder.set_current_field(name_no_comma));
            REQUIRE(builder.get_current_issues().info().at(0) == name_errors.full_name_out_of_range);
            REQUIRE(builder.get_current_field() == building_phase.full_name);

            //reject invalid input
            REQUIRE_NOTHROW(builder.set_current_field(name_to_long));
            REQUIRE(builder.get_current_issues().info().at(0) == name_errors.full_name_out_of_range);
            REQUIRE(builder.get_current_field() == building_phase.full_name);

            //accept valid input
            REQUIRE_NOTHROW(builder.set_current_field(valid_name));
            //Transition to address field
            SECTION("Invalid Address Inputs")
            {
                //Inputs teseted
                const std::string no_street(",city,OR,93939");
                const std::string no_city("street,,OR,93939");
                const std::string no_state("street,city,,93939");
                const std::string no_zip("street,city,OR,");
                const std::string non_numeric_zip("street,city,OR,potatoe");
                const std::string no_commas("street city OR 88483");
                const std::string not_enough_commas("street city, OR, 88483");
                const std::string to_many_commas("street, ,city, OR, 88483");

                //for access to address specific error messages
                invalid_address error_msg("", {});

                //no issues present at begining of street address build phase
                REQUIRE(builder.get_current_issues().error_msg == no_msg);
                REQUIRE(builder.get_current_issues().info().empty());
                REQUIRE(builder.get_current_field() == building_phase.street_address);

                //reject invalid input
                REQUIRE_NOTHROW(builder.set_current_field(no_street));
                REQUIRE(builder.get_current_issues().info().at(0) == error_msg.street_range);
                REQUIRE(builder.get_current_field() == building_phase.street_address);

                //reject invalid input
                REQUIRE_NOTHROW(builder.set_current_field(no_city));
                REQUIRE(builder.get_current_issues().info().at(0) == error_msg.city_range);
                REQUIRE(builder.get_current_field() == building_phase.street_address);

                //reject invalid input
                REQUIRE_NOTHROW(builder.set_current_field(no_state));
                REQUIRE(builder.get_current_issues().info().at(0) == error_msg.state_range);
                REQUIRE(builder.get_current_field() == building_phase.street_address);

                //reject invalid input
                REQUIRE_NOTHROW(builder.set_current_field(no_zip));
                REQUIRE(builder.get_current_issues().info().at(0) == error_msg.zip_range);
                REQUIRE(builder.get_current_field() == building_phase.street_address);

                //reject invalid input
                REQUIRE_NOTHROW(builder.set_current_field(non_numeric_zip));
                REQUIRE(builder.get_current_issues().info().at(0) == error_msg.zip_range);
                REQUIRE(builder.get_current_field() == building_phase.street_address);

                //reject invalid input
                REQUIRE_NOTHROW(builder.set_current_field(no_commas));
                REQUIRE_FALSE(builder.get_current_issues().info().empty());
                REQUIRE(builder.get_current_field() == building_phase.street_address);

                //reject invalid input
                REQUIRE_NOTHROW(builder.set_current_field(to_many_commas));
                REQUIRE_FALSE(builder.get_current_issues().info().empty());
                REQUIRE(builder.get_current_field() == building_phase.street_address);

                //accepts valid input even if it has an extra comma at the end
                SECTION("Valid input should not be affected by trailing commas")
                {

                    REQUIRE_NOTHROW(builder.set_current_field(valid_address + ","));
                    REQUIRE(builder.get_current_issues().info().empty());
                    REQUIRE(builder.get_current_field() == builder.review_account());
                }

                //accepts valid input
                REQUIRE_NOTHROW(builder.set_current_field(valid_address));

                //Transition to account review
                SECTION("Review account")
                {

                    //no issues are present when reviewing account
                    REQUIRE(builder.get_current_issues().error_msg == no_msg);
                    REQUIRE(builder.get_current_issues().info().empty());
                    REQUIRE(builder.get_current_field() == builder.review_account());

                    SECTION("Accept account")
                    {
                        REQUIRE_NOTHROW(builder.set_current_field("Yes"));
                        REQUIRE(builder.buildable());
                    }
                    SECTION("Reject account")
                    {

                        REQUIRE_NOTHROW(builder.set_current_field("No"));
                        REQUIRE(strcmp(builder.get_current_issues().error_msg, building_phase.account_rejected) == 0);
                        REQUIRE_FALSE(builder.buildable());
                    }
                    SECTION("Any input other than (Y)es/(N)o"){

                        REQUIRE_NOTHROW(builder.set_current_field("gobble de gook"));
                        REQUIRE(strcmp(builder.get_current_issues().error_msg, building_phase.account_rejected) == 0);
                        REQUIRE_FALSE(builder.buildable());
                    }
                }
            }
        }
    }
}
TEST_CASE("Exceptions thrown","[account_builder][exceptions]")
{
    //throws only one exception when build is called prematurely

    Account_Builder builder(db);

    SECTION("Build called while unbuildable"){

        REQUIRE_THROWS_AS(builder.build(),invalid_account_build); 
    }
    
}