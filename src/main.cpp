/* 
 
File: main.cpp

Brief: Provides the entry point for the ChocAn data processing application.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/LinkedListsCPP
 
*/

#include <clara.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/data/sqlite_db.hpp>
#include <ChocAn/app/state_controller.hpp>
#include <ChocAn/view/terminal_state_viewer.hpp>
#include <ChocAn/view/terminal_input_controller.hpp>

int run();

int main (int, char **) 
{
    using namespace clara;

    auto cli = 
    run();
}

int run()
{
    ChocAn::Database_Ptr db = std::make_unique<Mock_DB>();

    State_Controller controller ( std::make_unique<ChocAn>(db)
                                , std::make_unique<Terminal_State_Viewer>()
                                , std::make_unique<Terminal_Input_Controller>() );

    // TODO exit loop if viewer can't open view
    while(!controller.end_state())
    {
        controller.interact();
    }

    return 0;
}