/* 
 
File: chocan_service.hpp

Brief: chocan_service is a base class that handles linking of services to the 
       database.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_SERVICE_HPP
#define CHOCAN_SERVICE_HPP

#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/utils/exception.hpp>

class ChocAn_Service
{
public:

    using Database_Ptr = Data_Gateway::Database_Ptr;

    ChocAn_Service(Database_Ptr db) 
        : database(db) 
        { if (!db) { throw std::system_error(); } }

    virtual ~ChocAn_Service() {}

protected:

    Database_Ptr database;

};

#endif // CHOCAN_SERVICE_HPP