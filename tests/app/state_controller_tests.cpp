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
#include <ChocAn/app/state_viewer.hpp>
#include <ChocAn/app/state_controller.hpp>
#include <ChocAn/view/terminal_input_controller.hpp>

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

            REQUIRE(controller.interact().current_state().index() == state.index());
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

    Account member( Name("Bob", "Doe")
                    , Address( "1234 lame St."
                             , "Portland"
                             , "OR"
                             , 97030 )
                    , Member()
                    , mocks.chocan->id_generator );

    mocks.chocan->db->create_account(member);

    SECTION("Login transitions to provider menu when given a valid provider ID")
    {
        Application_State expected_state { Provider_Menu() };

        mocks.in_stream << provider.id() << '\n';

        REQUIRE(controller.interact().current_state().index() == expected_state.index());
    }
    SECTION("Login transitions to manager menu when given a valid manager ID")
    {
        Application_State expected_state { Manager_Menu() };

        mocks.in_stream << manager.id() << '\n';

        REQUIRE(controller.interact().current_state().index() == expected_state.index());
    }
    SECTION("Login does not transition state when given an invalid ID")
    {
        mocks.in_stream << "garbage\n";

        REQUIRE(std::holds_alternative<Login>(controller.interact().current_state()));
    }
    SECTION("Login does not transition state when given a Member ID")
    {
        mocks.in_stream << member.id() << '\n';

        REQUIRE(std::holds_alternative<Login>(controller.interact().current_state()));
    }
}

TEST_CASE("Provider Menu State behavior", "[provider_menu], [state_controller]")
{
    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Provider_Menu() );

    mocks.chocan->login_manager.login(1234);

    SECTION("Provider menu transitions to login on input '0'")
    {
        Application_State expected_state { Login() };

        mocks.in_stream << "0\n";

        REQUIRE(controller.interact().current_state().index() == expected_state.index());
    }
    SECTION("Provider menu transitions to Add Transaction on input '5'")
    {
        Application_State expected_state { Add_Transaction { &mocks.chocan->transaction_builder } };

        mocks.in_stream << "5\n";

        REQUIRE(controller.interact().current_state().index() == expected_state.index());
    }
    SECTION("Provider menu transition to exit on input 'exit'")
    {
        Application_State expected_state { Exit() };

        mocks.in_stream << "exit\n";

        REQUIRE(controller.interact().current_state().index() == expected_state.index());
    }
    SECTION("Provider menu transitions to Find Account on input '4'")
    {
        mocks.in_stream << "4\n";
        
        REQUIRE(std::holds_alternative<Find_Account>(controller.interact().current_state()));
    }
    SECTION("Provider menu transitions to View Account on input '1'")
    {
      mocks.chocan->login_manager.login(1234);
      mocks.in_stream << "1\n";

      REQUIRE(std::holds_alternative<View_Account>(controller.interact().current_state()));
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

        REQUIRE(controller.interact().current_state().index() == expected_state.index());
    }
    SECTION("Manager menu transition to exit on input 'exit'")
    {
        Application_State expected_state { Exit() };

        mocks.in_stream << "exit\n";

        REQUIRE(controller.interact().current_state().index() == expected_state.index());
    }
    SECTION("Manager  menu transitions to Find Account on input '1'")
    {
        mocks.in_stream << "1\n";
        
        REQUIRE(std::holds_alternative<Find_Account>(controller.interact().current_state()));
    }
    SECTION("Manager menu transitions to Delete Account on input '4'")
    {
        mocks.in_stream << "2\n";

        REQUIRE(std::holds_alternative<Delete_Account>(controller.current_state()));
    }
    SECTION("Manager menu transitions to Generate Report input '5'")
    {
        mocks.in_stream << "5\n";
        
        REQUIRE(std::holds_alternative<Generate_Report>(controller.interact().current_state()));
    }
}

