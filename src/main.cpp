/* 
 
File: main.cpp

Brief: Provides the entry point for the ChocAn data processing application

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/LinkedListsCPP
 
*/

#include <ChocAn/app/state_controller.hpp>
#include <ChocAn/view/terminal_state_viewer.hpp>

int main () {

    State_Controller controller;
    Terminal_State_Viewer viewer;

    while(!controller.end_state())
    {
        viewer.display_state(controller.current_state());

        controller.transition(viewer.interact());
    }

    return 0;
}
