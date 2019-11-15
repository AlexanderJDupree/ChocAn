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
                                    , "OR"
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
        { 1234, _account_table.at(1234) }
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
{ }

void Mock_DB::update_account(const Account& account)
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
    return;
}

void Mock_DB::create_account(const Account& account)
{
    return update_account(account);
}

void Mock_DB::delete_account(const unsigned ID)
{
    _account_table.erase(ID);
    return;
}

void Mock_DB::add_transaction(const Transaction&)
{
    return;
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
std::optional<Account> Mock_DB::get_account(const unsigned ID) const
{
    return account_table_lookup(ID, _account_table);
}

std::optional<Account> Mock_DB::get_account(const std::string& ID) const
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
std::optional<Account> Mock_DB::get_member_account(const unsigned ID) const
{
    return account_table_lookup(ID, _member_table);
}
std::optional<Account> Mock_DB::get_member_account(const std::string& ID)  const
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
std::optional<Account> Mock_DB::get_provider_account(const unsigned ID) const
{
    return account_table_lookup(ID, _provider_table);
}
std::optional<Account> Mock_DB::get_provider_account(const std::string& ID) const
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
std::optional<Account> Mock_DB::get_manager_account(const unsigned ID) const
{
    return account_table_lookup(ID, _manager_table);
}
std::optional<Account> Mock_DB::get_manager_account(const std::string& ID) const
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

std::optional<Service> Mock_DB::lookup_service(const unsigned code) const
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

std::optional<Service> Mock_DB::lookup_service(const std::string& code) const
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

const Data_Gateway::Service_Directory& Mock_DB::service_directory() const
{
    return _service_directory;
}

bool Mock_DB::id_exists(const unsigned ID) const
{
    return _account_table.find(ID) != _account_table.end();
}
