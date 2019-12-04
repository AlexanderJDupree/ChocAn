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

#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/entities/account.hpp>
#include <ChocAn/core/utils/validators.hpp>
#include <optional>
#include <stack>

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
    struct Buildable{};
    
    using Database_Ptr = Data_Gateway::Database_Ptr;
    using Build_State = std::variant<Type,First,Last,Street,City,State,Zip>;
    using Build_Stack = std::stack<Build_State>;

    Account_Builder(){reset();}

    Account_Builder& reset();
    
    const Account build_new_account(const ID_Generator& id_generator);
    void initiate_new_build_process();
    void request_updates_to_account(const Build_Stack& updates_needed); 
    void apply_updates_to_account(Account& account); 
    void set_field(const std::string &input);

    bool buildable()const;
    const std::string get_status();
    Build_State builder_state() const;
    std::optional<const chocan_user_exception> get_errors() const;

private:
    
    static const std::set<std::string> US_states;
    
    bool full_name_valid()const;
    void accept_input(const std::string &input);
    void transition_state();
    void deriveType(const std::string &input);
    void deriveFirst(const std::string &input);
    void deriveLast(const std::string &input);
    void deriveStreet(const std::string &input);
    void deriveCity(const std::string &input);
    void deriveState(const std::string &input);
    void deriveZip(const std::string &input);
    Account::Account_Type yield_account_type()const;

    std::optional<ID_Generator> id_generator;
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
