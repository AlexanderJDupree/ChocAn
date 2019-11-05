/* 

File: state.hpp

Brief: state class is an abstract base class and provides the interface that 
       all derived states must implement. Each state is recognized by their 
       unique typeid hash code.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_STATE_H
#define CHOCAN_STATE_H

#include <map>
#include <memory>
#include <vector>
#include <typeinfo>
#include <functional>
#include <ChocAn/core/chocan.hpp>

class State_Info
{
public:

    std::optional<std::string> status_msg;

};

class State
{
public:

    typedef std::shared_ptr<State>      State_Ptr;
    typedef std::vector<std::string>    Input_Vector;

    typedef std::map<std::string, std::function<State_Ptr()>> Transition_Table;

    virtual ~State() {};

    virtual State_Ptr evaluate(const Input_Vector& input) = 0;

    virtual State_Info info() const = 0;

    size_t id() const { return typeid(*this).hash_code(); }

    bool operator == (const State& rhs) const { return this->id() == rhs.id(); }

protected:

    // Main ChocAn service shared between states
    static ChocAn chocan;
};

#endif  // CHOCAN_STATE_H
