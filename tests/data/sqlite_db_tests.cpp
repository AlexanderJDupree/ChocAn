/* 

File: sqlite_db_tests.cpp

Brief: Unit tests for sqlite_db connector class

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/data/sqlite_db.hpp>

TEST_CASE("Constructing SQLite_DB object", "[constructors], [sqlite_db]")
{
    SECTION("SQLite_DB must be provided a file path to a .db file")
    {
        REQUIRE_NOTHROW( SQLite_DB("chocan.db") );
    }
}