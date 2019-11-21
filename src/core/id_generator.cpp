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

#include <random>
#include <ChocAn/core/id_generator.hpp>

ID_Generator::ID_Generator(Database_Ptr db)
    : database(db)
{
    if (!db)
    {
        throw std::logic_error("ID_Generator: DB is null, cannot construct");
    }
}

unsigned ID_Generator::yield() const
{
    thread_local static std::default_random_engine eng;
    std::uniform_int_distribution dist(100000000, 999999999);
    int id = dist(eng);
    while(database->id_exists(id)) {
        id = dist(eng);
    }
    return id;
}
