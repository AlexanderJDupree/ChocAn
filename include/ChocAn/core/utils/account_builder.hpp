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
    
    Account_Builder(Database_Ptr db) 
            : db(db),
              fields({"Name"
                     ,"Address"
                     ,"Type"
                     ,"ID"
                    })
        {}

        bool buildable() const;

        Account build() const;

        void reset();


    private:
            
        Database_Ptr db;
        std::vector<std::string> fields;
};


#endif  //CHOCAN_ACCOUNT_BUILDER_HPP

