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

class Account_Builder{

    public:

    using Database_Ptr = Data_Gateway::Database_Ptr;
    using Account_Info = std::map<std::string, std::string>;
    
    Account_Builder(Database_Ptr db) 
        : fields({"Zip"
                 ,"State"
                 ,"City"
                 ,"Street"
                 ,"Last Name"
                 ,"First Name"
                 ,"Account Type"
                 })
        , id_generator(db)
        {}


        Account build();
        
        bool buildable() const;

        void set_current_field(const std::string& input);

        std::string get_current_field();

        const chocan_user_exception get_current_issues();

        Account_Builder& reset();

    private:

        std::string valid_input(const std::string& field, const std::string& input);
            
        std::stack<std::string> fields;
        ID_Generator            id_generator;
        Account_Info            account_info;

        std::optional<chocan_user_exception> issues;

};

#endif  //CHOCAN_ACCOUNT_BUILDER_HPP

