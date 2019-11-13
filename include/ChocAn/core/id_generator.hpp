/* 
 
File: id_generator.hpp

Brief: ID_Generator class generates unique ChocAn_IDs for Accounts

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_ID_GEN_HPP
#define CHOCAN_ID_GEN_HPP

#include <ChocAn/core/data_gateway.hpp>

class ID_Generator
{
public:

    using Database_Ptr = Data_Gateway::Database_Ptr;

    ID_Generator(Database_Ptr db) 
        : database(db) 
        { if(!db) { throw std::exception(); } }

    unsigned yield() const;

private:

    Database_Ptr database;

};

#endif // CHOCAN_ID_GEN_HPP