/* 
File: terminal_state_viewer.cpp

Brief:  Implementation file for terminal state viewer

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <ChocAn/view/terminal_state_viewer.hpp>

struct View_Table
{
    const char* operator()(const Login&)               { return "login"; }
    const char* operator()(const Exit&)                { return "exit"; }
    const char* operator()(const Provider_Menu&)       { return "provider_menu"; }
    const char* operator()(const Manager_Menu&)        { return "manager_menu"; }
    const char* operator()(const Add_Transaction&)     { return "add_transaction"; }
    const char* operator()(const Confirm_Transaction&) { return "confirm_transaction"; }
};

struct Render_State_Name_Event
{
    const char* operator()(const Exit&)                { return "Exit"; }
    const char* operator()(const Login&)               { return "Login Service"; }
    const char* operator()(const Provider_Menu&)       { return "Provider Menu"; }
    const char* operator()(const Manager_Menu&)        { return "Manager Menu";  }
    const char* operator()(const Add_Transaction&)     { return "Transaction Manager"; }
    const char* operator()(const Confirm_Transaction&) { return "Confrim Transaction"; }
};

struct Render_State_Info_Event
{
    std::string operator()(const Login& login)
    {
        return login.login_status;
    }
    std::string operator()(const Provider_Menu& menu)
    {
        return menu.status;
    }
    std::string operator()(const Manager_Menu& menu)
    {
        return menu.status;
    }
    std::string operator()(const Add_Transaction& transaction)
    {
        std::stringstream stream;

        if(auto maybe_error = transaction.builder->get_last_error())
        {
            stream << maybe_error.value().what() << "\n";
            for (const std::string& err : maybe_error.value().info())
            {
                stream << '\t' << err << '\n';
            }
        }
        stream << "\nEnter " << transaction.builder->get_current_field() << ": ";
        return stream.str();
    }
    std::string operator()(const Confirm_Transaction& confirm)
    {
        std::stringstream stream;

        stream << "Date of Service:  " << confirm.transaction.service_date().month().count() << '-'
                                       << confirm.transaction.service_date().day().count()   << '-' 
                                       << confirm.transaction.service_date().year().count()  << '\n'
               << "\nProvider ID:\t" << confirm.transaction.provider().id() << "\n"
               << "Member ID:\t"   << confirm.transaction.member().id()   << '\n'
               << "\n"
               << "Service:\t"     << confirm.transaction.service().code() << " : "
                                   << confirm.transaction.service().name() << '\n'
               << "Service Fee:\t$"<< confirm.transaction.service().cost().value << '\n'
               << "\nComments:\n"  << confirm.transaction.comments();
        return stream.str();
    }
    std::string operator()(const Exit&)
    {
        return "";
    }
};

Terminal_State_Viewer::Terminal_State_Viewer( std::string&& view_location
                                            , std::string&& file_extension 
                                            , std::ostream& out_stream )
    : current_state  ( Login()        )
    , view_location  ( view_location  )
    , file_extension ( file_extension )
    , out_stream     ( out_stream     )
    , command_table  ({
          { "header",       [&](){ return render_view("header");  } }
        , { "footer",       [&](){ return render_view("footer");  } }
        , { "prompt",       [&](){ return render_prompt();        } }
        , { "empty_prompt", [&](){ return render_prompt("");      } }
        , { "state_info",   [&](){ 
                out_stream << std::visit(Render_State_Info_Event(), current_state);
            } }
        , { "state_name",   [&](){ 
                out_stream << std::visit(Render_State_Name_Event(), current_state);
            } }
    }) 
    {}

void Terminal_State_Viewer::render_prompt(const std::string& prompt)
{
    out_stream << prompt;
}

void Terminal_State_Viewer::render_user_error(const chocan_user_exception& error)
{
    out_stream << error.what() << ":\n";
    std::for_each(error.info().begin(), error.info().end(), [&](const std::string& err)
    {
        out_stream << '\t' << err << '\n';
    });
}
void Terminal_State_Viewer::render_state(const Application_State& state)
{
    current_state = state;

    return this->update();
}

void Terminal_State_Viewer::update()
{
    render_view(std::visit(View_Table(), current_state));
    return;
}

void Terminal_State_Viewer::render_view(const std::string& view_name)
{
    std::string view = view_location + view_name + file_extension;
    std::ifstream file(view);

    if(file.is_open())
    {
        char symbol;
        while (file.get(symbol).good())
        {
            // Display characters until 'start command' character is reached
            if (symbol == '<') { execute_command(read_command(file)); }
            else               { out_stream << symbol; }
        }
    }
    else
    {
        out_stream << "Error: Unable to open view: " << view << std::endl;
    }
    return file.close();
}

std::string Terminal_State_Viewer::read_command(std::ifstream& file)
{
    char symbol;
    std::stringstream command;

    // Consume symbols until end command character is reached
    while(file.get(symbol).good() && symbol != '>')
    {
        command << symbol;
    }
    return command.str();
}

void Terminal_State_Viewer::execute_command(const std::string& command)
{
    try
    {
        command_table.at(command)();
    }
    catch(const std::out_of_range& err)
    {
        out_stream << "Viewer Command: " << command << ", not recognized\n";
    }
    return;
}
