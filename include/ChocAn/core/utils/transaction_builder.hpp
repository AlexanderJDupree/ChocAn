/* 
File: transaction_builder.hpp

Brief:  Transaction builder performs exception handling and guides the user 
        though the complex transaction construction process

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_TRANSACTION_BUILDER_HPP
#define CHOCAN_TRANSACTION_BUILDER_HPP

#include <optional>
#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/entities/service.hpp>
#include <ChocAn/core/entities/account.hpp>
#include <ChocAn/core/entities/datetime.hpp>
#include <ChocAn/core/entities/transaction.hpp>

class Transaction_Builder
{
public:

    struct Set_Service_Date  {};
    struct Set_Provider_Acct {};
    struct Set_Member_Acct   {};
    struct Set_Comments      {};
    struct Set_Service       {};

    using Database_Ptr = Data_Gateway::Database_Ptr;
    using Builder_State = std::variant< Set_Service_Date
                                      , Set_Provider_Acct
                                      , Set_Member_Acct
                                      , Set_Comments
                                      , Set_Service
                                      >;

    Transaction_Builder(Database_Ptr db)
        : db(db)
        , state( Set_Provider_Acct() )
        { }

    // Returns True if every field contains a created object
    bool buildable() const;

    // Throws if buildable returns false
    Transaction build() const;

    // Resets builder to initial state
    Transaction_Builder& reset();

    Builder_State builder_state() const;
    void set_current_field(const std::string& input);

    const std::optional<chocan_user_exception>& get_last_error() const;

    void set_member_acct_field   (const std::string& input);
    void set_provider_acct_field (const std::string& input);
    void set_provider_acct_field (const Account& account);
    void set_service_date_field  (const std::string& input);
    void set_service_field       (const std::string& input);
    void set_comments_field      (const std::string& input);

private:

    Database_Ptr db;
    Builder_State state;

    std::optional<Service>  service;
    std::optional<DateTime> service_date;
    std::optional<Account>  provider_acct;
    std::optional<Account>  member_acct;
    std::optional<std::string> comments;

    std::optional<chocan_user_exception> error;
};

#endif // CHOCAN_TRANSACTION_BUILDER_HPP
