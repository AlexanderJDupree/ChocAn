/* 
File: terminal_state_viewer.cpp

Brief:  Implementation file for terminal state viewer

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <limits>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <ChocAn/app/states/exit_state.hpp>
#include <ChocAn/app/states/login_state.hpp>
#include <ChocAn/app/states/provider_menu_state.hpp>

#include <ChocAn/view/terminal_state_viewer.hpp>


const Terminal_State_Viewer::View_Table Terminal_State_Viewer::view_table 
{
    { Login_State().id(),         "login" },
    { Exit_State().id(),          "exit" },
    { Provider_Menu_State().id(), "provider_menu" }
};

Terminal_State_Viewer::Terminal_State_Viewer( std::string&& view_location
                                            , std::string&& view_not_impl
                                            , std::string&& file_extension )
    : view_location        ( std::move(view_location)  )
    , view_not_implemented ( std::move(view_not_impl)  ) 
    , file_extension       ( std::move(file_extension) )
    , input_vector  ()
    , command_table ({
          { "header",       [&](){ return render_view("header"); }}
        , { "footer",       [&](){ return render_view("footer"); }}
        , { "datetime",     [&](){ return render_view("datetime"); }}
        , { "prompt",       [&](){ return input_vector.emplace_back(get_input()); }}
        , { "empty_prompt", [&](){ return input_vector.emplace_back(get_input("")); }}
        , { "status_msg",   [&](){ std::cout << state_info.status_msg.value_or(""); }}
    }) {}

const State::Input_Vector& Terminal_State_Viewer::interact()
{
    // Ensures input vector is never empty
    input_vector.push_back("");

    return input_vector;
}

std::string Terminal_State_Viewer::get_input(const std::string& prompt) const
{
    std::string input;

    std::cout << prompt;

    std::cin >> input;

    reset_input_stream();

    return input;
}

void Terminal_State_Viewer::reset_input_stream() const
{
    // reset failed state
    std::cin.clear();

    // discard characters up to the limit of the stream OR to newline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return;
}

void Terminal_State_Viewer::render_state(const State& state)
{
    // Reset input vector
    input_vector.clear();
    // Load state info
    state_info = state.info();

    try
    {
        render_view(view_table.at(state.id()));
    }
    catch(const std::out_of_range& err)
    {
        render_view(view_not_implemented);
    }
    return;
}

void Terminal_State_Viewer::render_view(const std::string& view_name)
{
    std::string view = view_location + view_name + file_extension;
    std::ifstream file(view);

    if(file.is_open())
    {
        char symbol;
        while (file.get(symbol).good())
        {
            // Display characters until 'start command' character is reached
            if (symbol == '<') { execute_command(read_command(file)); }
            else               { std::cout << symbol; }
        }
    }
    else
    {
        std::cerr << "Error: Unable to open view: " << view << std::endl;
    }
    return file.close();
}

std::string Terminal_State_Viewer::read_command(std::ifstream& file)
{
    char symbol;
    std::stringstream command;

    // Consume symbols until end command character is reached
    while(file.get(symbol).good() && symbol != '>')
    {
        command << symbol;
    }
    return command.str();
}

void Terminal_State_Viewer::execute_command(const std::string& command)
{
    try
    {
        command_table.at(command)();
    }
    catch(const std::out_of_range& err)
    {
        std::cerr << "Viewer Command: " << command << ", not recognized\n";
    }
    return;
}
