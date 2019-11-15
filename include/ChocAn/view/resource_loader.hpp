/* 
File: resource_loader.hpp

Brief:  Resource Loader is a visitor functor that seriliazes state resources 
        into strings that the state_viewer can utilize

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_VIEW_RESOURCE_LOADER_HPP
#define CHOCAN_VIEW_RESOURCE_LOADER_HPP

#include <sstream>
#include <ChocAn/view/terminal_state_viewer.hpp>

struct Resource_Loader
{
    using Resource_Table = Terminal_State_Viewer::Resource_Table;

    Resource_Table operator()(const Exit&);
    Resource_Table operator()(const Login& login);
    Resource_Table operator()(const Provider_Menu& menu);
    Resource_Table operator()(const Manager_Menu& menu);
    Resource_Table operator()(const Add_Transaction& transaction);
    Resource_Table operator()(const Confirm_Transaction& state);

    std::string render_user_error(const chocan_user_exception& err);

};

#endif // CHOCAN_VIEW_RESOURCE_LOADER_HPP
