/* 

File: account_report_tests.cpp

Brief: Unit tests for parsers utility

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
#include <ChocAn/core/reporter.hpp>
#include <ChocAn/core/entities/datetime.hpp>
#include <ChocAn/core/entities/account_report.hpp>

TEST_CASE("Creating Provider Reports", "[provider_report], [constructors]")
{
    using Transactions = Data_Gateway::Transactions;

    Mock_DB mock_db;

    Account provider = mock_db.get_provider_account(1234).value();
    Account member   = mock_db.get_member_account(6789).value();

    Transactions empty_transactions;
    // Mock DB holds transaction data for the Provider with ID 1234
    Transactions transactions = mock_db.get_transactions(DateTime(0), DateTime::get_current_datetime(),provider);
    Transactions all_transactions = mock_db.get_transactions(DateTime(0), DateTime::get_current_datetime());

    SECTION("Provider Report is constructed with a provider account and a list of related transactions")
    {
        REQUIRE_NOTHROW(Provider_Report(provider, transactions));
    }
    SECTION("Provider Report can be constructed with an empty transaction list")
    {
        REQUIRE_NOTHROW(Provider_Report(provider, empty_transactions));
    }
    SECTION("Provider Report cannot be constructed with a non-member account")
    {
        REQUIRE_THROWS_AS(Provider_Report(member, empty_transactions), std::logic_error);
    }
    SECTION("Provider Report cannot be constructed with transacitons that are not related to the provider")
    {
        REQUIRE_THROWS_AS(Provider_Report(member, all_transactions), std::logic_error);
    }
}

TEST_CASE("Calculating total fees owed and services rendered", "[provider_report], [total_fee], [services_rendered]")
{
    using Transactions = Data_Gateway::Transactions;

    Mock_DB mock_db;

    Account provider = mock_db.get_provider_account(1234).value();

    // Mock DB holds transaction data for the Provider with ID 1234
    Transactions transactions = mock_db.get_transactions(DateTime(0), DateTime::get_current_datetime(),provider);

    Provider_Report report (provider, transactions);
    Provider_Report empty  (provider, { });

    SECTION("total fee owed is the sum of the cost of each transaction")
    {
        USD service_fee { 0.0 };
        for (const auto& t : transactions)
        {
            service_fee += t.service().cost();
        }

        REQUIRE(report.total_fee().value == service_fee.value);
    }
    SECTION("The total fee owed for no transactions is $0")
    {

        REQUIRE(empty.total_fee().value == 0);
    }
    SECTION("The number of services rendered is the length of the transactions list")
    {
        REQUIRE(report.services_rendered() == transactions.size());
    }
    SECTION("The number of services rendered for no transactions is 0")
    {
        REQUIRE(empty.services_rendered() == 0);
    }
}

TEST_CASE("Constructing Summary Reports", "[summary_report], [constructors]")
{
    using Provider_Activity = Summary_Report::Provider_Activity;

    Mock_DB mock_db;

    DateTime start(0);
    DateTime current = DateTime::get_current_datetime();

    Account provider1 = mock_db.get_provider_account(1234).value();
    Account provider2 = mock_db.get_provider_account(1111).value();

    // Mock DB holds transaction data for the Provider with ID 1234
    Provider_Activity activity { { provider1, mock_db.get_transactions(start, current,provider1) }
                               , { provider2, mock_db.get_transactions(start, current,provider2) } };
    
    SECTION("Summary Reports are constructed with a list of Provider Reports")
    {
        REQUIRE_NOTHROW(Summary_Report(start, current, activity));
    }
    SECTION("Summary Reports can be constructed with no reports")
    {
        REQUIRE_NOTHROW(Summary_Report(start, current, {} ));
    }
}

TEST_CASE("Calculating total cost and total number of services rendered")
{
    using Provider_Activity = Summary_Report::Provider_Activity;

    Mock_DB mock_db;

    Reporter reporter(std::make_shared<Mock_DB>(mock_db));

    Account provider1 = mock_db.get_provider_account(1234).value();
    Account provider2 = mock_db.get_provider_account(1111).value();

    // Compile the provider reports for the test
    Provider_Report report1 { reporter.gen_provider_report(DateTime(0), DateTime::get_current_datetime(),provider1) };
    Provider_Report report2 { reporter.gen_provider_report(DateTime(0), DateTime::get_current_datetime(),provider2) };

    Provider_Activity activity { report1, report2 };

    // Generate summary report with reporter
    Summary_Report summary = reporter.gen_summary_report(DateTime(0), DateTime::get_current_datetime());

    SECTION("The number of providers involved is the length of the activity list")
    {
        REQUIRE(summary.num_providers() == activity.size());
    }
    SECTION("The number of services rendered is the sum of services rendered by each provider")
    {
        REQUIRE(summary.num_services() == (report1.services_rendered() + report2.services_rendered()));
    }
    SECTION("The total cost for the period is the sum of total fees owed to each provider")
    {
        REQUIRE(summary.total_cost().value == (report1.total_fee() + report2.total_fee()).value);
    }
}