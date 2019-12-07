/* 
 
File: main.cpp

Brief: Provides the entry point for the ChocAn data processing application.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/LinkedListsCPP
 
*/

#define CHOCAN_VERSION_MAJOR 1
#define CHOCAN_VERSION_MINOR 1
#define CHOCAN_VERSION_PATCH 0

#include <fstream>
#include <clara.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/data/sqlite_db.hpp>
#include <ChocAn/app/state_controller.hpp>
#include <ChocAn/view/terminal_state_viewer.hpp>
#include <ChocAn/view/terminal_input_controller.hpp>

int run(std::istream& in_stream, std::ostream& out_stream, bool in_memory, bool compact);

int main (int argc, char ** argv) 
{
    using namespace clara;

#ifdef DEBUG
    bool in_memory = true;
#else
    bool in_memory = false;
#endif

    bool show_help = false;
    bool compact   = false;
    std::string input_file = "";

    auto cli = Help(show_help)
             | Opt(input_file, "Input File")
               ["-i"]["--input-file"]("Location of input, defaults to STDIN")
             | Opt(compact)
               ["-c"]["--compact-output"]("Don't clear screen with newlines on each iteration")
             | Opt(in_memory)
               ["-m"]["--in-memory"]("Run the database in memory, defaults to false for Release build");

    auto result = cli.parse( { argc, argv } );
    if(!result || show_help) 
    { 
        std::cerr << cli << std::endl;
        return 1;
    }

    std::ifstream in_stream(input_file);
    if(in_stream.is_open())
    {
        int exit_code = run(in_stream, std::cout, in_memory, compact);
        in_stream.close();
        return exit_code;
    }
    return run(std::cin, std::cout, in_memory, compact);
}

int run(std::istream& in_stream, std::ostream& out_stream, bool in_memory, bool compact)
{
    ChocAn::Database_Ptr db = (in_memory) ? std::make_unique<SQLite_DB>(":memory:", "chocan_schema.sql")
                                          : std::make_unique<SQLite_DB>("chocan.db");

    State_Controller controller ( std::make_unique<ChocAn>(db)
                                , std::make_unique<Terminal_State_Viewer>(compact, out_stream)
                                , std::make_unique<Terminal_Input_Controller>(in_stream) );

    // TODO exit loop if viewer can't open view
    while(!controller.end_state())
    {
        controller.interact();
    }

    return 0;
}