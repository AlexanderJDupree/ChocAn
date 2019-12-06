/* 
File: account_builder.hpp

Brief:  Account builder performs exception handling and guides the user
through the process of creating an account.

Authors: Daniel Mendez 
Alex Salazar
Arman Alauizadeh 
Alexander DuPree
Kyle Zalewski
Dominique Moore

https://github.com/AlexanderJDupree/ChocAn

*/

#ifndef CHOCAN_ACCOUNT_BUILDER_HPP
#define CHOCAN_ACCOUNT_BUILDER_HPP

#include <stack>
#include <optional>
#include <ChocAn/core/entities/account.hpp>

struct Fields
{
    std::optional<std::string> type;
    std::optional<std::string> first;
    std::optional<std::string> last;
    std::optional<std::string> street;
    std::optional<std::string> city;
    std::optional<std::string> state;
    std::optional<unsigned> zip;

};

class Account_Builder
{

public:
    
    struct Type{};
    struct First{};
    struct Last{};
    struct Street{};
    struct City{};
    struct State{};
    struct Zip{};
    struct Idle{};
    
    using Build_State = std::variant<Type,First,Last,Street,City,State,Zip,Idle>;
    using Build_Stack = std::stack<Build_State>;
    using Errors_With_Name = std::vector<invalid_name::Name_Errors>;
    using Errors_With_Address = std::vector<invalid_address::Address_Errors>;
    Account_Builder(){reset();}

    Account_Builder& initiate_new_build_process();
    
    const Account build_new_account(const ID_Generator& id_generator);
    void request_update_to_account(Build_State update_needed); 
    void apply_updates_to_account(Account& account); 
    void set_field(const std::string &input);
    void reset();

    bool buildable()const;
    const std::string get_status();
    Build_State builder_state() const;
    std::optional<const chocan_user_exception> get_errors() const;

private:
    
    void prepare_fields_for_update_sequence(const Build_State& update_needed);
    void accept_input(const std::string &input);
    void deriveType(const std::string &input);
    void deriveZip(const std::string &input);
    void maybe_build_name();
    void maybe_build_address();
    void maybe_reset_fields();
    void adapt_to_name_errors(const Errors_With_Name& errors);
    void adapt_to_address_errors(const Errors_With_Address& errors);
    Account::Account_Type yield_account_type()const;

    std::optional<ID_Generator> id_generator;
    std::optional<Name>         name;
    std::optional<Address>      address;
    Fields                      fields;
    Build_Stack                 build_state;
    chocan_user_exception::Info errors;

};

struct invalid_account_build : public chocan_user_exception
{
    explicit invalid_account_build(const char* err, Info info) 
        : chocan_user_exception(err, info)
    { }
};


#endif  //CHOCAN_ACCOUNT_BUILDER_HPP
