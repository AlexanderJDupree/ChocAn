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
#include <functional>
#include <ChocAn/app/state_viewer.hpp>

class Terminal_State_Viewer : public State_Viewer
{
public:

    // Keys are state ID's, Values are view file names
    typedef std::map<size_t, std::string>                View_Table;

    // Viewer Command look up table
    typedef std::map<std::string, std::function<void()>> Command_Table;

    Terminal_State_Viewer( std::string&& view_location = "views/" 
                         , std::string&& view_not_impl = "not_impl"
                         , std::string&& file_extension = ".txt" );

    // Returns the input vector used for the current state
    const State::Input_Vector& interact();

    // Looks up state view in view_table, renders the view.
    void render_state(const State& state);

private:

    void render_view(const std::string& view_name);

    // Reads input from STDIN into a string
    std::string get_input(const std::string& prompt = "> ") const;
    std::string read_command(std::ifstream& file);

    // Looks up viewer command from command table, executes function
    void execute_command(const std::string& command);

    void reset_input_stream() const;

    // Relative file location where view files are located
    const std::string view_location; 
    const std::string view_not_implemented;
    const std::string file_extension;

    State_Info          state_info;
    State::Input_Vector input_vector;

    const Command_Table command_table;
    static const View_Table view_table;

};

#endif // CHOCAN_TERMINAL_STATE_VIEWER_H