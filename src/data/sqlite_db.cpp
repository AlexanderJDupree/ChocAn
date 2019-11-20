/* 
 
File: sqlite_db.cpp

Brief: Implements Data Gateway interface with sqlite3 API

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <cstring>
#include <sstream>
#include <fstream>
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
SQLite_DB::SQLite_DB(const char* db_name, const char* schema_file)
    : SQLite_DB(db_name)
{
    if(!load_schema(schema_file))
    {
        throw chocan_db_exception("Fatal: Unable to load schema file");
    }
}

SQLite_DB::~SQLite_DB()
{
    // TODO sqlite3 will return SQLITE_BUSY if there are any non-finalized statements
    sqlite3_close(db);
}

bool SQLite_DB::load_schema(const char* schema_file)
{
    std::ifstream schema(schema_file);
    if (schema.is_open())
    {
        std::stringstream buffer;

        buffer << schema.rdbuf();

        return execute_statement(buffer.str(), no_callback);
    }
    return false;
}

bool SQLite_DB::create_account(const Account& account)
{
    /*
    This is the syntax I would like to use
    Data_Tree data = account.serialize();

    std::string sql = "INSERT OR REPLACE INTO accounts VALUES (" 
                    + data["id"]
                    + data["name"]["first"]
                    + data["name"]["last"]
                    + data["address"]["street"]
                    + data["address"]["city"]
                    + data["address"]["state"]
                    + data["address"]["zip"]
                    + data["type"]
                    + data["status"]
                    + ");";
                    */

    std::string sql = "INSERT OR REPLACE INTO accounts VALUES (" + serialize_account(account) + ");";

    return execute_statement(sql, no_callback);
}

bool SQLite_DB::update_account(const Account& account)
{
    // TODO review if this is desired behavior
    return create_account(account);
}

bool SQLite_DB::delete_account(const unsigned ID)
{
    std::string sql = "DELETE FROM accounts WHERE chocan_id=" + std::to_string(ID) + ';';

    return execute_statement(sql, no_callback);
}

bool SQLite_DB::id_exists(const unsigned ID) const
{
    bool flag = false;
    auto callback = [](void* flag, int argc, char** argv, char**) -> int 
    { 
        if(argc != 1) { return 1; }

        // If query returned 1, set flag to true
        *reinterpret_cast<bool*>(flag) = (strcmp(argv[0], "1") == 0);
        return 0;
    };

    std::string sql = "SELECT EXISTS ( SELECT 1 FROM accounts WHERE chocan_id=" + std::to_string(ID) + ");";

    const_cast<SQLite_DB&>(*this).execute_statement(sql, callback, &flag);
    return flag;
}

std::string SQLite_DB::serialize_account(const Account& account) const
{
    std::stringstream data;

    data << std::to_string(account.id())         << ", "
         << sqlquote(account.name().first())     << ", "
         << sqlquote(account.name().last())      << ", "
         << sqlquote(account.address().street()) << ", "
         << sqlquote(account.address().city())   << ", "
         << sqlquote(account.address().state())  << ", "
         << sqlquote(std::to_string(account.address().zip())) << ", "
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

std::string SQLite_DB::sqlquote(const std::string& str) const
{
    // TODO should double quote any unescaped quotes within the string as well
    return "'" + str + "'";
}

bool SQLite_DB::execute_statement(const std::string& sql, SQL_Callback callback, void* data)
{
    int rc = sqlite3_exec(db, sql.c_str(), callback, data, &err_msg);

    if(rc != SQLITE_OK)
    {
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}