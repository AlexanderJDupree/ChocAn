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

std::map<unsigned, Account> Mock_DB::account_table
{
    { 1234, Account( Name ("John", "Doe")
                   , Address ( "1234 cool st."
                             , "Portland"
                             , "OR"
                             , 97030 )
                   , Account_Type::Manager ) },

    { 5678, Account( Name ("Arman", "Doe")
                   , Address ( "1234 lame st."
                             , "Portland"
                             , "OR"
                             , 97030 )
                   , Account_Type::Provider ) }
};

void Mock_DB::update_account(const Account& account)
{
    account_table.insert( {account.id, account} );
    return;
}

void Mock_DB::create_account(const Account& account)
{
    return update_account(account);
}

void Mock_DB::delete_account(const unsigned ID)
{
    account_table.erase(ID);
    return;
}

Account::Account_Ptr Mock_DB::retrieve_account(const unsigned ID)
{
    try
    {
        Account account(account_table.at(ID));

        return std::make_unique<Account>(std::move(account));
    }
    catch(const std::out_of_range& err)
    {
        return nullptr;
    }
}

bool Mock_DB::id_exists(const unsigned ID)
{
    return account_table.find(ID) != account_table.end();
}
