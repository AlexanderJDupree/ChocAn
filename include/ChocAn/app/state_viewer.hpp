/* 
 
File: state_viewer.hpp

Brief:  

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_STATE_VIEWER_H
#define CHOCAN_STATE_VIEWER_H

#include <ChocAn/app/state.hpp>

class State_Viewer
{
public:

    virtual ~State_Viewer() {}

    virtual std::string interact() const = 0;

    virtual void render_state(const State& state) const = 0;

};

#endif // CHOCAN_STATE_VIEWER_H