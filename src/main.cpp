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

#include <limits>
#include <iostream>
#include <ChocAn/app/state_controller.hpp>

// TODO move interactive methods into an actual class interface
// TODO these are standin functions until we architect a better solution
template <typename T>
T get_input(const char* prompt = "\n> ");
// Clears fail flags, flushes input buffer
void reset_input_stream();

int main () {

//    State_Viewer view;
    State_Controller controller;

    while(!controller.accept_state())
    {
//       view.display_state(controller.current_state());
        std::cout << "current state: " << controller.current_state().id() << std::endl;

        controller.transition(get_input<std::string>());
    }

    return 0;
}

template <typename T>
T get_input(const char* prompt)
{
    T input;

    std::cout << prompt;
    std::cin >> input;

    reset_input_stream();

    return input;
}

void reset_input_stream()
{
    // reset failed state
    std::cin.clear();

    // discard characters up to the limit of the stream OR to newline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return;
}
