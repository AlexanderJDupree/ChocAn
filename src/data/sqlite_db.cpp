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

#include <sstream>
#include <ChocAn/data/sqlite_db.hpp>
#include <ChocAn/core/utils/exception.hpp>
#include <ChocAn/core/utils/overloaded.hpp>
#include <ChocAn/core/entities/account.hpp>

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

void SQLite_DB::create_account(const Account& account)
{
    std::string sql = "INSERT INTO \"accounts\" VALUES (" + serialize_account(account) + ");";
    return;
}

std::string SQLite_DB::serialize_account(const Account& account) const
{
    std::stringstream data;

    data << std::to_string(account.id()) << ", '"
         << account.name().first() << "', '"
         << account.name().last() << "', '"
         << account.address().street() << "', '"
         << account.address().city() << "', '"
         << account.address().zip() << "', "
         << std::visit( overloaded {
             [](const Manager&) -> std::string { return "'Manager', 'Valid'"; },
             [](const Provider&)-> std::string { return "'Provider', 'Valid'"; },
             [](const Member& m)-> std::string { 
                 return std::string("'Member', ") += (m.status() == Account_Status::Valid) 
                                                     ? "'Valid'" 
                                                     : "'Suspended'";
             }
         }, account.type());

    return data.str();
}