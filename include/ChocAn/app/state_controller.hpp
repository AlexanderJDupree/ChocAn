/* 
 
File: state_controller.hpp

Brief: State Controller models user interaction with the application as a 
       Finte State Machine. 

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef  CHOCAN_STATE_CONTROLLER_H
#define  CHOCAN_STATE_CONTROLLER_H

#include <map>
#include <stack>
#include <functional>
#include <ChocAn/core/chocan.hpp>
#include <ChocAn/app/state_viewer.hpp>
#include <ChocAn/app/input_controller.hpp>
#include <ChocAn/app/application_state.hpp>

class State_Controller
{
public:

    using ChocAn_Ptr        = ChocAn::ChocAn_Ptr;
    using Runtime_Stack     = std::stack<Application_State>;
    using State_Viewer_Ptr  = State_Viewer::State_Viewer_Ptr;
    using Input_Control_Ptr = Input_Controller::Input_Control_Ptr;
    using Transition_Table  = std::map<std::string, std::function<Application_State()>>;

    // TODO set default instances
    State_Controller( ChocAn_Ptr        chocan
                    , State_Viewer_Ptr  state_viewer
                    , Input_Control_Ptr input_controller
                    , Application_State initial_state = Login());

    State_Controller& interact();

    const Application_State& current_state() const;

    bool end_state() const;

    /** Visitor Methods **/
    Application_State operator()(Exit&);
    Application_State operator()(Login&);
    Application_State operator()(Find_Account&);
    Application_State operator()(View_Account&);
    Application_State operator()(Manager_Menu&);
    Application_State operator()(Provider_Menu&);
    Application_State operator()(Delete_Account&);
    Application_State operator()(Create_Account&);
    Application_State operator()(Update_Account&);
    Application_State operator()(Add_Transaction&);
    Application_State operator()(Generate_Report&);
    Application_State operator()(View_Summary_Report&);
    Application_State operator()(Confirm_Transaction&);
    Application_State operator()(View_Service_Directory&);

private:

    Application_State pop_runtime();

    ChocAn_Ptr         chocan;
    State_Viewer_Ptr   state_viewer;
    Input_Control_Ptr  input_controller;
    Runtime_Stack      runtime;
    bool               is_end_state = false;

};

#endif // CHOCAN_STATE_CONTROLLER_H
