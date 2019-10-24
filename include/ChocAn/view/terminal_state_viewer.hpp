/* 
 
File: terminal_state_viewer.hpp

Brief:  

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_TERMINAL_STATE_VIEWER_H
#define CHOCAN_TERMINAL_STATE_VIEWER_H

#include <ChocAn/app/state_viewer.hpp>

class Terminal_State_Viewer : public State_Viewer
{
public:

    std::string interact() const;

    void display_state(const State& state) const;

private:

    void reset_input_stream() const;

};

#endif // CHOCAN_TERMINAL_STATE_VIEWER_H