/* 
 
File: terminal_input_controller.hpp

Brief: terminal input controller retrieves input via STDIN

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_TERMINAL_INPUT_CONTROLLER_H
#define CHOCAN_TERMINAL_INPUT_CONTROLLER_H

#include <map>
#include <limits>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

#include <ChocAn/app/input_controller.hpp>

class Terminal_Input_Controller : public Input_Controller
{
public:

    Terminal_Input_Controller(std::istream& in_stream = std::cin)
        : in_stream(in_stream) {}

    std::string read_input() const override
    {
        std::string input;

        std::getline(in_stream, input);

        return input;
    }

    Form_Data read_form(const Fields& fields, Field_Callback prompt) const override
    {
        Form_Data form;

        // Read input into each field
        std::for_each( fields.begin(), fields.end(),
                       [&](const std::string& field)
                       {
                           prompt(field);
                           form[field] = read_input();
                       } );
        return form;
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

#endif // CHOCAN_TERMINAL_INPUT_CONTROLLER_H
