/* 
 
File: state_viewer.hpp

Brief:  State Viewer interface defines the methods required for the main module
        to interact with the state controller.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_STATE_VIEWER_H
#define CHOCAN_STATE_VIEWER_H

#include <memory>
#include <ChocAn/app/application_state.hpp>

class State_Viewer
{
public:

    using State_Viewer_Ptr = std::shared_ptr<State_Viewer>;

    virtual ~State_Viewer() {}

    virtual void render_state(const Application_State& state) = 0;

};

#endif // CHOCAN_STATE_VIEWER_H