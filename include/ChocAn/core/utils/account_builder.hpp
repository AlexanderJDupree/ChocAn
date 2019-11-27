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
#include <ChocAn/view/terminal_input_controller.hpp>
#include <stack>
#include <optional>

class Type {
    public:
        std::optional<Account::Account_Type> type;
};

class Word {
    public:
        std::optional<std::string> word;
};

class Number {
    public:
        std::optional<unsigned> num;
};

class Get_Type   : public Type{};
class Get_First  : public Word{};
class Get_Last   : public Word{};
class Get_Street : public Word{};
class Get_City   : public Word{};
class Get_State  : public Word{};
class Get_Zip    : public Number{};

class Account_Builder{

    public:
        template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
        template<class... Ts> overload(Ts...) -> overload<Ts...>;

        using Input_State  = std::variant<Get_Type,Get_First,Get_Last,Get_Street,Get_City,Get_State,Get_Zip>;
        using Fields       = std::vector<Input_State>;
        using Database_Ptr = Data_Gateway::Database_Ptr;

        Account_Builder(Database_Ptr db) : id_generator(db) {reset();}

        Account build();
        Account_Builder& reset();
        bool buildable() const;
        void set_field(const std::string& input);
        
        const Input_State& get_field() const;
        std::optional<const chocan_user_exception> get_issues()const;

    private:

        bool fields_ready(int from, int to) const;
        std::optional<Account::Account_Type> deriveType(const std::string& input);
        std::optional<unsigned> deriveZip(const std::string& input);

        ID_Generator                         id_generator;
        Fields                               fields;
        std::vector<std::string>             issues;
        std::optional<Name>                  name;
        std::optional<Address>               address;
        std::optional<Account::Account_Type> type;
};

struct invalid_account_build : public chocan_user_exception
{
    explicit invalid_account_build(const char* err, Info info) 
        : chocan_user_exception(err, info)
    { }
};


#endif  //CHOCAN_ACCOUNT_BUILDER_HPP



class Account_Builder{

    public:
        template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
        template<class... Ts> overload(Ts...) -> overload<Ts...>;
        
        using Fields =  std::vector<std::variant<Type, Word, Number>>;
        using Database_Ptr = Data_Gateway::Database_Ptr;

        Account_Builder(Database_Ptr db) : id_generator(db) {reset();}

        Account build();
        Account_Builder& reset();
        
        bool buildable() const;
        void set_field(const std::string& input);
        const std::string instructions()const;
        const chocan_user_exception status();

    private:

        void reset_fields_as_needed();
        bool fields_ready(int from, int to)const;
        std::optional<Account::Account_Type> deriveType(const std::string& input);
        std::optional<unsigned> deriveZip(const std::string& input);

        ID_Generator                         id_generator;
        Fields                               fields;
        std::vector<std::string>             issues;
        std::optional<Name>                  name;
        std::optional<Address>               address;
        std::optional<Account::Account_Type> type;
};

struct invalid_account_build : public chocan_user_exception
{
    explicit invalid_account_build(const char* err, Info info) 
        : chocan_user_exception(err, info)
    { }
};


#endif  //CHOCAN_ACCOUNT_BUILDER_HPP

