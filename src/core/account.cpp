/* 
 
File: account.cpp

Brief: Account data class implementation

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/core/entities/account.hpp>

Account::Account(Name name, Address address, Account_Type type, const ID_Generator& id_gen)
    : name    ( std::move(name)    )
    , address ( std::move(address) )
    , type    ( std::move(type)    )
    , id      ( id_gen.yield()     )
    {}

Account::Account(Name name, Address address, Account_Type type, unsigned id, Key<Data_Gateway>&)
    : name    ( std::move(name)    )
    , address ( std::move(address) )
    , type    ( std::move(type)    )
    , id      ( id )
    {}
