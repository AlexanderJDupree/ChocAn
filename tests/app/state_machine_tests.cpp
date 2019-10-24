/* 
File: state_controller_tests.cpp

Brief: Unit tests for State Controller machine

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn

*/

#include <catch.hpp>
#include <ChocAn/app/state_controller.hpp>


class Q1 : public State
{
public: 
    State_Ptr evaluate(const std::string& input);
};

class Q2 : public State
{
public: 
    State::State_Ptr evaluate(const std::string& input);
};

State::State_Ptr Q1::evaluate(const std::string& input)
{
    if(input == "1")
    {
        return std::make_unique<Q2>();
    }
    return std::make_unique<Q1>();
}

State::State_Ptr Q2::evaluate(const std::string& input)
{
    if(input == "0")
    {
        return std::make_unique<Q1>();
    }
    return std::make_unique<Q2>();
}

TEST_CASE("State Controller construction", "[constructors], [state_controller]")
{
    SECTION("State_Controller is constructed with initial state")
    {
        State_Controller controller { std::make_unique<Q1>() , 
                                    { std::make_unique<Q2>() } };

        REQUIRE(controller.current_state() == Q1());
    }
}
TEST_CASE("State controller transitions state", "[transition], [state_controller]")
{

    State_Controller controller { std::make_unique<Q1>() , 
                                { std::make_unique<Q2>() } };

    SECTION("Invalid input does not change state")
    {
        controller.transition("not recognized");

        REQUIRE(controller.current_state() == Q1());
    }
    SECTION("Valid input transitions state")
    {
        controller.transition("1");

        REQUIRE(controller.current_state() == Q2());
    }
    SECTION("Accept state can be transitioned into")
    {
        controller.transition("1");

        REQUIRE(controller.end_state());
    }
}
