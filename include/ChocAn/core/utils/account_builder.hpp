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
#include <optional>

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

enum class Build_State{Type,First,Last,Street,City,State,Zip,Buildable};

class Account_Builder
{

public:
    using Database_Ptr = Data_Gateway::Database_Ptr;

    Account_Builder(Database_Ptr db) : id_generator(db) { reset(); }

    Account build();
    Account_Builder &reset();

    bool buildable()const;
    const Build_State& get_state() const;
    const std::string get_status();
    void set_field(const std::string &input);
    std::optional<const chocan_user_exception> get_issues() const;

private:
    void accept_input(const std::string &input);
    void reset_fields_as_needed();
    void transition_state();
    void deriveType(const std::string &input);
    void deriveZip(const std::string &input);
    Account::Account_Type yield_account_type()const;

    ID_Generator id_generator;
    Fields       fields;
    Build_State  build_state;
    
    std::vector<std::string> issues;
    std::optional<Name> name;
    std::optional<Address> address;
};

struct invalid_account_build : public chocan_user_exception
{
    explicit invalid_account_build(const char* err, Info info) 
        : chocan_user_exception(err, info)
    { }
};


#endif  //CHOCAN_ACCOUNT_BUILDER_HPP