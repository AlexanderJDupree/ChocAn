/* 

File: id_generator.cpp

Brief: Unit tests for id_generator

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/core/id_generator.hpp>

TEST_CASE("ID_Generator use case", "[id_generator], [service]")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    // Create generator service
    ID_Generator generator(db);

    SECTION("Generator ONLY generator unique ID's")
    {
        bool results[1024];

        // Map predicate that id exists over range of results
        std::transform(results, results + 1024, results,
        [&](bool)
        { 
            return db->id_exists(generator.yield());
        });

        // If ANY result returns true, then the generator yielded an id that exists
        REQUIRE_FALSE(std::any_of(results, results + 1024, 
                        [](bool result)
                        { 
                            return result; 
                        }));

    }

    SECTION("Generator generates a random 9 digit number with minimum 1e8")
    {
        int results[1024];

        // Generate 1024 random id's
        std::transform(results, results + 1024, results,
        [&](int)
        {
            return generator.yield();
        });

        // Require that the id's fit within the required range
        REQUIRE_FALSE(std::any_of(results, results + 1024,
                        [](int result)
                        {
                            return (result < 100000000 || result > 999999999);
                        }));


    }

}
