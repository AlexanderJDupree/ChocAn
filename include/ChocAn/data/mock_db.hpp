#include <map>
#include <ChocAn/core/data_gateway.hpp>

class Mock_DB : public Data_Gateway
{
public:

    void update_account(const Account& account)
    {
        account_table.insert( {account.id, account} );
        return;
    }

    void create_account(const Account& account)
    {
        return update_account(account);
    }

    void delete_account(const unsigned ID)
    {
        account_table.erase(ID);
        return;
    }

    Account_Ptr retrieve_account(const unsigned ID)
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

    bool id_exists(const unsigned ID)
    {
        return account_table.find(ID) != account_table.end();
    }

    static std::map<unsigned, Account> account_table;

};

std::map<unsigned, Account> Mock_DB::account_table
{
    { 1234, Account( Name ("John", "Doe")
                   , Address ( "1234 cool st."
                             , "Portland"
                             , "OR"
                             , 97030 )
                   , Account_Type::Manager ) },

    { 5678, Account( Name ("Arman", "Doe")
                   , Address ( "1234 cool st."
                             , "Portland"
                             , "OR"
                             , 97030 )
                   , Account_Type::Provider ) }
};
