/* 
File: account_builder.cpp

Brief:  

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/core/utils/account_builder.hpp>

const Account_Builder::Build_Stack Account_Builder::create_account_sequence
{
    Account_Build_State::Buildable,
    Account_Build_State::Construct_Address,
    Account_Build_State::Set_Zip,
    Account_Build_State::Set_State,
    Account_Build_State::Set_City,
    Account_Build_State::Set_Street,
    Account_Build_State::Construct_Name,
    Account_Build_State::Set_LName,
    Account_Build_State::Set_FName,
    Account_Build_State::Set_Type
};

const Account_Builder::Build_Stack Account_Builder::set_address_sequence
{
    Account_Build_State::Update_Address,
    Account_Build_State::Construct_Address,
    Account_Build_State::Set_Zip,
    Account_Build_State::Set_State,
    Account_Build_State::Set_City,
    Account_Build_State::Set_Street
};

const Account_Builder::Build_Stack Account_Builder::set_name_sequence
{
    Account_Build_State::Update_Name,
    Account_Build_State::Construct_Name,
    Account_Build_State::Set_LName,
    Account_Build_State::Set_FName
};
