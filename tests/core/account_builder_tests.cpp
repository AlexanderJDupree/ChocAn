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
struct mock_errors{

        const std::string get_state_error(const std::string st)const{ return "State must be in abbreviated 2 character format\n \"" 
                                                          + st + " \" is not a U.S. state";};
        const std::string get_street_error()   const{return "Street address must be less than 25 characters";};
        const std::string get_city_error()     const{return "Cities must be less than 14 character";};
        const std::string get_zip_error()      const{return "Zip code must be 5 digit number";};
        const std::string get_invalid_address()const{ return "Invalid address values";};
        const std::string get_invalid_first()  const{ return "First Name must be 1 to 24 characters long";};
        const std::string get_invalid_last()   const{return "Last Name must be 1 to 24 characters long";};
        const std::string get_invalid_full()   const{return "Full Name must be 1 to 25 characters long";};
        const std::string get_invalid_name()   const{ return "Invalid name length";};
        const std::string get_build_premature()const{ return "Attempt made to build prematurely";};
        const std::string get_build_failed()   const{return "Failed to build account";};
        const std::string get_build_()         const{return "Issues with account";};
};

Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();
mock_errors error;
 
TEST_CASE("Account builds with valid inputs", "[account_builder]")
{

    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
            , mocks.state_viewer
            , mocks.input_controller
            , Create_Account{ &mocks.chocan->account_builder.reset() } );
    

    SECTION("Builds an account with valid input", "[account_builder][use_case][happy_path]")
    {
        Application_State expected_state{Manager_Menu()};

        mocks.in_stream << "member\nfirst\nlast\nstreet\ncity\nor\n97080";

        // Controller must eat all the input from the stream
        for (int i = 0; i < 7; ++i)
        {
            controller.interact();
        }
    
        REQUIRE(controller.current_state().index() == expected_state.index());
    }
    
}
