/* 

File: login_state.hpp

Brief: Login State controls login behavior for the ChocAn application

Authors: Daniel Mendez 
         Alex Salazar
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
    Login_State(std::string&& login_status = "Welcome")
        : login_status(std::move(login_status)) {}

    State_Ptr evaluate(const Input_Vector& input);

    State_Info info() const;

    bool login(const std::string& input);

private: 

    const std::string login_status;

};


#endif // CHOCAN_LOGIN_STATE_H