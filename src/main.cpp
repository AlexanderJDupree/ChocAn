/* 
 
File: main.cpp

Brief: Provides the entry point for the ChocAn data processing application.
       Main module utilizes a state machine model to control user interaction.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/LinkedListsCPP
 
*/

#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/app/state_controller.hpp>
#include <ChocAn/app/input_controller.hpp>
#include <ChocAn/view/terminal_state_viewer.hpp>

int main () 
{
    ChocAn::Database_Ptr db = std::make_unique<Mock_DB>();

    State_Controller controller ( std::make_unique<ChocAn>(db)
                                , std::make_unique<Terminal_State_Viewer>()
                                , std::make_unique<Input_Controller>() );

    // TODO exit loop if viewer can't open view
    while(!controller.end_state())
    {
        controller.interact();
    }
    // Required to render exit state, TODO refactor
    controller.interact();

    return 0;
}
