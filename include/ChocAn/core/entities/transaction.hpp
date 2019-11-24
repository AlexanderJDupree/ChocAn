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

class Transaction : public Serializable<Transaction, std::string, std::string>
{
public:

    Transaction( const Account& provider
               , const Account& member
               , const DateTime& service_date
               , const Service& service 
               , const std::string& comments );

    Data_Table serialize() const override;

    const DateTime& service_date() const { return _service_date; }
    const DateTime& filed_date()   const { return _filed_date;   }

    const Account& provider() const { return _provider; }
    const Account& member()   const { return _member;   }

    const Service& service() const { return _service; }

    const std::string& comments() const { return _comments; }

private:

    DateTime _service_date;
    DateTime _filed_date;

    Account _provider;
    Account _member;

    Service _service;
    std::string _comments;
};

struct invalid_transaction : public chocan_user_exception
{
    explicit invalid_transaction(const char* err, Info info) 
        : chocan_user_exception(err, info)
        { }
};

#endif  // CHOCAN_TRANSACTION_HPP