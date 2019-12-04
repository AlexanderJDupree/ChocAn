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

TEST_CASE("Account builder builds new account", "[account_builder]")
{
    mock_dependencies mocks;
    Account_Builder account_builder;

    SECTION("Builds an account with valid input", "[account_builder][use_case][happy_path]")
    {

        account_builder.initiate_new_build_process();

        std::string input = "member";

        SECTION("Builder accepts valid account type and makes transition")
        {
            REQUIRE(std::holds_alternative<Account_Builder::Type>(account_builder.builder_state()));
            account_builder.set_field(input);
            REQUIRE(std::holds_alternative<Account_Builder::First>(account_builder.builder_state()));
        }
    
    }
    
}
