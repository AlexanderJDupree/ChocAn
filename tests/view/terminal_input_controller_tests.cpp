/* 
File: terminal_input_controller_tests.cpp

Brief: Unit tests for Terminal Input Controller utility

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn

*/

#include <sstream>
#include <catch.hpp>
#include <ChocAn/view/terminal_input_controller.hpp>

TEST_CASE("Retrieving input from the Input Controller", "[input_controller]")
{
    std::stringstream test_stream;
    SECTION("Input controller reads input from stream")
    {

        test_stream << "Hello World\n";

        Terminal_Input_Controller input_controller(test_stream);

        REQUIRE(input_controller.read_input() == "Hello World");

    }
    SECTION("Input Controller can read batch form input")
    {
        auto callback = [](const std::string&){ };

        Input_Controller::Form_Data expected_data 
        {
            { "Name",    "John Doe" },
            { "Address", "1234 Cool St."},
            { "Age",     "30" }
        };

        Input_Controller::Form_Data test_form 
        {
            { "Name",    "" },
            { "Address", "" },
            { "Age",     "" }
        };

        // Populate the stream with expected data
        std::for_each(expected_data.begin(), expected_data.end(),
                     [&](Input_Controller::Field_Data field)
                     {
                         test_stream << field.second << "\n";
                     } );

        Terminal_Input_Controller input_controller(test_stream);

        REQUIRE(input_controller.read_form(test_form, callback) == expected_data);
    }
}