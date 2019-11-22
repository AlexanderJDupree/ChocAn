/* 
 
File: input_controller.hpp

Brief: Input Controller defines a uniform way of receiving different types of 
       input from the user.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_INPUT_CONTROLLER_H
#define CHOCAN_INPUT_CONTROLLER_H

#include <map>
#include <vector>
#include <string>
#include <functional>

class Input_Controller
{
public:

    using Fields         = std::vector<std::string>;
    using Form_Data      = std::map<std::string, std::string>;
    using Field_Callback = std::function<void(const std::string&)>;

    using Input_Control_Ptr = std::shared_ptr<Input_Controller>;

    // Read a single line of input
    virtual std::string read_input() const = 0;

    // Read a line of input for each field in fields
    virtual Form_Data read_form(const Fields& fields, Field_Callback prompt) const = 0;
};

#endif // CHOCAN_INPUT_CONTROLLER_H