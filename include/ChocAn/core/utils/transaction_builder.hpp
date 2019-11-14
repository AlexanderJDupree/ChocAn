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

#include <vector>
#include <optional>
#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/entities/service.hpp>
#include <ChocAn/core/entities/account.hpp>
#include <ChocAn/core/entities/datetime.hpp>
#include <ChocAn/core/entities/transaction.hpp>

class Transaction_Builder
{
public:

    using Database_Ptr = Data_Gateway::Database_Ptr;

    Transaction_Builder(Database_Ptr db)
        : db(db)
        , fields( { "Comments"
                  , "Service Code"
                  , "Date of Provided Service (MM-DD-YYYY)"
                  , "Member ID"
                  , "Provider ID" } )
        { }

    // Returns True if every field contains a created object
    bool buildable() const;

    // Throws if buildable returns false
    Transaction build() const;

    // Resets builder to initial state
    void reset();

    std::string get_current_field() const;
    void set_current_field(const std::string& input);

    const std::optional<chocan_user_exception>& get_last_error() const;

    void set_member_acct_field   (const std::string& input);
    void set_provider_acct_field (const std::string& input);
    void set_service_date_field  (const std::string& input);
    void set_service_field       (const std::string& input);
    void set_comments_field      (const std::string& input);

private:

    Database_Ptr db;

    std::vector<std::string> fields;

    std::optional<Service>  service;
    std::optional<DateTime> service_date;
    std::optional<Account>  provider_acct;
    std::optional<Account>  member_acct;
    std::optional<std::string> comments;

    std::optional<chocan_user_exception> error;
};

#endif // CHOCAN_TRANSACTION_BUILDER_HPP
