/* 
 
File: transaction.cpp

Brief: Transaction implementation

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/core/utils/validators.hpp>
#include <ChocAn/core/entities/transaction.hpp>

Transaction::Transaction( const Account&     provider 
                        , const Account&     member 
                        , const DateTime&    service_date
                        , const Service&     service
                        , const std::string& comments)
                : _service_date ( service_date )
                , _filed_date   ( DateTime::get_current_datetime() )
                , _provider     ( provider )
                , _member       ( member   )
                , _service      ( service  )
                , _comments     ( comments )
{
    chocan_user_exception::Info errors;

    if( !std::holds_alternative<Provider>(_provider.type()) )
    {
        errors["Provider"] = Invalid_Value { "Account", "is not a provider account"};
    }
    if( !std::holds_alternative<Member>(_member.type()) )
    {
        errors["Member"] = Invalid_Value { "Account", "is not a member account"};
    }
    if( service_date > _filed_date )
    {
        errors["Service date"] = Invalid_Value { "", "cannot be future dated" };
    }
    if( !Validators::length(comments, 0, 100) )
    {
        errors["Comments"] = Invalid_Length { comments, 0, 100 };
    }
    ( !errors.empty() )
        ? throw invalid_transaction("Invalid Transaction fields", errors)
        : void();
}

Transaction::Transaction( const Account& provider
                        , const Account& member
                        , const Service& service 
                        , const DateTime& service_date
                        , const DateTime& filed_date
                        , const std::string& comments
                        , const Key<Data_Gateway>& )
                : _service_date( service_date )
                , _filed_date  ( filed_date   )
                , _provider    ( provider     )
                , _member      ( member       )
                , _service     ( service      )
                , _comments    ( comments     )
{ 
    // Database constructor ASSUMES the data is stored in valid state
}

Transaction::Data_Table Transaction::serialize() const
{
    DateTime::Data_Table service_date = _service_date.serialize();
    DateTime::Data_Table filed_date   = _filed_date.serialize();
    return 
    {
        { "service_date_alt", service_date["month"] + '-' + service_date["day"] + '-' + service_date["year"] },
        { "filed_date_alt"  , filed_date["month"] + '-' + filed_date["day"] + '-' + filed_date["year"] + ' ' 
                            + filed_date["hour"] + ':' + filed_date["minutes"] + ':' + filed_date["seconds"] },
        { "service_date",  service_date["unix"] },
        { "filed_date"  ,  filed_date["unix"] },
        { "provider_name", _provider.name().first() + ' ' + _provider.name().last() },
        { "provider_id" ,  std::to_string(_provider.id())  },
        { "member_name", _member.name().first() + ' ' + _member.name().last() },
        { "member_id"   ,  std::to_string(_member.id())    },
        { "service_code", std::to_string(_service.code()) },
        { "service_cost", _service.cost().to_string() },
        { "service_name", _service.name() },
        { "comments", _comments }
    };
}
