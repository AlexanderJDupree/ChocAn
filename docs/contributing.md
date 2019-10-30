# Contributing Guide

A lot of architecture code was recently added to this project. As such, I recognize that the codebase may seem foreign or even intimidating. For this reason, I am writing this contributing guide in order to enlighten contributors about how to write code 
within the current framework. Hopefully this will also illuminate the purposes and benefits of the current architecture decisions. 

## Design Overview

Firstly, the ChocAn application is designed with a layered system architecture. 

![architecture](architecture_layers.png)

On the bottom we have the **Data Access Layer** which is responsible for communicating with the databse to retrieve/update ChocAn records.
Next is the **Domain Layer**, this layer describes all the business logic for the application, and defines how entities interact with each other.
You'll find the Member/Provider classes and reporting managers all in this layer. Connected to the **Domain Layer** is the **Application Layer**. 
The **Application Layer** manages the control flow of the actual application, and communicates with the **Domain Layer** to validate input and 
perform other actions. Lastly is the **Presentation Layer**, this layer is where all the IO actually happens. Reading input from the user, printing and 
formatting state representation, all occurs here. In fact, the only place where *iostream* should be included in a source file is within the **Presentation Layer**. This guide will deal mostly with the **Application** and **Presentation** layer.

## Application Layer

The **Application Layer** uses a [finite-state machine](https://en.wikipedia.org/wiki/Finite-state_machine) to model user interaction with the application. Essentially, as we interact with the application the behavior of the application changes. For example, when I start up the application I am greeted with a login screen, if I enter the correct login I can transition to a provider menu screen. However, the set of actions I can take are different at the menu screen than the login screen. Using a state machine and implementing the [state pattern](https://en.wikipedia.org/wiki/State_pattern) allows our application to exhibit different behavior depending on the internal state. In terms of the code, the two main files for the state machine are the `State` abstract base class, and the `State_Controller`. 

Lets first look at the `State` abstract base class. 

(include/ChocAn/app/state.hpp)
```c++
class State
{
public:

    typedef std::shared_ptr<State>      State_Ptr;
    typedef std::vector<std::string>    Input_Vector;

    virtual ~State() {};

    virtual State_Ptr evaluate(const Input_Vector& input) = 0;

    virtual State_Info info() const = 0;

    size_t id() const { return typeid(*this).hash_code(); }

    bool operator == (const State& rhs) const { return this->id() == rhs.id(); }

    bool operator < (const State& rhs) const { return id() < rhs.id(); }
};

```

First, we typedef the std::shared_ptr to `State_Ptr`, and std::vector to `Input_Vector`. Typedefs help relieve us of the tedious C++ standard library typenames and allows us to reference an object type in a uniform way. The `State` class also has two pure virtual methods that derived classes must implement. The first is the *evaluate* function. *evaluate* takes in an `Input_Vector` and returns a `State_Ptr`. You can think of this as a function that maps *inputs* to *states*. How it maps these inputs is up to you, but generally invalid or unrecognizable input should map back to itself. The second virtual method is *info*, which returns a `State_Info` object that holds optional data about the state.

The `State_Controller` just manages what the current state is and feeds input into the current state to evaluate. So in the main module we continue to loop until the `State_Controller` reaches an *end* state and with each iteration we display the states **view** and have the `State_Controller` transition state to the evaluation of the states input.

```c++
int main () {

    State_Controller controller;
    Terminal_State_Viewer viewer;

    // Loop until we hit an end state
    while(!controller.end_state())
    {
        // render the view of the current state
        viewer.render_state(controller.current_state());

        // transition into a new state depending on the evaluation of the input collected from the viewer
        controller.transition(viewer.interact());
    }

    return 0;
}

```
## Making New States

Now that we understand the basic design of the application layer, lets add a new state! As per the requirements, we need to simulate the providers terminal and the managers terminal. As such, we will need a `Provider_Menu_State` and a `Manager_Menu_State`. The roles of these states will be to transition us into one of the activities that a provider or manager will perform. I already wrote a `Provider_Menu_State` so lets implement the `Manager_Menu_State`. 

First, lets create a new branch where our work will go. 

```bash
git checkout -b manager_menu
```

Then lets create the required source and header files:

```bash
touch include/ChocAn/app/states/manager_menu_state.hpp
touch src/app/manager_menu_state.cpp
```

To implement the `State` interface we need to include the header file, inherit from `State` and define the two interface methods. It will look like this when it's done:


(include/ChocAn/app/states/manager_menu_state.hpp)
```c++
#ifndef CHOCAN_MANAGER_MENU_STATE_H
#define CHOCAN_MANAGER_MENU_STATE_H

#include <ChocAn/app/state.hpp>

class Manager_Menu_State : public State
{
public:

    State_Ptr evaluate(const Input_Vector& input) ;

    State_Info info() const;+
};

#endif  // CHOCAN_MANAGER_MENU_STATE_H
```

Now in the `.cpp` file we can define the behavior for this state. For now we will keep it simple, we will say `"exit"` leaves the application, `"0"` goes back to the login screen and everything else loops back on itself. To do this we need to include the header files for those states and return the correct `State_Ptr` from the `evaluate` function. It should look like this:

(src/app/manager_menu_state.cpp)
```c++

#include <ChocAn/app/states/exit_state.hpp>
#include <ChocAn/app/states/login_state.hpp>
#include <ChocAn/app/states/manager_menu_state.hpp>

State::State_Ptr Manager_Menu_State::evaluate(const Input_Vector& input)
{
    if(input.at(0) == "exit")
    {
        return std::make_unique<Exit_State>();
    }
    if(input.at(0) == "0")
    {
        return std::make_unique<Login_State>();
    }
    return std::make_unique<Manager_Menu_State>();
}

State_Info Manager_Menu_State::info() const
{
    return State_Info();
}
```

First notice we use the templated `std::make_unique` function to return the `State_Ptr`. This is because we are using smart pointers (pointers that deallocate the object when the pointer goes out of scope) and `std::make_unique` is the recommended way of creating these pointers. Also, the `info()` function just returns a default `State_Info` object, since there is no information we want to communicate yet. 

## Testing Our New State

Now that we created the new state, we should test it! There isn't much to test right now, but we do know that invalid input should return the same state, `"exit"` should return an `Exit_State`, and `"0"` should return a `Login_State`. I already wrote tests for this behavior for previous states, so we will only need to include our new state in the test case.

Open `tests/app/state_tests.cpp` and look for the `"State Behavior"` test case. At the top level we defined the states and added them to a vector for testing. Lets add a couple lines to include the new state.

(tests/app/state_tests.cpp)
```c++

// Add this include 
#include <ChocAn/app/states/manager_menu_state.hpp>

TEST_CASE("State Behavior", "[state]")
{
    Login_State login_state;
    Exit_State  exit_state;
    Manager_Menu_State manager_menu_state; // Add this line
    Provider_Menu_State provider_menu_state;

    std::vector<State*> states { &login_state
                               , &exit_state
                               , &manager_menu_state // Add this line
                               , &provider_menu_state
                               }; 

    /* Test Section */
}
```

Then in the test section `"States do not transition on evaluation of invalid input"` copy add this assertion:

```c++
    SECTION("States do not transition on evaluation of invalid input")
    {
        REQUIRE(*(Login_State().evaluate({ "Bad Input" })) == Login_State());
        REQUIRE(*(Exit_State().evaluate({ "Bad_Input" })) == Exit_State());
        REQUIRE(*(Provider_Menu_State().evaluate({ "Bad_Input" })) == Provider_Menu_State());

        // Add this assertion
        REQUIRE(*(manager_menu_state.evaluate({ "Bad_Input" })) == Manager_Menu_State());
    }
```

Also, lets test to make sure the state returns to the login on input `"0"`. To do this add a new test section within the test case and use the `REQUIRE` macro. Like this:

```c++
SECTION("Manager Menu State returns to login on input '0'")
{
    REQUIRE(*(manager_menu_state.evaluate({ "0" })) == Login_State());
}
```

## Building and Running the tests

Now that we added the tests, lets build and run the test target to make sure everything works as expected.

Since we added new files to the project we need to update the Makefiles to include these files in our build targets. We can do this by manually editing the Makefiles, or use the **Premake5** build system to generate the makefiles for us. If you don't have **Premake5** you can [download the binary here](https://premake.github.io/download.html).

With **Premake5** installed and included in our *PATH* we can run the following command to update the Makefiles:

```bash
premake5 gmake
```

Now to build the project and run the unit tests simply run:

```bash
./build.sh
```

If all goes well you will see this output:

```bash
  « Debug Configuration Tests »                                                                                         

         ❯ echo && bin/tests/debug_tests 
===============================================================================
All tests passed (53 assertions in 12 test cases)

 ✔︎  0.0s 

  « Release Configuration Tests »                                                                                       

         ❯ echo && bin/tests/release_tests 
===============================================================================
All tests passed (53 assertions in 12 test cases)

 ✔︎  0.0s 
```
If we want to make sure our tests were actually run we can re-run the test binary with the following command:

```bash
./bin/tests/debug_tests '[state]' -s 
```

This command runs only the tests with the `[state]` tag and `-s` prints out a test summary. 

## Creating a View for the State

Now we have implemented and tested the behavior for our `Manger_Menu_State`, we need a way to represent that state to the user. This is where the **Presentation Layer** comes into play. The workhorse of the **Presentation Layer** is the `Terminal_State_Viewer` class which implements the `State_Viewer` interface defined in the `Application Layer`. The `Terminal_State_Viewer` presents `State` viewing as console output. This is done by storing the `views` as plain text files and printing out a specific file when we receive a `State`.

Lets take an abbreviated look at the `Terminal_State_Viewer` definition:

(include/ChocAn/view/terminal_state_viewer.hpp) 
```c++
#ifndef CHOCAN_TERMINAL_STATE_VIEWER_H
#define CHOCAN_TERMINAL_STATE_VIEWER_H

#include <map>
#include <functional>
#include <ChocAn/app/state_viewer.hpp>

class Terminal_State_Viewer : public State_Viewer
{
public:

    typedef std::map<size_t, std::string>                View_Table;
    typedef std::map<std::string, std::function<void()>> Command_Table;

    // Returns the input vector used for the current state
    const State::Input_Vector& interact();

    // Looks up state view in view_table, renders the view.
    void render_state(const State& state);

private:

    /* Other Utility Methods */

    const Command_Table     command_table;
    static const View_Table view_table;

};

#endif // CHOCAN_TERMINAL_STATE_VIEWER_H
```

Here we have some handy typedefs again, the first of which defines a `View_Table`. A `View_Table` is an associative data structure that maps `State` ids to file names. This is used in the `render_state` function which lookups the `State` ID in the `view_table` and prints out the file associated with that state. The other typedef defines a `Command_Table`. The `Command_Table` maps strings to specific viewer functions, I'll show you how it works in a bit. 

First, we need to create a view for the state:

```bash
touch views/manager_menu.txt
```
Now we can design the UI for the state. I'm going to populate the menu with all the options the application will eventually have, even though they don't go anywhere yet. 

We want the menu to look something like this at runtime. 

```bash
======================== ChocAn Services =========================

0) Return to Login Screen

1) View Provider/Member Profile

2) Create Provider Account

3) Create Member Account

4) Delete Provider/Member Account

5) Generate Summary Reports

( Entering 'exit' will close the application )

> 
```

This menu looks good. However, there are lot of UI elements that are shared from view to view. For example, the `ChocAn Services` header should be reused, as well as the footer message. Also, how do we indicate when and where to prompt the user for input? We need some way to communicate to the viewer to render a specific UI element or to catch user input. This is where the `Command_Table` is leveraged. As of right now the `Command_Table` only has a few basic commands which can be found in the `Terminal_State_Viewer` source file. We will use the `header`, `footer`, and `prompt` command for our menu.

To create a menu that will look like the one we wanted we just need to edit the view file to contain the following lines:

(views/manager_menu.txt)
```bash
<header>

0) Return to Login Screen

1) View Provider/Member Profile

2) Create Provider Account

3) Create Member Account

4) Delete Provider/Member Account

5) Generate Summary Reports

<footer>

<prompt>
```

The last thing we need to do is add the view to the `View_Table`. We can do this in the `Terminal_State_Viewer` source file.

(src/view/terminal_state_viewer.cpp)
```c++
#include <ChocAn/app/states/manager_menu_state.hpp> // <-- Add this include

const Terminal_State_Viewer::View_Table Terminal_State_Viewer::view_table 
{
    { Login_State().id(),         "login"         },
    { Exit_State().id(),          "exit"          },
    { Provider_Menu_State().id(), "provider_menu" },
    { Manager_Menu_State().id(),  "manager_menu"  } // <-- Add this line
};
```

And finally we can build and run the application. 

```bash
./build.sh && ./bin/debug/ChocAn_debug  
```

Note: The password to get to the Manager Menu is '5678' for right now. 


## Wrapping Up

This is only a basic introduction to the capabilities provided through this architecture. Having the representation removed from the state behavior allows us to develop the UI independently of the application logic. Furthermore, this architecture scales easily to accomodate more application states and associted views. Also, this architecture naturally supports features like nested menus, batch form input, and view composition. This framework however, is not finished and is in need of more tests and refactoring. 

As a final example, if you wanted to create a form for a Member profile or something here's how you would do it:

```bash
<header>

<status_msg>

Enter Name: <prompt>

Enter City: <prompt>
Enter State: <prompt>
Enter Street Address: <prompt>
```

This view would collect all user input into the `Input_Vector` and have the associated state unpack and evaluate that input. The state can also provide a status message via the `State_Info` method, which can be displayed via the `<status_msg>` viewer command. Also, we can always add more commands for the viewer to use in order to handle different types of interaction with the user. 