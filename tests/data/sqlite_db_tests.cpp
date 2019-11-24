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
#include <ChocAn/core/entities/transaction.hpp>

// Will instruct sqltie3 to construct temp DB in memory only
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

TEST_CASE("Retrieving accounts from the database", "[get_account], [sqlite_db]")
{
    /* 
    Note: chocan_schema.sql comes with dummy data. 
    123456789 belongs to a manager account
    123123123 belongs to a member account
    123451234 belongs to a provider account
    */
    
    SQLite_DB db(TEST_DB, CHOCAN_SCHEMA);

    SECTION("Retrieve accounts returns a filled optional object on success")
    {
        REQUIRE(db.get_account(123456789));
    }
    SECTION("Retrieival fails if account does not exist")
    {
        REQUIRE_FALSE(db.get_account("000"));
    }
    SECTION("Retrieval fails if the ID string is not convertible to int")
    {
        REQUIRE_FALSE(db.get_account("garbage"));
    }
    SECTION("Retrieval of a member account")
    {
        REQUIRE(db.get_member_account(123123123));
    }
    SECTION("Member account retrieval fails when given a non member ID")
    {
        REQUIRE_FALSE(db.get_member_account("123456789"));
    }
    SECTION("Retrieval of a provider account")
    {
        REQUIRE(db.get_provider_account(123451234));
    }
    SECTION("Member account retrieval fails when given a non provider ID")
    {
        REQUIRE_FALSE(db.get_provider_account("123456789"));
    }
    SECTION("Retrieval of a manager account")
    {
        REQUIRE(db.get_manager_account(123456789));
    }
    SECTION("Member account retrieval fails when given a non manager ID")
    {
        REQUIRE_FALSE(db.get_manager_account("123123123"));
    }
}

TEST_CASE("Add transactions to database", "[add_transaction], [sqlite_db]")
{
    SQLite_DB db(TEST_DB, CHOCAN_SCHEMA);

    Transaction transaction ( db.get_provider_account(123451234).value()
                            , db.get_member_account(123123123).value()
                            , DateTime( Day(23), Month(11), Year(2019))
                            , db.lookup_service("123456").value()
                            , "comments" );

    SECTION("add transaction returns the transaction ID on success")
    {
        REQUIRE(db.add_transaction(transaction) != 0);
    }
    SECTION("Add transaction returns 0 if the transaction ID already exists")
    {
        db.add_transaction(transaction);

        REQUIRE(db.add_transaction(transaction) == 0);
    }
}

TEST_CASE("Retrieving the service directory", "[service_directory], [sqlite_db]")
{
    SQLite_DB db(TEST_DB, CHOCAN_SCHEMA);
    SQLite_DB no_schema_db(TEST_DB);

    SECTION("service directory returns a populated table on success")
    {
        REQUIRE_FALSE(db.service_directory().empty());
    }
    SECTION("service directory returns an empty table on failure")
    {
        REQUIRE(no_schema_db.service_directory().empty());
    }
}