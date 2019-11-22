/* 
 
File: chocan.hpp

Brief:  ChocAn is the main service gateway for use in the Application layer. 
        ChocAn requires a valid Databse connection that the composed service
        objects will use. 

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_CORE_SERVICE_HPP
#define CHOCAN_CORE_SERVICE_HPP

#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/id_generator.hpp>
#include <ChocAn/core/login_manager.hpp>
#include <ChocAn/core/utils/transaction_builder.hpp>
#include <ChocAn/core/utils/account_builder.hpp>

class ChocAn
{
public:

    using Database_Ptr = Data_Gateway::Database_Ptr;

    typedef std::shared_ptr<ChocAn> ChocAn_Ptr;

    // Service Objects will throw exception if db is null
    ChocAn(Database_Ptr db)
        : db            ( db ) 
        , id_generator  ( db )
        , login_manager ( db )
        , transaction_builder ( db )
        , account_builder ( db )
        { }
    
    /* Service Objects */

    // TODO wrap service object access behind function calls
    Database_Ptr  db;

    ID_Generator  id_generator;
    Login_Manager login_manager;
    Transaction_Builder transaction_builder;
    Account_Builder account_builder;

protected:

};

#endif // CHOCAN_CORE_SERVICE_HPP
