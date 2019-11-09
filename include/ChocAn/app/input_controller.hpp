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
#include <limits>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

class Input_Controller
{
public:


    using Form_Data   = std::map<std::string, std::string>;
    using Field_Data  = std::pair<const std::string, std::string>;
    using Form_Prompt = std::function<void(const std::string&)>;

    using Input_Control_Ptr = std::shared_ptr<Input_Controller>;

    Input_Controller(std::istream& in_stream = std::cin)
        : in_stream(in_stream) {}

    // TODO Use SFINAE to ensure T is default constructible
    std::string read_input() const
    {
        std::string input;

        std::getline(in_stream, input);

        return input;
    }

    Form_Data& read_form(Form_Data& fields, Form_Prompt prompt)
    {
        // Read input into each field
        std::for_each( fields.begin(), fields.end(),
                       [&](Field_Data& field)
                       {
                           prompt(field.first);
                           field.second = read_input();
                       } );

        return fields;
    }

private:

    void reset_input_stream() const
    {
        in_stream.clear();

        // discard characters up to the limit of the stream OR to newline
        in_stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::istream& in_stream;

};

#endif // CHOCAN_INPUT_CONTROLLER_H