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

#include <map>
#include <ChocAn/app/application_state.hpp>

struct Resource_Loader
{
    using Resource_Table = std::map<std::string, std::string>;

    Resource_Loader(const Application_State& state)
        : state(&state)
        { update(); }

    void update();

    Resource_Table operator()(const Exit&);
    Resource_Table operator()(const Login& login);
    Resource_Table operator()(const View_Account&);
    Resource_Table operator()(const Provider_Menu& menu);
    Resource_Table operator()(const Manager_Menu& menu);
    Resource_Table operator()(const Add_Transaction& transaction);
    Resource_Table operator()(const Confirm_Transaction& state);
    Resource_Table operator()(const Create_Account& state);
    Resource_Table operator()(const Find_Account& state);

    //for getting fields while creating account 
    Resource_Table operator()(const Get_Type& field_state);
    Resource_Table operator()(const Get_First& field_state);
    Resource_Table operator()(const Get_Last& field_state);
    Resource_Table operator()(const Get_Street& field_state);
    Resource_Table operator()(const Get_City& field_state);
    Resource_Table operator()(const Get_State& field_state);
    Resource_Table operator()(const Get_Zip& field_state);

    std::string render_user_error(const chocan_user_exception& err);

    const Application_State* state;
    Resource_Table table;
};

#endif // CHOCAN_VIEW_RESOURCE_LOADER_HPP
