/* 

File: manager_menu_state.hpp

Brief: Manager Menu is an intermediate state that directs user 

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_MANAGER_MENU_STATE_H
#define CHOCAN_MANAGER_MENU_STATE_H

#include <ChocAn/app/state.hpp>

class Manager_Menu_State : public State
{
public:

    Manager_Menu_State() {}

    ~Manager_Menu_State() {}

    State_Ptr evaluate(const Input_Vector& input);

    State_Info info() const;
};

#endif  // CHOCAN_MANAGER_MENU_STATE_H
