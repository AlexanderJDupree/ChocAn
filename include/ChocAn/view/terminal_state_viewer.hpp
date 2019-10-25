/* 
 
File: terminal_state_viewer.hpp

Brief:  

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_TERMINAL_STATE_VIEWER_H
#define CHOCAN_TERMINAL_STATE_VIEWER_H

#include <map>
#include <ChocAn/app/state_viewer.hpp>

class Terminal_State_Viewer : public State_Viewer
{
public:

    typedef std::map<size_t, std::string> View_Table;

    std::string interact() const;

    void render_state(const State& state) const;

private:

    void render(const std::string& view_name) const;

    void render_state_info(const State_Info& info) const;

    void reset_input_stream() const;

    static const View_Table view_table;
    static const std::string view_location;
    static const std::string view_not_implemented;

};

#endif // CHOCAN_TERMINAL_STATE_VIEWER_H