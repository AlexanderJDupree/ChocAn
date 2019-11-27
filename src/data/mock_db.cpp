/* 

File: mock_db.cpp

Brief: Mock Database implementation

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <algorithm>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/core/utils/overloaded.hpp>

Mock_DB::Mock_DB()
    : _account_table
    ( {

        { 5678, Account( Name ("Dan", "Manager")
                        , Address ( "1234 cool st."
                                    , "Portland"
                                    , "OR"
                                    , 97030 )
                        , Manager()
                        , 5678
                        , db_key ) },

        { 1234, Account( Name ("Arman", "Provider")
                        , Address ( "1234 lame st."
                                    , "Portland"
                                    , "OR"
                                    , 97030 )
                        , Provider()
                        , 1234
                        , db_key ) },
        { 1111, Account( Name ("Dude", "Awesome")
                        , Address ( "1234 totally st."
                                    , "Los Angeles"
                                    , "CA"
                                    , 91510 )
                        , Provider()
                        , 1111
                        , db_key ) },

        { 6789, Account( Name ("Alex", "Member")
                        , Address ( "1234 Meh st."
                                    , "Portland"
                                    , "OR"
                                    , 97030 )
                        , Member { Account_Status::Valid }
                        , 6789 
                        , db_key ) },

        { 9876, Account( Name ("Jane", "Member")
                        , Address ( "1234 awesome st."
                                    , "Portland"
                                    , "ME"
                                    , 97030 )
                        , Member { Account_Status::Suspended }
                        , 9876
                        , db_key ) }
    } )
    , _member_table 
    ( {
        { 6789, _account_table.at(6789) },
        { 9876, _account_table.at(9876) }
    } )
    , _provider_table
    ( {
        { 1234, _account_table.at(1234) },
        { 1111, _account_table.at(1111) }
    } )
    , _manager_table
    ( {
        { 5678, _account_table.at(5678) }
    } )
    , _service_directory 
    ( {
        { 123456, Service ( 123456, USD { 29.99 }, "Back Rub", db_key ) },
        { 111111, Service ( 111111, USD { 59.99 }, "Addiction Consulting", db_key ) },
        { 222222, Service ( 222222, USD { 100.00 }, "Addiction Treatment", db_key ) }
    })
    , _transaction_table
    ( {
        { 1, Transaction ( _provider_table.at(1234)
                         , _member_table.at(6789)
                         , DateTime(1574380800)
                         , _service_directory.at(123456)
                         , "comments" ) },

        { 2, Transaction ( _provider_table.at(1234)
                         , _member_table.at(6789)
                         , DateTime(1574480800)
                         , _service_directory.at(222222)
                         , "some comments" ) },

        { 3, Transaction ( _provider_table.at(1111)
                         , _member_table.at(6789)
                         , DateTime(1574680800)
                         , _service_directory.at(123456)
                         , "lame comments" ) },

        { 4, Transaction ( _provider_table.at(1111)
                         , _member_table.at(6789)
                         , DateTime(1574580800)
                         , _service_directory.at(111111)
                         , "more comments" ) }
    })
{ }

bool Mock_DB::update_account(const Account& account)
{
    unsigned id = account.id();

    // Insert to master table
    _account_table.insert( { id, account } );

    // Update auxillary tables
    std::visit( overloaded {
        [&](const Manager&) { _manager_table.insert  ( { id, _account_table.at(id)} ); },
        [&](const Provider&){ _member_table.insert   ( { id, _account_table.at(id)} ); },
        [&](const Member&)  { _provider_table.insert ( { id, _account_table.at(id)} ); }
    }, account.type() );
    return true;
}

unsigned Mock_DB::create_account(const Account& account)
{
    if(update_account(account)) { return account.id(); }
    return 0;
}

bool Mock_DB::delete_account(const unsigned ID)
{
    _account_table.erase(ID);
    return true;
}

unsigned Mock_DB::add_transaction(const Transaction& transaction)
{
    try
    {
        unsigned transaction_id = transaction.filed_date().unix_timestamp();
        _transaction_table.insert( { transaction_id, transaction } );
        return transaction_id;
    }
    catch(const std::exception&)
    {
        return 0;
    }
}

std::optional<Account> Mock_DB::account_table_lookup(const unsigned ID, const Account_Table& table) const
{
    try
    {
        // Just Account
        return table.at(ID);
    }
    catch(const std::out_of_range&)
    {
        // Nothing
        return { };
    }
}
std::optional<Account> Mock_DB::account_table_lookup(const unsigned ID, const Reference_Table& table) const
{
    try
    {
        // Just Account
        return table.at(ID);
    }
    catch(const std::out_of_range&)
    {
        // Nothing
        return { };
    }
}
std::optional<Account> Mock_DB::get_account(const unsigned ID)
{
    return account_table_lookup(ID, _account_table);
}

std::optional<Account> Mock_DB::get_account(const std::string& ID)
{
    try
    {
        return get_account(std::stoi(ID));
    }
    catch(const std::exception&)
    {
        return { };
    }
}
std::optional<Account> Mock_DB::get_member_account(const unsigned ID)
{
    return account_table_lookup(ID, _member_table);
}
std::optional<Account> Mock_DB::get_member_account(const std::string& ID)
{
    try
    {
        return get_member_account(std::stoi(ID));
    }
    catch(const std::exception&)
    {
        return { };
    }
}
std::optional<Account> Mock_DB::get_provider_account(const unsigned ID)
{
    return account_table_lookup(ID, _provider_table);
}
std::optional<Account> Mock_DB::get_provider_account(const std::string& ID)
{
    try
    {
        return get_provider_account(std::stoi(ID));
    }
    catch(const std::exception&)
    {
        return { };
    }
}
std::optional<Account> Mock_DB::get_manager_account(const unsigned ID)
{
    return account_table_lookup(ID, _manager_table);
}
std::optional<Account> Mock_DB::get_manager_account(const std::string& ID)
{
    try
    {
        return get_manager_account(std::stoi(ID));
    }
    catch(const std::exception&)
    {
        return { };
    }
}

std::optional<Service> Mock_DB::lookup_service(const unsigned code)
{
    try
    {
        return _service_directory.at(code);
    }
    catch(const std::out_of_range&)
    {
        return { };
    }
}

std::optional<Service> Mock_DB::lookup_service(const std::string& code)
{
    try
    {
        return lookup_service(std::stoi(code));
    }
    catch(const std::exception&)
    {
        return { };
    }
}

Data_Gateway::Service_Directory Mock_DB::service_directory()
{
    return _service_directory;
}

Data_Gateway::Transactions Mock_DB::get_transactions(DateTime start, DateTime end, Account acct)
{
    Transactions transactions;
    auto ids_match = [&](const Transaction& transaction) -> bool
    {
        return std::visit( overloaded {
            [&](const Member&)  { return transaction.member() == acct;   },
            [&](const Provider&){ return transaction.provider() == acct; },
            [&](const Manager&) { return false; }
        }, acct.type() );
    };
    auto predicate = [&](const std::pair<const unsigned, Transaction>& entry)
    {
        DateTime service_date = entry.second.service_date();
        if(start <= service_date && service_date <= end && ids_match(entry.second))
        {
            transactions.push_back(entry.second);
        }
    };

    std::for_each(_transaction_table.begin(), _transaction_table.end(), predicate);

    return transactions;
}

Data_Gateway::Transactions Mock_DB::get_transactions(DateTime start, DateTime end)
{
    Transactions transactions;
    auto predicate = [&](const std::pair<const unsigned, Transaction>& entry)
    {
        DateTime service_date = entry.second.service_date();
        if(start <= service_date && service_date <= end)
        {
            transactions.push_back(entry.second);
        }
    };

    std::for_each(_transaction_table.begin(), _transaction_table.end(), predicate);

    return {};
}

bool Mock_DB::id_exists(const unsigned ID) const
{
    return _account_table.find(ID) != _account_table.end();
}
