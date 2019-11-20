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

#include <cstdio>
#include <iostream>
#include <catch.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/data/sqlite_db.hpp>

const char* TEST_DB = ":memory:";
const char* CHOCAN_SCHEMA = "chocan_schema.sql";

TEST_CASE("Constructing SQLite_DB object", "[constructors], [sqlite_db]")
{
    SECTION("SQLite_DB must be provided a file path to a .db file")
    {
        REQUIRE_NOTHROW( SQLite_DB(TEST_DB) );
    }
    SECTION("SQLite_DB does not throw when constructed with a valid schema file")
    {
        REQUIRE_NOTHROW( SQLite_DB(TEST_DB, CHOCAN_SCHEMA) );
    }
    SECTION("SQLite_DB does throw when constructed with an invalid schema file")
    {
        REQUIRE_THROWS_AS( SQLite_DB(TEST_DB, "not a file"), chocan_db_exception );
    }
}

TEST_CASE("Serializing Accounts into a values statement", "[serialize_account], [sqlite_db]")
{
    Mock_DB mock_db;
    SQLite_DB db(TEST_DB, CHOCAN_SCHEMA);

    SECTION("Serializing a provider account")
    {
        std::string expected = "1234, 'Arman', 'Provider', '1234 lame st.', 'Portland', 'OR', '97030', 'Provider', 'Valid'";
        std::string val = db.serialize_account(mock_db.get_account("1234").value());

        REQUIRE(val == expected);
    }
    SECTION("Serializing a manager account")
    {
        std::string expected = "5678, 'Dan', 'Manager', '1234 cool st.', 'Portland', 'OR', '97030', 'Manager', 'Valid'";
        std::string val = db.serialize_account(mock_db.get_account("5678").value());

        REQUIRE(val == expected);
    }
    SECTION("Serializing a valid member account")
    {
        std::string expected = "6789, 'Alex', 'Member', '1234 Meh st.', 'Portland', 'OR', '97030', 'Member', 'Valid'";
        std::string val = db.serialize_account(mock_db.get_account("6789").value());

        REQUIRE(val == expected);
    }
    SECTION("Serializing a suspended member account")
    {
        std::string expected = "9876, 'Jane', 'Member', '1234 awesome st.', 'Portland', 'OR', '97030', 'Member', 'Suspended'";
        std::string val = db.serialize_account(mock_db.get_account("9876").value());

        REQUIRE(val == expected);
    }
}

TEST_CASE("Creating Accounts", "[create_account], [sqlite_db]")
{
    Mock_DB mock_db;
    SQLite_DB db(TEST_DB, CHOCAN_SCHEMA);

    SECTION("Create_account returns true on success")
    {
        REQUIRE(db.create_account(mock_db.get_account("1234").value()));
    }
}

TEST_CASE("Deleting accounts", "[delete_account], [sqlite_db]")
{
    Mock_DB mock_db;
    SQLite_DB db(TEST_DB, CHOCAN_SCHEMA);

    db.create_account(mock_db.get_account("1234").value());

    SECTION("Delete accounts returns true when the operation succeeds")
    {
        REQUIRE(db.delete_account(1234));

        SECTION("Deleted accounts ID's no longer exist in database")
        {
            REQUIRE_FALSE(db.id_exists(1234));
        }
    }
}

TEST_CASE("Check if ID exists", "[id_exists], [sqlite_db]")
{
    Mock_DB mock_db;
    SQLite_DB db(TEST_DB, CHOCAN_SCHEMA);

    SECTION("id_exists returns true if there is an account with the id")
    {
        Account test = mock_db.get_account(1234).value();

        db.create_account(test);

        REQUIRE(db.id_exists(test.id()));
    }
    SECTION("id_exists returns false if there is no account with the id")
    {
        REQUIRE_FALSE(db.id_exists(0));
    }
}

TEST_CASE("Looking up services in service directory", "[lookup_service], [sqlite_db]")
{
    SQLite_DB db(TEST_DB, CHOCAN_SCHEMA);

    SECTION("Returns de-serialized service object when given a valid service code")
    {
        // Chocan schema provides default data for service 123456
        Service service = db.lookup_service(123456).value();

        REQUIRE(service.code() == 123456);
        REQUIRE(service.cost().value == 39.99);
        REQUIRE(service.name() == "Back Rub");
    }
    SECTION("Returns None when given an invalid_service code")
    {
        REQUIRE_FALSE(db.lookup_service(0));
    }
}
