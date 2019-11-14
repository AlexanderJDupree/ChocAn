/* 
File: state_controller_tests.cpp

Brief: Unit tests for State Controller machine

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn

*/

#include <catch.hpp>

#include <vector>
#include <sstream>
#include <ChocAn/core/chocan.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/view/terminal_input_controller.hpp>
#include <ChocAn/app/state_controller.hpp>
#include <ChocAn/view/terminal_state_viewer.hpp>

class mock_dependencies
{
public:
    std::stringstream in_stream;
    std::stringstream out_stream;

    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();
    ChocAn::ChocAn_Ptr chocan     = std::make_unique<ChocAn>(db);

    State_Viewer::State_Viewer_Ptr state_viewer = 
        std::make_unique<Terminal_State_Viewer>( "tests/view"
                                               , "-test.txt"
                                               , out_stream );

    Input_Controller::Input_Control_Ptr input_controller = 
        std::make_unique<Terminal_Input_Controller>(in_stream);
};

TEST_CASE("State Controller construction", "[constructors], [state_controller]")
{
    mock_dependencies mocks;

    SECTION("State Controller requires a constructed Chocan instance, state viewer, and input controller")
    {
        REQUIRE_NOTHROW(State_Controller( mocks.chocan
                                        , mocks.state_viewer
                                        , mocks.input_controller ));
    }
    SECTION("State Controller throws if any dependency is null")
    {
        REQUIRE_THROWS(State_Controller(nullptr, nullptr, nullptr));
    }
}

TEST_CASE("Shared State Behavior", "[state], [state_controller]")
{
    mock_dependencies mocks;
    std::vector<Application_State> states { Login()
                                          , Exit()
                                          , Provider_Menu()
                                          , Manager_Menu()
                                          };

    SECTION("State Controller does not transition state on invalid input")
    {
        std::for_each(states.begin(), states.end(), 
        [&](const Application_State& state)
        {
            mocks.in_stream << "Bad Input\n";

            State_Controller controller( mocks.chocan
                                       , mocks.state_viewer
                                       , mocks.input_controller
                                       , state);

            REQUIRE(controller.transition().current_state().index() == state.index());
        } );
    }
}

TEST_CASE("Login state behavior", "[login], [state_controller]")
{
    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Login() );
    
    /* Ensure A manager and provider account exist in mock db */
    Account provider( Name("John", "Doe")
                    , Address( "1234 Cool St."
                             , "Portland"
                             , "OR"
                             , 97030 )
                    , Provider()
                    , mocks.chocan->id_generator );
    mocks.chocan->db->create_account(provider);

    Account manager( Name("Jane", "Doe")
                    , Address( "1234 Meh St."
                             , "Portland"
                             , "OR"
                             , 97030 )
                    , Manager()
                    , mocks.chocan->id_generator );
    mocks.chocan->db->create_account(manager);

    SECTION("Login transitions to provider menu when given a valid provider ID")
    {
        Application_State expected_state { Provider_Menu() };

        mocks.in_stream << provider.id() << '\n';

        REQUIRE(controller.transition().current_state().index() == expected_state.index());
    }
    SECTION("Login transitions to manager menu when given a valid manager ID")
    {
        Application_State expected_state { Manager_Menu() };

        mocks.in_stream << manager.id() << '\n';

        REQUIRE(controller.transition().current_state().index() == expected_state.index());
    }
}

TEST_CASE("Provider Menu State behavior", "[provider_menu], [state_controller]")
{
    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Provider_Menu() );

    SECTION("Provider menu transitions to login on input '0'")
    {
        Application_State expected_state { Login() };

        mocks.in_stream << "0\n";

        REQUIRE(controller.transition().current_state().index() == expected_state.index());
    }
    SECTION("Provider menu transitions to Add Transaction on input '5'")
    {
        Application_State expected_state { Add_Transaction { &mocks.chocan->transaction_builder } };

        mocks.in_stream << "5\n";

        REQUIRE(controller.transition().current_state().index() == expected_state.index());
    }
    SECTION("Provider menu transition to exit on input 'exit'")
    {
        Application_State expected_state { Exit() };

        mocks.in_stream << "exit\n";

        REQUIRE(controller.transition().current_state().index() == expected_state.index());
    }
}

TEST_CASE("Manager Menu State behavior", "[manager_menu], [state_controller]")
{
    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Manager_Menu() );

    SECTION("Manager menu transitions to login on input '0'")
    {
        Application_State expected_state { Login() };

        mocks.in_stream << "0\n";

        REQUIRE(controller.transition().current_state().index() == expected_state.index());
    }
    SECTION("Manager menu transition to exit on input 'exit'")
    {
        Application_State expected_state { Exit() };

        mocks.in_stream << "exit\n";

        REQUIRE(controller.transition().current_state().index() == expected_state.index());
    }
}

TEST_CASE("Add_Transaction State Behavior", "[add_transaction], [state_controller]")
{
    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Add_Transaction{ &mocks.chocan->transaction_builder } );

    SECTION("Add transaction transitions to Confrim Transaction when a transaction is built")
    {
    }
}