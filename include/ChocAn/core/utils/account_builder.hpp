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

class Account_Field{
    public:

        Account_Field(std::string instructions) : instructions(instructions) {};

        virtual ~Account_Field() {};

        virtual bool parse_input(const std::string& input) = 0;

        std::string instructions;
};

class Account_Type : public Account_Field{
    public:
        Account_Type() : Account_Field("Instructions for type") {};

        std::variant<Manager,Member,Provider> account_type;

        bool parse_input(const std::string& input) override;
};

class First_Name : public Account_Field{
    public:
        First_Name() : Account_Field("Instruction for first name") {};

        std::optional<std::string> first_name;
        
        bool parse_input(const std::string& input) override;
};

class Last_Name : public Account_Field{
    public:
        Last_Name() : Account_Field("Instruction for last name") {};
       
        std::optional<std::string> last_name;
        
        bool parse_input(const std::string& input) override;
};

class Street : public Account_Field{
    public:
        Street() : Account_Field("Instructions for street") {};
       
        std::optional<std::string> street;
        
        bool parse_input(const std::string& input) override;
};

class City : public Account_Field{
    public:
        City() : Account_Field("Instructions for city") {};
       
        std::optional<std::string> city;
        
        bool parse_input(const std::string& input) override;
};

class State : public Account_Field{
    public: 
        State() : Account_Field("Instructions for state") {};
       
        std::optional<std::string> state;
        
        bool parse_input(const std::string& input) override;
};

class Zip : public Account_Field{
    public:
        Zip() : Account_Field("Instructions for zip") {};

        std::optional<unsigned> zip;
        
        bool parse_input(const std::string& input) override;
};

class Account_Builder{

    public:

        using Build_Phase = std::variant<Account_Type,First_Name,Last_Name,Street,City,State,Zip>;
        using Database_Ptr = Data_Gateway::Database_Ptr;

        Account_Builder(Database_Ptr db) : id_generator(db), build_phase(Account_Type()) {reset();}

        Account build();

        Account_Builder& reset();
        bool buildable() const;
        void set_current_field(const std::string& input);
        const std::string get_current_field() const;
        const chocan_user_exception get_current_issues();

    private:

        const std::string valid_account_type(const std::string& input);
        void parseName(const std::string& input);
        void parseAddress(const std::string& input);
        void remove_leading_white_space(std::string& str);
        bool confirm_account(const std::string& input)const;

        ID_Generator             id_generator;
        Build_Phase build_phase;
        std::vector<std::string> issues;

};

struct invalid_account_build : public chocan_user_exception
{
    explicit invalid_account_build(const char* err, Info info) 
        : chocan_user_exception(err, info)
    { }
};


#endif  //CHOCAN_ACCOUNT_BUILDER_HPP

