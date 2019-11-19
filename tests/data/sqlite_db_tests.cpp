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
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/data/sqlite_db.hpp>

TEST_CASE("Constructing SQLite_DB object", "[constructors], [sqlite_db]")
{
    SECTION("SQLite_DB must be provided a file path to a .db file")
    {
        REQUIRE_NOTHROW( SQLite_DB("chocan.db") );
    }
}

TEST_CASE("Serializing Accounts into a values statement")
{
    Mock_DB mock_db;
    SQLite_DB db("chocan.db");

    SECTION("Serializing a provider account")
    {
        std::string expected = "1234, 'Arman', 'Provider', '1234 lame st.', 'Portland', '97030', 'Provider', 'Valid'";
        std::string val = db.serialize_account(mock_db.get_account("1234").value());

        REQUIRE(val == expected);
    }
}