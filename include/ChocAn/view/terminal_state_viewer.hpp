/* 
 
File: terminal_state_viewer.hpp

Brief:  Terminal State Viewer handles IO actions for the application. Prints
        and displays state views as well as store user input into an input 
        vector.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_TERMINAL_STATE_VIEWER_H
#define CHOCAN_TERMINAL_STATE_VIEWER_H

#include <map>
#include <iostream>
#include <functional>
#include <ChocAn/app/state_viewer.hpp>

class Terminal_State_Viewer : public State_Viewer
{
public:

    using Command_Table  = std::map<std::string, std::function<void()>>;
    using Resource_Table = std::map<std::string, std::function<std::string()>>;

    Terminal_State_Viewer( std::string&& view_location  = "views/" 
                         , std::string&& file_extension = ".txt" 
                         , std::ostream& out_stream     = std::cout );


    // Re-renders the stored state
    void update() override;

    // Stores state, looks up state in view table, renders view
    void render_state(const Application_State& state) override;

private:

    void render_view(const std::string& view_name);

    void render_prompt(const std::string& prompt = "> ");

    std::string read_resource(const std::string& resource_name);

    std::string read_command(std::ifstream& file);
    void execute_command(const std::string& command);

    // Relative file location where view files are located
    // TODO Update view_location to use absolute paths
    const std::string   view_location; 
    const std::string   file_extension;
    std::ostream&       out_stream;
    Command_Table       command_table;
    Resource_Table      resource_table;
};

#endif // CHOCAN_TERMINAL_STATE_VIEWER_H