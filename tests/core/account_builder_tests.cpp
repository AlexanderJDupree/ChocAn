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


//will need to be updated if changes are made to the address, name, or account_builder error messages

Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

TEST_CASE("Account builder can build new accounts", "[account_builder]")
{
    mock_dependencies mocks;
    Account_Builder account_builder;
    std::string input;

    SECTION("Builds an account with valid input", "[account_builder][use_case][happy_path]")
    {
        account_builder.initiate_new_build_process();
        SECTION("Builder initiates to Type state when initiate_new_build_process() is called","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::Type>(account_builder.builder_state()));
        }

        input = "member";
        account_builder.set_field(input);
        SECTION("Builder transition to First state upon valid account type input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::First>(account_builder.builder_state()));
        }
        
        input = "first";
        account_builder.set_field(input);
        SECTION("Builder transition to Last state upon valid first name input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::Last>(account_builder.builder_state()));
        }
        
        input = "last";
        account_builder.set_field(input);
        SECTION("Builder transition to Street state upon valid last name input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::Street>(account_builder.builder_state()));
        }
        
        input = "street";
        account_builder.set_field(input);
        SECTION("Builder transition to City state upon valid street input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::City>(account_builder.builder_state()));
        }
    
        input = "city";
        account_builder.set_field(input);
        SECTION("Builder transition to State state upon valid city input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::State>(account_builder.builder_state()));
        }
        
        input = "OR";
        account_builder.set_field(input);
        SECTION("Builder transition to Zip state upon valid state input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(std::holds_alternative<Account_Builder::Zip>(account_builder.builder_state()));
        }
    
        input = "97080";
        account_builder.set_field(input);
        SECTION("Builder is in a buildable state after recieving valid zip input","[account_builder][use_case][happy_path]")
        {
            REQUIRE(account_builder.buildable());
        }
        
        REQUIRE_NOTHROW(Account(account_builder.build_new_account(db)));
    
    }
    
    SECTION("Builds an account despite persitantly bad user input", "[account_builder][use_case][user_errors]")
    {
        mock_dependencies mocks;
        Account_Builder account_builder;
        std::stack<std::string> good_input("90808",
                                           "OR",
                                           "Portland",
                                           "1123 cool street",
                                           "last name", 
                                           "first name", 
                                           "provider");
        std::vector<std::string> bad_inputs("not an account",
                                            "134053480598350",
                                            "////***/*//*/*/*/++6+66+21jj464``",
                                            "!@#$%^&*()_+/*/*/",
                                            std::string(1000,'*'),
                                            "some more nonsense");
    
        


    }
}
