/* 
File: terminal_state_viewer.cpp

Brief:  

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <fstream>
#include <iostream>
#include <stdexcept>

#include <ChocAn/app/states/exit_state.hpp>
#include <ChocAn/app/states/login_state.hpp>
#include <ChocAn/app/states/provider_menu_state.hpp>

#include <ChocAn/view/terminal_state_viewer.hpp>


const Terminal_State_Viewer::View_Table Terminal_State_Viewer::view_table 
{
    { Login_State().id(), "login.txt" },
    { Exit_State().id(), "exit.txt" },
    { Provider_Menu_State().id(), "provider_menu.txt" }
};

const std::string Terminal_State_Viewer::view_location = "views/";
const std::string Terminal_State_Viewer::view_not_implemented = "not_impl.txt";

std::string Terminal_State_Viewer::interact() const
{
    std::string input;

    std::cin >> input;

    reset_input_stream();

    return input;
}

void Terminal_State_Viewer::reset_input_stream() const
{
    // reset failed state
    std::cin.clear();

    // discard ARBITRARY 256 characters OR to newline
    std::cin.ignore(256, '\n');
    return;
}

void Terminal_State_Viewer::render_state(const State& state) const
{
    try
    {
        render(view_table.at(state.id()));
    }
    catch(const std::out_of_range& err)
    {
        render(view_not_implemented);
    }
    
    return;
}

void Terminal_State_Viewer::render(const std::string& view_name) const
{
    std::string view = view_location + view_name;
    std::ifstream file(view);

    if(file.is_open())
    {
        std::cout << file.rdbuf();
    }
    else
    {
        std::cerr << "Error: Unable to open view: " << view << std::endl;
    }
}


