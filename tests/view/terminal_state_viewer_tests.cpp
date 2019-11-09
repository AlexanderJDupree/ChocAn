/* 
File: terminal_state_viewer.cpp

Brief: Unit tests for terminal state viewer component

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
#include <ChocAn/view/terminal_state_viewer.hpp>

TEST_CASE("Terminal State Viewer Construction", "[constructors], [terminal_state_viewer]")
{
    std::stringstream test_stream;
    SECTION("Terminal State viewer requires an ostream& and does not throw")
    {
        REQUIRE_NOTHROW(Terminal_State_Viewer( "tests/view/"
                                             , ".txt"
                                             , test_stream) );
    }
}

TEST_CASE("Render State Views with viewer commands", "[views], [terminal_state_viewer]")
{
    std::stringstream test_stream;
    SECTION("<header>, <footer>, <state_name>, <state_info> composite command test")
    {
        Terminal_State_Viewer viewer ( "tests/view/"
                                     , "-test.txt"
                                     , test_stream );

        viewer.render_state(Login { "Hello World" });

        REQUIRE(test_stream.str() == "== test Login Service ==\n"
                                     "Hello World\n"
                                     "== test footer ==\n"
                                     "> " );
    }
}