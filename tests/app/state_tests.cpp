/* 
File: state_tests.cpp

Brief: Unit tests for invdividual application states

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
*/

#include <catch.hpp>
#include <algorithm>
#include <functional>

#include <ChocAn/app/states/exit_state.hpp>
#include <ChocAn/app/states/login_state.hpp>
#include <ChocAn/app/states/provider_menu_state.hpp>

TEST_CASE("State Behavior", "[state]")
{
    Login_State login_state;
    Exit_State  exit_state;
    Provider_Menu_State provider_menu_state;

    std::vector<State*> states { &login_state
                               , &exit_state
                               , &provider_menu_state
                               }; 

    SECTION("States do not transition on evaluation of invalid input")
    {
        REQUIRE(*(Login_State().evaluate("Bad Input")) == Login_State());
        REQUIRE(*(Exit_State().evaluate("Bad_Input")) == Exit_State());
        REQUIRE(*(Provider_Menu_State().evaluate("Bad_Input")) == Provider_Menu_State());
    }
    SECTION("All states transition to exit state on input 'exit'")
    {
        std::for_each(states.begin(), states.end(), 
        [](State* state)
        { 
            REQUIRE(*(state->evaluate("exit")) == Exit_State());
        });
    }
    SECTION("Provider Menu State returns to login on input '0'")
    {
        REQUIRE(*(provider_menu_state.evaluate("0")) == Login_State());
    }
    SECTION("Login State returns login message after a failed attempt")
    {
        State::State_Ptr failed_login = Login_State().evaluate("bad input");

        REQUIRE(failed_login->info().msg);
    }

}