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

#include <catch.hpp>
#include <ChocAn/core/chocan.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/app/state_viewer.hpp>
#include <ChocAn/app/state_controller.hpp>
#include <ChocAn/view/terminal_input_controller.hpp>
#include <ChocAn/core/utils/account_builder.hpp>
#include <ChocAn/core/entities/name.hpp>
#include <ChocAn/core/entities/address.hpp>
#include <sstream>

class mock_state_viewer : public State_Viewer
{
    public:

        void update()
        {
            handler();
        }

        void render_state(const Application_State&, Callback event)
        {
            handler = event;

            update();
        }

        Callback handler;
};

class mock_dependencies
{
    public:
        std::stringstream in_stream;

        Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();
        ChocAn::ChocAn_Ptr chocan     = std::make_unique<ChocAn>(db);

        State_Viewer::State_Viewer_Ptr state_viewer = std::make_unique<mock_state_viewer>();

        Input_Controller::Input_Control_Ptr input_controller = std::make_unique<Terminal_Input_Controller>(in_stream);
};

struct valid_inputs{

    valid_inputs() : type("Member")
                    ,first("first")
                    ,last("last")
                    ,street("1010 cool street")
                    ,city("Portland")
                    ,state("OR")
                    ,zip(97080)
                    ,name(first,last)
                    ,address(street,city,state,zip) 
                    ,account_type(Member())
                    {};

    
    const std::string type;
    const std::string first;
    const std::string last;
    const std::string street;
    const std::string city;
    const std::string state;
    const unsigned zip;
    const Name name;
    const Address address;
    Account::Account_Type account_type;
};

struct updates{

    updates() : new_first("bob")
              , new_last("davidson")
              , new_street("cool street")
              , new_city("portland")
              , new_state("AK")
              , new_zip(97313)
              {};

    const std::string new_first;
    const std::string new_last;
    const std::string new_street;
    const std::string new_city;
    const std::string new_state;
    const unsigned new_zip;
};

const std::vector<std::string> bad_inputs({"charactor limit grosssly exceeeedd by thiss verrry longg string of charactersssfdsafadsf",
                                            std::string(1000,'*'),
                                            std::string(1000,'0')});

Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();
valid_inputs good_input;
updates updated;
mock_dependencies mocks;
Account_Builder account_builder;

