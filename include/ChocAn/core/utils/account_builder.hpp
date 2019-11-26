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

enum class Field_Types{Account_Type,First_Name,Last_Name,Street,City,State,Zip};

struct Build_Instructions{

    Build_Instructions(){};
    
    const std::string account_type   
        = "Account Type : Manager OR Provider OR Member\nMUST TYPE EXACTLY ONE OF THESE OPTIONS";
    const std::string full_name      
        = "Full Name As : First[1-24], Last[1-24]\nTOTAL CHARACTER LIMIT [2-25]";
    const std::string street_address 
        = "Address As   : Street[1-25], City[1-14], State[2], Zip[5]\nTOTAL CHARACTER LIMIT [9-46]";

    const char* invalid_type_msg = "Invalid Account Type Requested";
    const char* account_rejected = "Account Rejected";

};

class Account_Builder{

    public:

    using Database_Ptr = Data_Gateway::Database_Ptr;
    using Account_field = std::map<Field_Types, std::string>;
    
    Account_Builder(Database_Ptr db) 
        : id_generator(db)
        , account_field({})
        , valid_types({"Manager","manager","Member","member","Provider","provider"})
        { reset(); }

        Account build();
        
        bool buildable() const;

        void set_current_field(const std::string& input);

        const std::string get_current_field() const;
        
        const std::string review_account() const;
        
        const chocan_user_exception get_current_issues();

        Account_Builder& reset();

    private:

        const std::string valid_account_type(const std::string& input);
        void parseName(const std::string& input);
        void parseAddress(const std::string& input);
        void remove_leading_white_space(std::string& str);
        bool confirm_account(const std::string& input)const;
            
        ID_Generator             id_generator;
        Account_field            account_field;
        
        std::vector<std::string> valid_types;
        std::stack<std::string>  build_phase;
        
        bool account_accepted;
       
        std::optional<Name>      name;
        std::optional<Address>   address;
        std::optional<unsigned>  zip;
        std::optional<chocan_user_exception> issues;

};

struct invalid_account_build : public chocan_user_exception
{
    const std::string building_unbuildable = "Account build requested while account is not buildable";

    const char* default_msg = "Invalid account build";

    explicit invalid_account_build(const char* err, Info info) 
        : chocan_user_exception(err, info)
        { }
};


#endif  //CHOCAN_ACCOUNT_BUILDER_HPP

