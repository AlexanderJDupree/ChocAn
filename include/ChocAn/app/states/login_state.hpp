/* 

File: login_state.hpp

Brief: Login State controls login behavior for the ChocAn application

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_LOGIN_STATE_H
#define CHOCAN_LOGIN_STATE_H

#include <ChocAn/app/state.hpp>

class Login_State : public State
{
public:
    State_Ptr evaluate(const std::string& input);

    bool login(std::string input);
};


#endif // CHOCAN_LOGIN_STATE_H