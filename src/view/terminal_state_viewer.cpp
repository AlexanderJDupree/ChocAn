/* 
 
File: terminal_state_viewer.cpp

Brief:  

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <iostream>
#include <ChocAn/view/terminal_state_viewer.hpp>

std::string Terminal_State_Viewer::interact() const
{
    std::string input;

    std::cin >> input;

    reset_input_stream();

    return input;
}

void Terminal_State_Viewer::reset_input_stream() const
{
    // reset failed state
    std::cin.clear();

    // discard characters up to the limit of the stream OR to newline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return;
}

void Terminal_State_Viewer::display_state(const State& state) const
{
    std::cout << "\nState ID:" << state.id() << "\nView Not implemented" 
              << "\n> ";
}