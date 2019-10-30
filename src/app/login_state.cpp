/* 
 
File: login_state.cpp

Brief: Implements login state behavior

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/app/states/exit_state.hpp>
#include <ChocAn/app/states/login_state.hpp>
#include <ChocAn/app/states/provider_menu_state.hpp>
#include <ChocAn/app/states/manager_menu_state.hpp>

State::State_Ptr Login_State::evaluate(const Input_Vector& input)
{
    // TODO check vector size??
    if(input.at(0) == "exit")
    {
        return std::make_unique<Exit_State>();
    }
    if(input.at(0) == "5678")
    {
        return std::make_unique<Manager_Menu_State>();
    }
    if(login(input.at(0)))
    {
        return std::make_unique<Provider_Menu_State>();
    }
    return std::make_unique<Login_State>("Invalid Login");
}

State_Info Login_State::info() const
{
    return State_Info { login_status };
}

bool Login_State::login(const std::string& input)
{
    // TODO implement login logic
    if (input == "1234")
    {
        return true;
    }
    return false;
}
