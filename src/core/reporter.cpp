/* 
 
File: reporter.cpp

Brief:  reporter service makes structured queries to the databse to compile 
        different types of reports

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <algorithm>
#include <ChocAn/core/reporter.hpp>

Reporter::Reporter(Database_Ptr db)
    : db(db)
{
    if(!db)
    {
        throw std::logic_error("Reporter: DB is null, construction failed");
    }
}

Provider_Report Reporter::gen_provider_report( const DateTime& start
                                             , const DateTime& end
                                             , const Account& provider) const
{
    return Provider_Report( provider, db->get_transactions(start, end, provider));
}

Summary_Report Reporter::gen_summary_report(const DateTime& start, const DateTime& end) const
{
    Summary_Report::Provider_Activity activity;

    // Get all provider accounts
    Data_Gateway::Accounts provider_accounts = db->get_provider_accounts();

    // For each provider, generate a Provider report for the period [start, end]
    std::for_each(provider_accounts.begin(), provider_accounts.end(), [&](const Account& provider)
    {
        activity.push_back(gen_provider_report(start, end, provider));
    } );

    return Summary_Report(start, end, activity);
}
