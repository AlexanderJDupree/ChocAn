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
#include <ChocAn/view/terminal_state_viewer.hpp>

int main () 
{
    State_Controller controller;
    Terminal_State_Viewer viewer;

    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    // TODO exit loop if viewer can't open view
    while(!controller.end_state())
    {
        viewer.render_state(controller.current_state());

        controller.transition(viewer.interact());
    }

    return 0;
}
