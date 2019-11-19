/* 
 
File: sqlite_db.hpp

Brief: Implements Data Gateway interface with sqlite3 API

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/data/sqlite_db.hpp>
#include <ChocAn/core/utils/exception.hpp>

SQLite_DB::SQLite_DB(const char* db_name)
{
    if(sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        throw chocan_db_exception("Fatal: Unable to connect to DB");
    }
}

SQLite_DB::~SQLite_DB()
{
    // TODO sqlite3 will return SQLITE_BUSY if there are any non-finalized statements
    sqlite3_close(db);
}

