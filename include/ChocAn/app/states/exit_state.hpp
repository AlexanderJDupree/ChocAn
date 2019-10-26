/* 

File: exit_state.hpp

Brief: exit state is an end state for the chocAn application and will perform 
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
public:
    State_Ptr evaluate(const std::string& input);

    State_Info info() const;
};

#endif // CHOCAN_EXIT_STATE_H