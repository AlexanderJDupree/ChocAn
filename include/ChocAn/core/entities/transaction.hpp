/* 
 
File: transaction.hpp

Brief: Transaction entity holds data related to a chocAn service transaction

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_TRANSACTION_HPP
#define CHOCAN_TRANSACTION_HPP

#include <ChocAn/core/utils/exception.hpp>
#include <ChocAn/core/entities/account.hpp>
#include <ChocAn/core/entities/service.hpp>
#include <ChocAn/core/entities/datetime.hpp>

class Transaction
{
public:

    Transaction( const Account& provider
               , const Account& member
               , const DateTime& service_date
               , const Service& service 
               , const std::string& comments );

    const DateTime service_date;
    const DateTime current_date;

    const Account provider;
    const Account member;

    const Service& service;
    const std::string& comments;

};

struct invalid_transaction : public chocan_user_exception
{
    explicit invalid_transaction(const char* err, Info info) 
        : error_info ( std::move(info) )
        , error_msg  ( err ) 
        { }

    const char* what() const noexcept
    {
        return error_msg;
    }

    const Info& info() const noexcept
    {
        return error_info;
    }

    const Info error_info;
    const char* error_msg;
};

#endif  // CHOCAN_TRANSACTION_HPP