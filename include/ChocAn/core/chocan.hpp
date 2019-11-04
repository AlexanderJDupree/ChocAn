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
#include <ChocAn/core/login_manager.hpp>

class ChocAn
{
public:

    using Database_Ptr = Data_Gateway::Database_Ptr;

    ChocAn(Database_Ptr db)
        : login_manager(db)
        , db(db) 
        { }

    Login_Manager login_manager;

private:

    Database_Ptr db;

};

#endif // CHOCAN_CORE_SERVICE_HPP