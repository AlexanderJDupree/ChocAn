/* 

File: address_tests.cpp

Brief: Unit tests for address class and associated utility classes.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/core/entities/address.hpp>

TEST_CASE("Constructors for address classes", "[constructors], [address]")
{
    const std::string valid_street = "1234 A. St";
    const std::string valid_city   = "Portland";
    const std::string valid_state  = "OR";
    const unsigned    valid_zip    = 97030;

    SECTION("Construct valid address object")
    {
        REQUIRE_NOTHROW( Address( valid_street
                                , valid_city
                                , valid_state
                                , valid_zip ) );
    }
    SECTION("Missing street address")
    {
        REQUIRE_THROWS_AS( Address( ""
                                  , valid_city
                                  , valid_state
                                  , valid_zip )
                                  , invalid_address );
    }
    SECTION("Street address too long")
    {
        REQUIRE_THROWS_AS( Address( std::string ('c', 26)
                                  , valid_city
                                  , valid_state
                                  , valid_zip )
                                  , invalid_address );
    }
    SECTION("Missing city")
    {
        REQUIRE_THROWS_AS( Address( valid_street
                                  , ""
                                  , valid_state
                                  , valid_zip )
                                  , invalid_address );
    }
    SECTION("City is too long")
    {
        REQUIRE_THROWS_AS( Address( valid_street
                                  , std::string ('c', 15)
                                  , valid_state
                                  , valid_zip )
                                  , invalid_address );
    }
/*
    SECTION("City is not alphabetic")
    {
        REQUIRE_THROWS_AS( Address( valid_street
                                  , std::string ("almost9valid")
                                  , valid_state
                                  , valid_zip )
                                  , invalid_address );
    }

*/
    SECTION("State must be in abbreviated 2 character format")
    {
        REQUIRE_THROWS_AS( Address( valid_street
                                  , valid_city
                                  , ""
                                  , valid_zip )
                                  , invalid_address );
    }
    SECTION("State must be in abbreviated 2 character format")
    {
        REQUIRE_THROWS_AS( Address( valid_street
                                  , valid_city
                                  , "ABC"
                                  , valid_zip )
                                  , invalid_address );
    }
    SECTION("Zip code must be 5 digits")
    {
        REQUIRE_THROWS_AS( Address( valid_street
                                  , valid_city
                                  , valid_state
                                  , 1234 )
                                  , invalid_address );
    }
    SECTION("Zip code must be 5 digits")
    {
        REQUIRE_THROWS_AS( Address( valid_street
                                  , valid_city
                                  , valid_state
                                  , 123456 )
                                  , invalid_address );
    }
}

TEST_CASE("Address equality operators", "[operators], [address]")
{
    Address address ( "1234 Cool St"
                    , "Portland"
                    , "OR"
                    , 97030 );

    Address same_address ( "1234 Cool St"
                         , "Portland"
                         , "OR"
                         , 97030 );

    Address diff_address ( "1234 Lame St"
                         , "Dallas"
                         , "TX"
                         , 97050 );

    SECTION("Addresses are equal when all fields are equal")
    {
        REQUIRE(address == same_address);
    }
    SECTION("Addresses are not equal when any field is different")
    {
        REQUIRE(address != diff_address);
    }
}

TEST_CASE("Invalid Address Exception", "[address], [exceptions]")
{
    SECTION("Exception contains a message for each type of error")
    {
        chocan_user_exception::Info info;
        try
        {
            Address( "", "", "", 0);
        }
        catch(const invalid_address& err)
        {
            info = err.info();
            REQUIRE(std::string(err.what()) == "Invalid address values");
        }
        REQUIRE(info.size() == 4);
    }

}