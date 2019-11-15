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
    : _name    ( name    )
    , _address ( address )
    , _type    ( type    )
    , _id      ( id_gen.yield() )
    {}

Account::Account(Name name, Address address, Account_Type type, unsigned id, Key<Data_Gateway>&)
    : _name    ( name    )
    , _address ( address )
    , _type    ( type    )
    , _id      ( id )
    {}
