/* 
File: account_builder.cpp

Brief:  Account builder performs exception handling and guides the user
        through the process of creating an account.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/core/utils/account_builder.hpp>
#include <algorithm>

bool Account_Builder::buildable() const
{
    return true; 
}


Account Account_Builder::build()
{
    if (!buildable())
    {
        throw invalid_account_build("Attempt to build prematurely", issues);
    }

    try{
        return Account(name.value(),address.value(),type.value(),id_generator);

    }catch(...){
        
        throw invalid_account_build("Failed to build account", issues);
    }
}

Account_Builder &Account_Builder::reset()
{
    type.reset();
    name.reset();
    address.reset();
    issues.clear();
    fields = {Get_Type(),Get_First(),Get_Last(),Get_Street(),Get_City(),Get_State(),Get_Zip()};

    return *this;
}

const Input_State& Account_Builder::get_field() const{

    if(buildable()) throw invalid_account_build("Cannot recieve input while account is buildable",{});

    for(auto&& field: fields){
    
        std::visit(overload{

            [&](Type& arg)  {if(!arg.type) return field;},
            [&](Word& arg)  {if(!arg.word) return field;},
            [&](Number& arg){if(!arg.num)  return field;}

        },field);
    }
}

std::optional<const invalid_account_build> Account_Builder::get_issues() const{

    if(issues.empty()) return std::optional<const invalid_account_build>();

    return invalid_account_build("Issues with account",issues);

}

void Account_Builder::set_field(const std::string& input){

    if(buildable()) return;

    issues.clear();

    for(auto&& field: fields){
    
        std::visit(overload{

            [&](Type& arg)  {if(!arg.type) arg.type = deriveType(input);},
            [&](Word& arg)  {if(!arg.word) arg.word = input;},
            [&](Number& arg){if(!arg.num)  arg.num  = deriveZip(input);}

        },field);
    }

    if(!type && fields_ready(0,0)){
        
        type = std::get<Type>(fields.at(0)).type.value();
    }
    else if(!name && fields_ready(1,2)){

        try{ 

            name = Name(std::get<Word>(fields.at(1)).word.value()
                       ,std::get<Word>(fields.at(2)).word.value());

        }catch(const invalid_name& err){

            issues = err.info();
        }

    }else if(!address && fields_ready(3,6)){

        try
        {

            address = Address(std::get<Word>(fields.at(3)).word.value()
                            , std::get<Word>(fields.at(4)).word.value()
                            , std::get<Word>(fields.at(5)).word.value()
                            , std::get<Number>(fields.at(6)).num.value());
        }
        catch (const invalid_address& err)
        {
            issues = err.info();
        }
    }
    
    reset_fields_as_needed();
}

void Account_Builder::reset_fields_as_needed(){

    if(issues.empty()) return;

    for(std::string it: issues){

        if(it.find("First") != std::string::npos){
            
            fields[1] = Get_First();
        } 
        else if(it.find("Last")  != std::string::npos){

            fields[2] = Get_Last();
        } 
        else if(it.find("Full")  != std::string::npos){

            fields[1] = Get_First();
            fields[2] = Get_Last();
        }
        else if(it.find("Street")  != std::string::npos){

            fields[3] = Get_Street();
        }
        else if(it.find("Cities")  != std::string::npos){

            fields[4] = Get_City();
        }
        else if(it.find("State")  != std::string::npos){

            fields[5] = Get_State();
        }
        else if(it.find("Zip")  != std::string::npos){

            fields[6] = Get_Zip();
        }
    }

}

bool Account_Builder::fields_ready(int from, int to) const{

    for(int i = from; i <= to; ++i){

        std::visit(overload{

            [](Type arg)  {if (!arg.type) return false;},
            [](Word arg)  {if (!arg.word) return false;},
            [](Number arg){if (!arg.num)  return false;}
        },
        fields.at(i));
    }

    return true;
}
std::optional<Account::Account_Type> Account_Builder::deriveType(const std::string& input){

    std::optional<Account::Account_Type> temp;

    return temp;
}
std::optional<unsigned> Account_Builder::deriveZip(const std::string& input){

    std::optional<unsigned> temp;

    return temp;
}
