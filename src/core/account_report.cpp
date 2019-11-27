/* 
 
File: account_report.cpp

Brief: account report object compiles account activity into a data structure

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/core/entities/account_report.hpp>

Provider_Report::Provider_Report(const Account& account, const Transactions& transactions)
    : Account_Report(account, transactions)
{
    if(!std::holds_alternative<Provider>(account.type()))
    {
        throw std::logic_error("Can't compile provider reports for non-provider account");
    }

    for(const auto& transaction : transactions)
    {
        if (account != transaction.provider() )
        {
            throw std::logic_error("Reporting Error: Unrelated transaction activity");
        }
    }
}

USD Provider_Report::total_fee() const
{
    USD sum{ 0.0 };
    for (const auto& transaction : _transactions)
    {
        sum += transaction.service().cost();
    }
    return sum;
}

unsigned Provider_Report::services_rendered() const
{
    return _transactions.size();
}

unsigned Summary_Report::num_providers() const
{
    return _activity.size();
}

unsigned Summary_Report::num_services() const
{
    unsigned service_count = 0;
    for(const auto& report : _activity)
    {
        service_count += report.services_rendered();
    }
    return service_count;
}

USD Summary_Report::total_cost() const
{
    USD cost { 0.0 };
    for (const auto& report : _activity)
    {
        cost += report.total_fee();
    }
    return cost;
}