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
    using Input_Control_Ptr = Input_Controller::Input_Control_Ptr;
    using State_Viewer_Ptr  = State_Viewer::State_Viewer_Ptr;
    using Transition_Table  = std::map<std::string, std::function<Application_State()>>;
    using Runtime_Stack     = std::stack<Application_State>;

    // TODO set default instances
    State_Controller( ChocAn_Ptr        chocan
                    , State_Viewer_Ptr  state_viewer
                    , Input_Control_Ptr input_controller
                    , Application_State initial_state = Login());

    // Renders state view, and calls transition
    State_Controller& interact();

    // Transition to next state through visitor pattern
    State_Controller& transition();

    const Application_State& current_state() const;

    bool end_state() const;

    /** Visitor Methods **/
    Application_State operator()(const Login&);
    Application_State operator()(const Exit&);
    Application_State operator()(const Provider_Menu&);
    Application_State operator()(const Manager_Menu&);
    Application_State operator()(Add_Transaction&);
    Application_State operator()(const Confirm_Transaction&);
    Application_State operator()(Find_Account&);
    Application_State operator()(const View_Account&);

private:

    ChocAn_Ptr         chocan;
    State_Viewer_Ptr   state_viewer;
    Input_Control_Ptr  input_controller;
    Runtime_Stack      runtime;

};

#endif // CHOCAN_STATE_CONTROLLER_H