TEST_CASE("Add_Transaction State Behavior", "[add_transaction], [state_controller]")
{
    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Add_Transaction{ &mocks.chocan->transaction_builder.reset() } );

    SECTION("Add transaction transitions to Confrim Transaction when a transaction is built")
    {
        // This input sequence will change if we make changes to the transaction builder
        mocks.in_stream << "1234\n6789\n10-13-2019\n123456\ncomments\n";

        // Controller must eat all the input from the stream
        for (int i = 0; i < 5; ++i)
        {
            controller.interact();
        }

        REQUIRE(std::holds_alternative<Confirm_Transaction>(controller.current_state()));
    }
    SECTION("Add transaction transitions back to provider menu on input 'cancel'")
    {
        // This input sequence will change if we make changes to the transaction builder
        mocks.in_stream << "cancel\n";

        REQUIRE(std::holds_alternative<Provider_Menu>(controller.interact().current_state()));
    }
    SECTION("Add transaction transitions to Exit on input 'exit'")
    {
        // This input sequence will change if we make changes to the transaction builder
        mocks.in_stream << "exit\n";

        REQUIRE(std::holds_alternative<Exit>(controller.interact().current_state()));
    }
    SECTION("Add transaction transitions to View Service Directory on input 'help'")
    {
        mocks.in_stream << "help\n";

        REQUIRE(std::holds_alternative<View_Service_Directory>(controller.interact().current_state()));

        SECTION("View Service Directory transitions back to add transaction after pressing enter")
        {
            mocks.in_stream << "\n";

            REQUIRE(std::holds_alternative<Add_Transaction>(controller.interact().current_state()));
        }
    }
}

TEST_CASE("Confirm Transaction State Behavior", "[confirm_transaction], [state_controller]")
{
    mock_dependencies mocks;

    Transaction transaction(
        mocks.db->get_provider_account("1234").value(),
        mocks.db->get_member_account("6789").value(),
        DateTime(Month(9), Day(1), Year(2019)),
        mocks.db->lookup_service("123456").value(),
        "comments"
    );

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Confirm_Transaction { transaction } );

    SECTION("Confrim transaction transitions to provider menu when transaction is confirmed")
    {
        mocks.in_stream << "yes\n";

        REQUIRE(std::holds_alternative<Provider_Menu>(controller.interact().current_state()));
    }
    SECTION("Confrim transaction transitions back to Add Transaction if transaction is not confirmed")
    {
        // This input sequence will change if we make changes to the transaction builder
        mocks.in_stream << "no\n";

        REQUIRE(std::holds_alternative<Add_Transaction>(controller.interact().current_state()));
    }
}

TEST_CASE("Exit State Behavior", "[exit], [state_controller]")
{
    mock_dependencies mocks;

    mocks.chocan->login_manager.login(1234);

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Exit() );

    SECTION("end_state() returns true when controller is at exit state")
    {
        REQUIRE(controller.interact().end_state());
    }
    SECTION("Exit state logs the user out of the system")
    {
        controller.interact();

        REQUIRE_FALSE(mocks.chocan->login_manager.logged_in());
    }
}
TEST_CASE("Find Account State behavior", "[find_account], [state_controller]")
{
    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Provider_Menu() );

    mocks.chocan->login_manager.login(1234);

    mocks.in_stream << "4\n";

    controller.interact(); // Transition to find_account with the provider menu on the runtime stack

    SECTION("Find Account transitions to View Account on valid member id '9876'")
    {
        mocks.in_stream << "9876\n";

        REQUIRE(std::holds_alternative<View_Account>(controller.interact().current_state()));
    }
    SECTION("Find Account transitions to the previous menu on input 'cancel'")
    {
        mocks.in_stream << "cancel\n";

        REQUIRE(std::holds_alternative<Provider_Menu>(controller.interact().current_state()));
    }
    SECTION("Find Account does not transition state on invalid input")
    {
        mocks.in_stream << "garbage\n";

        REQUIRE(std::holds_alternative<Find_Account>(controller.interact().current_state()));
    }
}

TEST_CASE("Generate Report state behavior", "[generate_report], [state_controller]")
{
    mock_dependencies mocks;

    State_Controller controller( mocks.chocan
                               , mocks.state_viewer
                               , mocks.input_controller
                               , Manager_Menu() );

    mocks.chocan->login_manager.login(5678);

    mocks.in_stream << "5\n";

    controller.interact(); // Transition to Generate Report with the manager menu on the runtime stack

    SECTION("Entering exit, exits the application")
    {
        mocks.in_stream << "exit\n";

        REQUIRE(std::holds_alternative<Exit>(controller.interact().current_state()));
    }
    SECTION("Entering cancel returns to the previous menu")
    {
        mocks.in_stream << "cancel\n";

        REQUIRE(std::holds_alternative<Manager_Menu>(controller.interact().current_state()));
    }
}
