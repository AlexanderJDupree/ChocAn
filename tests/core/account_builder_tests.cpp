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

TEST_CASE("Account builds with valid inputs", "[account_builder]")
{

    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
            , mocks.state_viewer
            , mocks.input_controller
            , Create_Account{ &mocks.chocan->account_builder.reset() } );
    

    SECTION("Builds an account with valid input", "[account_builder][use_case][happy_path]")
    {
        mocks.in_stream << "member\nfirst\nlast\nstreet\ncity\nor\n97080";

        // Controller must eat all the input from the stream
        for (int i = 0; i < 7; ++i)
        {
            controller.interact();
        }
    
        REQUIRE(std::holds_alternative<Manager_Menu>(controller.current_state()));
    }
    
}
