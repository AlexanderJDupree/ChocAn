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
#include <ChocAn/core/utils/serializable.hpp>

struct USD { double value = 0; };

class Service : public Serializable<Service, std::string, std::string>
{
public:

    Service(unsigned code, USD cost, const std::string& service_name, const Key<Data_Gateway>&)
        : _cost(cost)
        , _code(code)
        , _name(service_name)
        { }

    Service(const Data_Table& data, const Key<Data_Gateway>&)
        : _cost( { std::stod(data.at("cost")) } )
        , _code( std::stoi(data.at("code")) )
        , _name( data.at("name") )
        { }

    const USD&         cost() const { return _cost; }
    unsigned           code() const { return _code; }
    const std::string& name() const { return _name; }

    Data_Table serialize() const 
    {
        return 
        {
            { "name", _name },
            { "code", std::to_string(_code)       },
            { "cost", std::to_string(_cost.value) }
        };
    }
    
private:

    USD         _cost;
    unsigned    _code;
    std::string _name;
};

#endif // CHOCAN_SERVICE_HPP
