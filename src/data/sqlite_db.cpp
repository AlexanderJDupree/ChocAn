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
#include <ChocAn/core/entities/transaction.hpp>

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
    // TODO Refactor to use the sqlite3 prepare, step, and column interface to prevent SQL injection
    int rc = sqlite3_exec(db, sql.c_str(), callback, data, &err_msg);

    if(rc != SQLITE_OK)
    {
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

unsigned SQLite_DB::create_account(const Account& account)
{
    std::stringstream sql;
    Account::Data_Table data = account.serialize();

    sql << "INSERT OR REPLACE INTO accounts VALUES (" 
        << data["chocan_id"] << ", "
        << sqlquote(data["f_name"]) << ", "
        << sqlquote(data["l_name"]) << ", "
        << sqlquote(data["street"]) << ", "
        << sqlquote(data["city"])   << ", "
        << sqlquote(data["state"])  << ", "
        << sqlquote(data["zip"])    << ", "
        << sqlquote(data["type"])   << ", "
        << sqlquote(data["status"]) << ");";
    

    if(execute_statement(sql.str(), no_callback)) { return account.id(); }
    return 0;
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

unsigned SQLite_DB::add_transaction(const Transaction& transaction)
{
    std::stringstream sql;
    Transaction::Data_Table data = transaction.serialize();

    sql << "INSERT INTO transactions VALUES ("
        << data["service_date"] << ','
        << data["filed_date"]   << ','
        << data["provider_id"]  << ','
        << data["member_id"]    << ','
        << data["service_code"] << ','
        << sqlquote(data["comments"]) << ");";

    if(execute_statement(sql.str(), no_callback)) 
    { 
        // TODO having the primary key as the filed date will cause a collision
        // if two transations are filed within the same second
        return transaction.filed_date().unix_timestamp();
    }
    return 0;
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
        try
        {
            return Service(data, db_key);
        }
        catch(const std::exception& e)
        {
            // TODO log error
        }
        
    }
    return { }; // Lookup failed
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

std::optional<Account> SQLite_DB::get_account(const unsigned ID, const std::string& type)
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

    std::string acct_type = (type == "*") ? "" : " and type=" + sqlquote(type);
    std::string sql = "SELECT * FROM accounts WHERE chocan_id=" + std::to_string(ID) + acct_type + ";";

    if(execute_statement(sql, callback, &data) && !data.empty())
    {
        try
        {
            return Account(data, db_key);
        }
        catch(const std::exception&)
        {
            // TODO record error in a logger
        }
    }
    return { };
}
std::optional<Account> SQLite_DB::get_account(const std::string& ID, const std::string& type)
{
    try
    {
        return get_account(std::stoi(ID), type);
    }
    catch(const std::exception&)
    {
        return { };
    }
}
std::optional<Account> SQLite_DB::get_account(const unsigned ID)
{
    return get_account(ID, "*");
}
std::optional<Account> SQLite_DB::get_account(const std::string& ID)
{
    return get_account(ID, "*");
}
std::optional<Account> SQLite_DB::get_member_account(const unsigned ID)
{
    return get_account(ID, "Member");
}
std::optional<Account> SQLite_DB::get_member_account(const std::string& ID)
{
    return get_account(ID, "Member");
}
std::optional<Account> SQLite_DB::get_provider_account(const unsigned ID)
{
    return get_account(ID, "Provider");
}
std::optional<Account> SQLite_DB::get_provider_account(const std::string& ID)
{
    return get_account(ID, "Provider");
}
std::optional<Account> SQLite_DB::get_manager_account(const unsigned ID)
{
    return get_account(ID, "Manager");
}
std::optional<Account> SQLite_DB::get_manager_account(const std::string& ID)
{
    return get_account(ID, "Manager");
}

Data_Gateway::Service_Directory SQLite_DB::service_directory()
{
    Service_Directory directory;

    std::vector<unsigned> service_codes;
    auto callback = [](void* codes, int, char** argv, char**) -> int
    {
        std::vector<unsigned>* service_codes = static_cast<std::vector<unsigned>*>(codes);

        service_codes->push_back(std::stoi(argv[0]));

        return 0;
    };

    std::string sql = "SELECT code FROM services;";

    if (execute_statement(sql, callback, &service_codes))
    {
        std::for_each(service_codes.begin(), service_codes.end(), [&](unsigned code)
        {
            if( auto maybe_service = lookup_service(code))
            {
                directory.insert( { code, maybe_service.value() } );
            }
        } );
    }
    return directory;
}

std::string SQLite_DB::sqlquote(const std::string& str) const
{
    // TODO should double quote any unescaped quotes within the string as well
    return "'" + str + "'";
}
