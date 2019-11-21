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

enum field_types{current_type,current_first,current_last,current_street,current_city,current_state,current_zip};

class Account_Builder{

    public:

    using Database_Ptr = Data_Gateway::Database_Ptr;
    using Account_Info = std::map<field_types, std::string>;
    
    Account_Builder(Database_Ptr db) 
        : id_generator(db)
        , account_info({})
        , valid_types({"Manager","manager","Member","member","Provider","provider"})
        { reset(); }

        Account build();
        
        bool buildable() const;

        void set_current_field(const std::string& input);

        const std::string get_current_field() const;
        
        const chocan_user_exception get_current_issues();

        Account_Builder& reset();

    private:

        const std::string valid_account_type(const std::string& input);
        const std::string review_account() const;
        void parseName(const std::string& input);
        void parseAddress(const std::string& input);
        void remove_leading_white_space(std::string& str);
        bool confirm_account(const std::string& input)const;
            
        ID_Generator             id_generator;
        Account_Info             account_info;
        
        std::vector<std::string> valid_types;
        std::stack<std::string>  fields;
        
        bool account_accepted;
       
        std::optional<Name>      name;
        std::optional<Address>   address;
        std::optional<unsigned>  zip;
        std::optional<chocan_user_exception> issues;

};

#endif  //CHOCAN_ACCOUNT_BUILDER_HPP

