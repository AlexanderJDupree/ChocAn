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

        std::vector<std::string> expected_data { "John Doe"
                                               , "1234 Cool St."
                                               , "30" };

        Input_Controller::Form_Data expected_form
        {
            { "Name"   , "John Doe" },
            { "Address", "1234 Cool St." },
            { "Age"    , "30"}
        };

        // Populate the stream with expected data
        std::for_each(expected_data.begin(), expected_data.end(),
                     [&](const std::string&  data)
                     {
                         test_stream << data << "\n";
                     } );

        Terminal_Input_Controller input_controller(test_stream);

        REQUIRE(input_controller.read_form({ "Name", "Address", "Age" }, callback) == expected_form);
    }
    SECTION("Input controller reads exit when eof flag is set")
    {
        test_stream << '\0';

        Terminal_Input_Controller input_controller(test_stream);

        REQUIRE(input_controller.read_input() == "exit");
    }
    SECTION("Confirmation of input returns true when input is yes")
    {
        test_stream << "Yes\n";

        Terminal_Input_Controller input_controller(test_stream);

        REQUIRE(input_controller.confirm_input().value());
    }
    SECTION("Confirmation of input returns false when input is no")
    {
        test_stream << "No\n";

        Terminal_Input_Controller input_controller(test_stream);

        REQUIRE_FALSE(input_controller.confirm_input().value());
    }
    SECTION("Confirmation of input returns nothing when input isn't intelligible")
    {
        test_stream << "garbage\n";

        Terminal_Input_Controller input_controller(test_stream);

        REQUIRE_FALSE(input_controller.confirm_input());
    }
}
