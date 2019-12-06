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

#include <sstream>
#include <fstream>
#include <iostream>
#include <ChocAn/view/terminal_state_viewer.hpp>

Terminal_State_Viewer::Terminal_State_Viewer( bool compact_output
                                            , std::ostream& out_stream
                                            , std::string&& view_location
                                            , std::string&& file_extension )
    : view_location  ( view_location  )
    , file_extension ( file_extension )
    , out_stream     ( out_stream     )
    , command_table  ( {
          { "header",       [&](){ return render_view("header");  } }
        , { "footer",       [&](){ return render_view("footer");  } }
<<<<<<< HEAD
        , { "clear_screen", [&](){ return clear_screen();  } }
        , { "prompt",       [&](){ event_callback(); } }
=======
        , { "clear_screen", [&](){ return clear_screen();         } }
        , { "prompt",       [&](){ return event_callback();       } }
>>>>>>> master
    } )
    , resources ({})
    , compact_output( compact_output )
    {}


void Terminal_State_Viewer::render_state(const Application_State& state, Callback handler)
{
    resources = Resource_Loader(state);

    event_callback = handler;

    return this->update();
}

void Terminal_State_Viewer::update()
{
    resources.update();
    try
    {
        render_view(resources.table.at("state_name"));
    }
    catch(const std::out_of_range&)
    {
        out_stream << "Error: 'state_name' is not defined for current state\n";
    }
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
        return resources.table.at(resource_name);
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

std::string Terminal_State_Viewer::clear_screen() const
{
<<<<<<< HEAD
    return std::string(100, '\n');
=======
    if(!compact_output)
    {
        out_stream << std::string(100, '\n');
    }

    return;
>>>>>>> master
}
