/* 
 
File: account.cpp

Brief: Account data class implementation

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <functional>
#include <ChocAn/core/utils/overloaded.hpp>
#include <ChocAn/core/entities/account.hpp>

Account::Account(Name name, Address address, Account_Type type, const ID_Generator& id_gen)
    : _name    ( name    )
    , _address ( address )
    , _type    ( type    )
    , _id      ( id_gen.yield() )
    {}

Account::Account(Name name, Address address, Account_Type type, unsigned id, const Key<Data_Gateway>&)
    : _name    ( name    )
    , _address ( address )
    , _type    ( type    )
    , _id      ( id      )
    {}

Account::Account(const Data_Table& data, const Key<Data_Gateway>& )
    : _name    ( Name(data.at("f_name"), data.at("l_name")) )
    , _address ( Address ( data.at("street")
                         , data.at("city")
                         , data.at("state")
                         , std::stoi(data.at("zip")) ) )
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

    _type = type_constructor.at(data.at("type"))();
    _id   = std::stoi(data.at("chocan_id"));
}

Account::Data_Table Account::serialize() const
{
    std::string status = "Valid";
    std::string type = std::visit( overloaded {
        [&](const Manager&) -> std::string { return "Manager";  },
        [&](const Provider&)-> std::string { return "Provider"; },
        [&](const Member& m)-> std::string {
            status = (m.status() == Account_Status::Valid) ? status : "Suspended";
            return "Member";
        }
    }, _type);

    return 
    {
        { "f_name", _name.first() },
        { "l_name", _name.last()  },
        { "street", _address.street() },
        { "city"  , _address.city()   },
        { "state" , _address.state()  },
        { "chocan_id", std::to_string(_id) },
        { "zip"   , std::to_string(_address.zip()) },
        { "type"  , type   },
        { "status", status }
    };
}

Account::Data_Table Account::build_key_table()
{
    return 
    {
        { "f_name", ""  }, { "l_name", "" }, 
        { "street", ""  }, { "city"  , "" }, 
        { "state" , ""  }, { "zip"   , "" }, 
        {"chocan_id", ""}, {"type"   , "" }, 
        { "status"  , ""}
    };
}
