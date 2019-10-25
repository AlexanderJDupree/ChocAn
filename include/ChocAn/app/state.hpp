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

#include <memory>
#include <typeinfo>
#include <optional>

// TODO evaluate prototype
class State_Info
{
public:

    std::optional<std::string> msg;

};

class State
{
public:

    typedef std::shared_ptr<State> State_Ptr;

    virtual ~State() {};

    virtual State_Ptr evaluate(const std::string& input) = 0;

    virtual State_Info info() const = 0;

    size_t id() const { return typeid(*this).hash_code(); }

    bool operator == (const State& rhs) const { return this->id() == rhs.id(); }

    bool operator < (const State& rhs) const { return id() < rhs.id(); }
};

#endif  // CHOCAN_STATE_H
