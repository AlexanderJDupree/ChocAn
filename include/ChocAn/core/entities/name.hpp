/* 
 
File: name.hpp

Brief: Name handles logic and checks for name fields in accounts

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_NAME_HPP
#define CHOCAN_NAME_HPP

#include <string>
#include <ChocAn/core/utils/exception.hpp>
#include <vector>

class Name
{
public:

    Name(const std::string& first, const std::string& last);

    bool operator==(const Name& rhs) const;
    bool operator!=(const Name& rhs) const;

    const std::string& first() const { return _first; }
    const std::string& last() const  { return _last;  }

private:

    std::string _first;
    std::string _last;
};

struct invalid_name : public chocan_user_exception
{
    explicit invalid_name(const char* err, Info info) 
        : chocan_user_exception(err, info)
        { }
    struct Bad_First{};
    struct Bad_Last{};
    struct Bad_Full{};

    using Name_Errors = std::variant<Bad_First,Bad_Last,Bad_Full>;
    std::vector<Name_Errors> specific_errors;
};


#endif // CHOCAN_NAME_HPP
