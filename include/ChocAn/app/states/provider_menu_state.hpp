/* 

File: provider_menu_state.hpp

Brief: Provider Menu is an intermediate state that directs user to a specific 
       set of actions that a provider can do. 

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_PROVIDER_MENU_STATE_H
#define CHOCAN_PROVIDER_MENU_STATE_H

#include <ChocAn/app/state.hpp>

class Provider_Menu_State : public State
{
public:
    State_Ptr evaluate(const Input_Vector& input);

    State_Info info() const;
};

#endif // CHOCAN_PROVIDER_MENU_STATE_H
