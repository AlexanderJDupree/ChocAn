/* 
 
File: account_report.hpp

Brief: account report object compiles account activity into a data structure

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_ACCOUNT_REPORT_HPP
#define CHOCAN_ACCOUNT_REPORT_HPP

#include <vector>
#include <ChocAn/core/entities/account.hpp>
#include <ChocAn/core/entities/transaction.hpp>


class Account_Report
{
public:
    using Transactions = std::vector<Transaction>;

    Account_Report(const Account& account, const Transactions& transactions)
        : _account(account), _transactions(transactions) { }

    virtual ~Account_Report() {};

    const Account& account() const { return _account; };
    const Transactions& transactions() const { return _transactions; }

protected:

    Account _account;
    Transactions _transactions;
};

class Member_Report : public Account_Report
{
public:
    Member_Report(const Account& account, const Transactions& transactions)
        : Account_Report(account, transactions) {}
};

class Provider_Report : public Account_Report
{
public:

    Provider_Report(const Account& account, const Transactions& transactions);

    virtual ~Provider_Report() {};

    USD total_fee() const;
    unsigned services_rendered() const;
};

class Summary_Report
{
public:

    using Provider_Activity = std::vector<Provider_Report>;

    Summary_Report( const DateTime& start
                  , const DateTime& end
                  , const Provider_Activity& activity)
        : _end(end)
        , _start(start)
        , _activity(activity) 
        { } 

    const DateTime& start_date() const { return _start; }
    const DateTime& end_date() const   { return _end;   }

    const Provider_Activity& activity() const { return _activity; }

    unsigned num_providers() const; 
    unsigned num_services() const;
    USD total_cost() const;

private:

    DateTime _end;
    DateTime _start;
    Provider_Activity _activity;
};

using ChocAn_Report = std::variant< Member_Report
                                  , Provider_Report
                                  , Summary_Report
                                  >;

#endif // CHOCAN_ACCOUNT_REPORT_HPP