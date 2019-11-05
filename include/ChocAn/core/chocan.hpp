/* 
 
File: chocan.hpp

Brief: 

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

    Database_Ptr  db;
    ID_Generator  id_generator;
    Login_Manager login_manager;

protected:

};

#endif // CHOCAN_CORE_SERVICE_HPP
