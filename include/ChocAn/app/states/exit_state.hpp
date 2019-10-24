/* 

File: exit_state.hpp

Brief: exit state is a final state for the chocAn application and will perform 
       any necessary teardown.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_EXIT_STATE_H
#define CHOCAN_EXIT_STATE_H

#include <ChocAn/app/state.hpp>

class Exit_State : public State
{
    State_Ptr evaluate(const std::string& input);
};

#endif // CHOCAN_EXIT_STATE_H