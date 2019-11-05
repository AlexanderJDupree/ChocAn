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
    if(login(input.at(0)))
    {
        if (chocan->login_manager.session_owner()->type == Account_Type::Manager)
        {
               return std::make_unique<Manager_Menu_State>();
        }
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
    try
    {
        // stoi throws invalid_argument if input can't be converted to int
        return chocan->login_manager.login(std::stoi(input));
    }
    catch(const std::invalid_argument& err)
    {
        return false;
    }
}
