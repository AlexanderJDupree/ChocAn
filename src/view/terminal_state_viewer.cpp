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

#include <fstream>
#include <iostream>
#include <ChocAn/view/resource_loader.hpp>
#include <ChocAn/view/terminal_state_viewer.hpp>

Terminal_State_Viewer::Terminal_State_Viewer( std::string&& view_location
                                            , std::string&& file_extension 
                                            , std::ostream& out_stream )
    : view_location  ( view_location  )
    , file_extension ( file_extension )
    , out_stream     ( out_stream     )
    , command_table  ( {
          { "header",       [&](){ return render_view("header");  } }
        , { "footer",       [&](){ return render_view("footer");  } }
        , { "prompt",       [&](){ return render_prompt();        } }
        , { "empty_prompt", [&](){ return render_prompt("");      } }
    } )
    , resource_table ( { }     )
    {}

void Terminal_State_Viewer::render_prompt(const std::string& prompt)
{
    out_stream << prompt;
}

void Terminal_State_Viewer::render_state(const Application_State& state)
{
    resource_table = std::visit(Resource_Loader(), state);

    return this->update();
}

void Terminal_State_Viewer::update()
{
    render_view(resource_table.at("state_name")());
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
            if (symbol == '<') 
            { 
                if (file.peek() == '@' && file.get(symbol).good())
                {
                    out_stream << read_resource(read_command(file));
                }
                else
                {
                    execute_command(read_command(file)); 
                }
            }
            else               
            { 
                out_stream << symbol; 
            }
        }
    }
    else
    {
        out_stream << "Error: Unable to open view: " << view << std::endl;
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

std::string Terminal_State_Viewer::read_resource(const std::string& resource_name)
{
    try
    {
        return resource_table.at(resource_name)();
    }
    catch(const std::out_of_range&)
    {
        return "Unknown Resource: " + resource_name;
    }
}

void Terminal_State_Viewer::execute_command(const std::string& command)
{
    try
    {
        command_table.at(command)();
    }
    catch(const std::out_of_range&)
    {
        out_stream << "Viewer Command: " << command << ", not recognized\n";
    }
    return;
}
