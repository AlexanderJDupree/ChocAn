/* 

File: transaction_tests.cpp

Brief: Unit tests for Transaction class

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
#include <ChocAn/core/entities/transaction.hpp>

TEST_CASE("Constructing Transaction objects", "[constructors], [transaction]")
{
    Data_Gateway::Database_Ptr mock_db = std::make_unique<Mock_DB>();

    // TODO this test will fail if the IDs get changed in the mock DB
    Account member   = mock_db->get_account(6789).value();
    Account provider = mock_db->get_account(1234).value();
    Service back_rub = mock_db->lookup_service(123456).value();

    SECTION("Valid construction of a transaction")
    {
        REQUIRE_NOTHROW(Transaction( provider
                                   , member
                                   , DateTime( Month(11), Day(11), Year(2019))
                                   , back_rub
                                   , "Some comments" ));
    }
    SECTION("Constructor throws if date time is greater than current date")
    {
        REQUIRE_THROWS(Transaction( provider
                                  , member
                                  , DateTime( Month(12), Day(30), Year(4000))
                                  , back_rub
                                  , "some comments"));
    }
    SECTION("Constructor throws if provider or memebr accounts are not of their respective type")
    {
        REQUIRE_THROWS(Transaction( member
                                  , provider
                                  , DateTime( Month(12), Day(20), Year(2011))
                                  , back_rub
                                  , "some comments" ));
    }
}