TEST_CASE("Account builder can build new accounts", "[account_builder]")
{

    SECTION("Builds an account with valid input", "[account_builder][use_case][happy_path]")
    {
        account_builder.initiate_new_build_process();
        
        SECTION("Builder initiates to Type state when initiate_new_build_process() is called","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::Type>(account_builder.builder_state()));
        }

        account_builder.set_field(good_input.type);
        
        SECTION("Builder transition to First state upon valid account type input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::First>(account_builder.builder_state()));
        }
        
        account_builder.set_field(good_input.first);
        
        SECTION("Builder transition to Last state upon valid first name input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::Last>(account_builder.builder_state()));
        }
        
        account_builder.set_field(good_input.last);
        
        SECTION("Builder transition to Street state upon valid last name input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::Street>(account_builder.builder_state()));
        }
        
        account_builder.set_field(good_input.street);
        
        SECTION("Builder transition to City state upon valid street input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::City>(account_builder.builder_state()));
        }
    
        account_builder.set_field(good_input.city);
        
        SECTION("Builder transition to State state upon valid city input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::State>(account_builder.builder_state()));
        }
        
        account_builder.set_field(good_input.state);
        
        SECTION("Builder transition to Zip state upon valid state input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::Zip>(account_builder.builder_state()));
        }
    
        account_builder.set_field(std::to_string(good_input.zip));
        
        SECTION("Builder is in a buildable state after recieving valid zip input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(account_builder.buildable());
        }
        
        REQUIRE_NOTHROW(Account(account_builder.build_new_account(db)));
    
    }
    
    SECTION("Builds an account despite persitantly bad user input", "[account_builder][user_errors]")
    {
        
        const Account expected_account(Name(good_input.first,good_input.last)
                                ,Address(good_input.street,good_input.city,good_input.state,good_input.zip)
                                ,good_input.account_type
                                ,mocks.chocan->id_generator);

        account_builder.initiate_new_build_process();

        for (int i = 0; i < 7; ++i)
        {
            for (const std::string &bad_input : bad_inputs)
            {
                account_builder.set_field(bad_input);
            }

            switch (i)
            {
            case 0:
                account_builder.set_field(good_input.type);
                break;
            case 1:
                account_builder.set_field(good_input.first);
                break;
            case 2:
                account_builder.set_field(good_input.last);
                break;
            case 3:
                account_builder.set_field(good_input.street);
                break;
            case 4:
                account_builder.set_field(good_input.city);
                break;
            case 5:
                account_builder.set_field(good_input.state);
                break;
            case 6:
                account_builder.set_field(std::to_string(good_input.zip));
                break;
            default:
                break;
            }
        }

        const Account account_built(account_builder.build_new_account(mocks.chocan->id_generator));

        REQUIRE(account_built.name() == expected_account.name());

        REQUIRE(account_built.address() == expected_account.address());

    }
}
TEST_CASE("Account builder updates accounts as needed", "[account_builder],[update_account]")
{
    Account account_to_update(good_input.name,good_input.address,good_input.account_type,mocks.chocan->id_generator);

    Account_Builder::Build_Stack updates_needed({Account_Builder::Zip()
                                                ,Account_Builder::State()
                                                ,Account_Builder::City()
                                                ,Account_Builder::Street()
                                                ,Account_Builder::Last()
                                                ,Account_Builder::First()
                                                });

    SECTION("Account builder updates the first name as needed","[account_builder],[update_account],[first_name]"){

        REQUIRE(account_to_update.name().first() != updated.new_first);

        account_builder.request_update_to_account(updates_needed.top());
        account_builder.set_field(updated.new_first);
        account_builder.apply_updates_to_account(account_to_update);

        REQUIRE(account_to_update.name().first() == updated.new_first);
    }
    
    updates_needed.pop();

    SECTION("Account builder updates the last name as needed","[account_builder],[update_account],[last_name]"){

        REQUIRE(account_to_update.name().last() != updated.new_last);

        account_builder.request_update_to_account(updates_needed.top());
        account_builder.set_field(updated.new_last);
        account_builder.apply_updates_to_account(account_to_update);

        REQUIRE(account_to_update.name().last() == updated.new_last);
    }
    
    updates_needed.pop();

    SECTION("Account builder updates the street as needed","[account_builder],[update_account],[street]"){

        REQUIRE(account_to_update.address().street() != updated.new_street);

        account_builder.request_update_to_account(updates_needed.top());
        account_builder.set_field(updated.new_street);
        account_builder.apply_updates_to_account(account_to_update);

        REQUIRE(account_to_update.address().street() == updated.new_street);
    }

    updates_needed.pop();

    SECTION("Account builder updates the city as needed","[account_builder],[update_account],[city]"){

        REQUIRE(account_to_update.address().city() != updated.new_city);

        account_builder.request_update_to_account(updates_needed.top());
        account_builder.set_field(updated.new_city);
        account_builder.apply_updates_to_account(account_to_update);

        REQUIRE(account_to_update.address().city() == updated.new_city);
    }
    
    updates_needed.pop();

    SECTION("Account builder updates the state as needed","[account_builder],[update_account],[state]"){

        REQUIRE(account_to_update.address().state() != updated.new_state);

        account_builder.request_update_to_account(updates_needed.top());
        account_builder.set_field(updated.new_state);
        account_builder.apply_updates_to_account(account_to_update);

        REQUIRE(account_to_update.address().state() == updated.new_state);
    }
    
    updates_needed.pop();

    SECTION("Account builder updates the zip as needed","[account_builder],[update_account],[zip]"){

        REQUIRE(account_to_update.address().zip() != updated.new_zip);

        account_builder.request_update_to_account(updates_needed.top());
        account_builder.set_field(std::to_string(updated.new_zip));
        account_builder.apply_updates_to_account(account_to_update);

        REQUIRE(account_to_update.address().zip() == updated.new_zip);
    }

}
TEST_CASE("Account builder throws exceptions when needed", "[account_builder],[exceptions]")
{

}
