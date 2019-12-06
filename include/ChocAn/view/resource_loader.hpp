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
#include <ChocAn/core/entities/account_report.hpp>

struct Resource_Loader
{
    using Resource_Table    = std::map<std::string, std::string>;
    using Provider_Activity = Summary_Report::Provider_Activity;

    Resource_Loader(const Application_State& state)
        : state(&state)
        { update(); }

    void update();

    Resource_Table operator()(const Exit&);
    Resource_Table operator()(const Login& login);
    Resource_Table operator()(const View_Account&);
    Resource_Table operator()(const Manager_Menu& menu);
    Resource_Table operator()(const Provider_Menu& menu);
    Resource_Table operator()(const Find_Account& state);
    Resource_Table operator()(const Delete_Account& state);
    Resource_Table operator()(const Create_Account& state);
    Resource_Table operator()(const Update_Account& state);
    Resource_Table operator()(const Generate_Report& state);
    Resource_Table operator()(const View_Summary_Report& state);
    Resource_Table operator()(const Confirm_Transaction& state);
    Resource_Table operator()(const Add_Transaction& transaction);
    Resource_Table operator()(const View_Service_Directory& state);

    /* String Formatting Methods */
    std::string row_bar(unsigned num_fields) const;
    std::string center(unsigned num, unsigned width = 20) const;
    std::string center(const std::string& str, unsigned width = 20) const;

    /* Special Rendering Methods */
    std::string render_user_error(const std::optional<chocan_user_exception>& maybe_err) const;
    std::string render_builder_prompt(Account_Builder::Build_State state) const;

    std::string render_provider_activity(const Provider_Activity& activity) const;

    std::string render_directory(const Data_Gateway::Service_Directory& directory) const;

    std::string render_summary(const Summary_Report& summary) const;

    const Application_State* state;
    Resource_Table table;
};

#endif // CHOCAN_VIEW_RESOURCE_LOADER_HPP
