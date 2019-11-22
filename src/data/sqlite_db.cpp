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
#include <functional>
#include <ChocAn/data/sqlite_db.hpp>
#include <ChocAn/core/utils/exception.hpp>
#include <ChocAn/core/utils/overloaded.hpp>
#include <ChocAn/core/entities/account.hpp>
#include <ChocAn/core/entities/service.hpp>

SQLite_DB::SQLite_DB(const char* db_name)
{
    if(sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        throw chocan_db_exception("Fatal: Unable to connect to DB", {});
    }
}
SQLite_DB::SQLite_DB(const char* db_name, const char* schema_file)
    : SQLite_DB(db_name)
{
    if(!load_schema(schema_file))
    {
        throw chocan_db_exception("Fatal: Unable to load schema file", {});
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
    auto callback = [](void* flag, int, char** argv, char**) -> int 
    { 
        // If query returned 1, set flag to true
        *static_cast<bool*>(flag) = (strcmp(argv[0], "1") == 0);
        return 0;
    };

    std::string sql = "SELECT EXISTS ( SELECT 1 FROM accounts WHERE chocan_id=" + std::to_string(ID) + ");";

    const_cast<SQLite_DB&>(*this).execute_statement(sql, callback, &flag);
    return flag;
}

bool SQLite_DB::add_transaction(const Transaction&)
{
    return true;
}

std::optional<Service> SQLite_DB::lookup_service(const unsigned code)
{
    SQL_Row data;
    auto callback = [](void* table, int argc, char** argv, char** col_name) -> int
    {
        SQL_Row* data = static_cast<SQL_Row*>(table);

        for(int i = 0; i < argc; ++i)
        {
            data->insert( { col_name[i], argv[i] } );
        }
        return 0;
    };

    std::string sql = "SELECT * FROM services WHERE code=" + std::to_string(code) + ';';

    if(execute_statement(sql, callback, &data) && !data.empty())
    {
        return deserialize_service(data);
    }
    return { };
}

std::optional<Service> SQLite_DB::lookup_service(const std::string& code)
{
    try
    {
        return lookup_service(std::stoi(code));
    }
    catch(const std::exception&)
    {
        return { };
    }
}

std::optional<Account> SQLite_DB::get_account(const unsigned ID)
{
    SQL_Row data;
    auto callback = [](void* table, int argc, char** argv, char** col_name) -> int
    {
        SQL_Row* data = static_cast<SQL_Row*>(table);

        for(int i = 0; i < argc; ++i)
        {
            data->insert( { col_name[i], argv[i] } );
        }
        return 0;
    };

    std::string sql = "SELECT * FROM accounts WHERE chocan_id=" + std::to_string(ID) + ";";


    if(execute_statement(sql, callback, &data) && !data.empty())
    {
        return deserialize_account(data);
    }
    return { };
}
std::optional<Account> SQLite_DB::get_account(const std::string& ID)
{
    try
    {
        return get_account(std::stoi(ID));
    }
    catch(const std::exception&)
    {
        return { };
    }
}
std::optional<Account> SQLite_DB::get_member_account(const unsigned)
{
    return { };
}
std::optional<Account> SQLite_DB::get_member_account(const std::string&)
{
    return { };
}
std::optional<Account> SQLite_DB::get_provider_account(const unsigned)
{
    return { };
}
std::optional<Account> SQLite_DB::get_provider_account(const std::string&)
{
    return { };
}
std::optional<Account> SQLite_DB::get_manager_account(const unsigned)
{
    return { };
}
std::optional<Account> SQLite_DB::get_manager_account(const std::string&)
{
    return { };
}

Data_Gateway::Service_Directory SQLite_DB::service_directory()
{
    return {};
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

Account SQLite_DB::deserialize_account(const SQL_Row& data) const
{
    std::map<std::string, std::function<Account::Account_Type()>> type_constructor
    {
        { "Manager",  [&](){ return Manager();  } },
        { "Provider", [&](){ return Provider(); } },
        { "Member",   [&]()
        { 
            return Member(data.at("status") == "Suspended" ? Account_Status::Suspended 
                                                           : Account_Status::Valid); 
        } }
    };

    try
    {
        return Account( Name(data.at("f_name"), data.at("l_name"))
                      , Address( data.at("street")
                               , data.at("city")
                               , data.at("state")
                               , std::stoi(data.at("zip")) )
                      , type_constructor.at(data.at("type"))()
                      , std::stoi(data.at("chocan_id"))
                      , db_key );
    }
    catch(const std::exception&)
    {
        throw chocan_db_exception("DB: Error Failed to deserialize Account", data);
    }
}

Service SQLite_DB::deserialize_service(const SQL_Row& data) const
{
    try
    {
        return Service( std::stoi(data.at("code"))
                        , USD { std::stod(data.at("cost")) }
                        , data.at("name")
                        , db_key );
    }
    catch(const std::exception&) 
    { 
        throw chocan_db_exception("DB Error: Failed to deserialize Service", {});
    }
}

std::string SQLite_DB::sqlquote(const std::string& str) const
{
    // TODO should double quote any unescaped quotes within the string as well
    return "'" + str + "'";
}
