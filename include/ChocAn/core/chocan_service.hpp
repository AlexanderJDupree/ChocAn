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

#include <ChocAn/core/chocan.hpp>
#include <ChocAn/core/data_gateway.hpp>

class Service_Key
{
private:

    friend class ChocAn;

    Service_Key() = default;

    Service_Key(const Service_Key&) = delete;
    Service_Key& operator=(const Service_Key&) = delete;

};

class ChocAn_Service
{
public:

    using Database_Ptr = Data_Gateway::Database_Ptr;

    ChocAn_Service() : database(nullptr) {}

    virtual ~ChocAn_Service() {}

    void set_gateway(Database_Ptr db) { database = db; }

protected:

    Database_Ptr database;

};

#endif // CHOCAN_SERVICE_HPP