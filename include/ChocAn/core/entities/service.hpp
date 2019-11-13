/* 
 
File: service.hpp

Brief: Service object holds data related to a ChocAn service carried out by a 
       Provider

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

#include <string>
#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/utils/passkey.hpp>

class Service
{
public:

    using USD = double;

    Service(unsigned code, USD cost, const std::string& service_name, Key<Data_Gateway>&)
        : cost(cost)
        , code(code)
        , name(service_name)
        { }

    const USD         cost;
    const unsigned    code;
    const std::string name;
};

#endif // CHOCAN_SERVICE_HPP
