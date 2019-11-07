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

#include <ChocAn/core/chocan_service.hpp>

class ID_Generator : public ChocAn_Service
{
public:

    ID_Generator(Database_Ptr db) : ChocAn_Service(db) {}

    unsigned yield() const;

};

#endif // CHOCAN_ID_GEN_HPP