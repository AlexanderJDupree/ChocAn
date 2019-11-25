/* 
File: transaction_builder_tests.cpp

Brief:  Transaction builder unit tests

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <stack>
#include <catch.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/core/utils/transaction_builder.hpp>

TEST_CASE("Setting Member Account Field", "[transaction_builder]")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    Transaction_Builder builder(db);

    SECTION("Set Member Account Field pushes error when given provider ID")
    {
        builder.set_member_acct_field("1234");

        REQUIRE(builder.get_last_error().has_value());
    }
    SECTION("Set Member Account Field pushes error when given invalid input")
    {
        builder.set_member_acct_field("garbage");

        REQUIRE(builder.get_last_error().has_value());
    }
    SECTION("Set Member Account Field pushes error when given a suspended Member ID")
    {
        builder.set_member_acct_field("9876");
        REQUIRE(builder.get_last_error().has_value());
    }
    SECTION("Set Member Account Field does not push an error when given a valid ID")
    {
        builder.set_member_acct_field("6789");
        REQUIRE_FALSE(builder.get_last_error().has_value());
    }
}

TEST_CASE("Setting Provider Account Field", "[transaction_builder]")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    Transaction_Builder builder(db);

    SECTION("Set Provider Account Field pushes error when given a member ID")
    {
        builder.set_provider_acct_field("9876");

        REQUIRE(builder.get_last_error().has_value());
    }
    SECTION("Set Provider Account Field pushes error when given invalid input")
    {
        builder.set_provider_acct_field("garbage");
        REQUIRE(builder.get_last_error().has_value());
    }
    SECTION("Set Provider Account Field does not push error when given a valid ID")
    {
        builder.set_provider_acct_field("1234");
        REQUIRE_FALSE(builder.get_last_error().has_value());
    }
}

TEST_CASE("Setting Service Date Field", "[transaction_builder]")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    Transaction_Builder builder(db);

    SECTION("Set Service Date Field pushes error when given invalid input")
    {
        builder.set_service_date_field("invalid input");
        REQUIRE(builder.get_last_error().has_value());
    }
    SECTION("Set Service Date Field pushes error when given datetime in incorrect format")
    {
        builder.set_service_date_field("12/31/2019");
        REQUIRE(builder.get_last_error().has_value());
    }
    SECTION("Set Service Date Field pushes error when given datetime with dates in incorrect order")
    {
        builder.set_service_date_field("31-12-2019");
        REQUIRE(builder.get_last_error().has_value());
    }
    SECTION("Set Service Date Field pushes error when given a future datetime")
    {
        builder.set_service_date_field("10-10-4020");
        REQUIRE(builder.get_last_error().has_value());
    }
    SECTION("Set Service Date does NOT pushe error when given a valid datetime")
    {
        builder.set_service_date_field("10-10-2019");
        REQUIRE_FALSE(builder.get_last_error().has_value());
    }
}

TEST_CASE("Setting Service Field", "[transaction_builder]")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    Transaction_Builder builder(db);

    SECTION("Set Service Field does NOT push error when given a valid service code")
    {
        builder.set_service_field("123456");
        REQUIRE_FALSE(builder.get_last_error().has_value());
    }
    SECTION("Set Service Field pushes error when given invalid service code")
    {
        builder.set_service_field("garbage");
        REQUIRE(builder.get_last_error().has_value());
    }
}

TEST_CASE("Setting Comments Field", "[transaction_builder]")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    Transaction_Builder builder(db);

    SECTION("Set Comments Field does not push error when comments are less than 100 characters")
    {
        builder.set_comments_field("hello world");
        REQUIRE_FALSE(builder.get_last_error().has_value());
    }
    SECTION("Set Comments Field throws when comments are greater than 100 chracters")
    {
        builder.set_comments_field(std::string('n', 101));
        REQUIRE(builder.get_last_error().has_value());
    }
}

TEST_CASE("Building Transactions", "[transaction_builder]")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    Transaction_Builder builder(db);

    SECTION("Builder throws if transaction is not buildable")
    {
        REQUIRE_THROWS_AS(builder.build(), chocan_user_exception);
    }
    SECTION("Builder builds a Transaction when it is buildable")
    {
        std::stack<std::string> inputs( { "comments" 
                                        , "123456" // service code
                                        , "10-10-2019" // date of service
                                        , "6789" // member ID
                                        , "1234" // Provider id
                                        } );
        while(!builder.buildable())
        {
            builder.set_current_field(inputs.top());
            inputs.pop();
        }

        REQUIRE_NOTHROW(builder.build());
    }
}
