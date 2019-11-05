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

class ChocAn
{
public:

    using Database_Ptr = Data_Gateway::Database_Ptr;

    typedef std::shared_ptr<ChocAn> ChocAn_Ptr;

    ChocAn(Database_Ptr db)
        : db            ( db ) 
        , id_generator  ( db )
        , login_manager ( db )
        { }
    
    /* Service Objects */

    Database_Ptr  db; // TODO Review whether we should wrap DB methods

    ID_Generator  id_generator;
    Login_Manager login_manager;

protected:

};

#endif // CHOCAN_CORE_SERVICE_HPP
