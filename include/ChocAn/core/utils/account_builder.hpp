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

class Account_Builder{

    public:

    using Database_Ptr = Data_Gateway::Database_Ptr;
    using Account_Type = Account::Account_Type;
    using Account_Info = std::map<std::string, std::string>;
    
    Account_Builder(Database_Ptr db) 
        : fields({"Type"
                 ,"Name"
                 ,"Address"
                 ,"ID"
                 })
        , id_generator(db)
        {}

        bool buildable() const;

        Account build() const;

        void reset();


    private:
            
        std::vector<std::string> fields;
        
        ID_Generator id_generator;
        Account_Type account_type;
        Account_Info account_info;

};


#endif  //CHOCAN_ACCOUNT_BUILDER_HPP

