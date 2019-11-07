/* 
 
File: id_generator.cpp

Brief: ID_Generator class implementation

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/core/id_generator.hpp>

unsigned ID_Generator::yield() const
{
    int id = 111111110;
    while(database->id_exists(++id));
    return id;
}
