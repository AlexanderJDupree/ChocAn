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
#include <ChocAn/core/utils/validators.hpp>
#include <algorithm>

const std::string account_type   = "Account Type : Manager OR Provider OR Member\nMUST TYPE EXACTLY ONE OF THESE OPTIONS";
const std::string full_name      = "Full Name As : First[1-24], Last[1-24]\nTOTAL CHARACTER LIMIT [2-25]";
const std::string street_address = "Address As   : Street[1-25], City[1-14], State[2], Zip[5]\nTOTAL CHARACTER LIMIT [9-46]";


bool Account_Builder::buildable() const
{
    return fields.empty() && account_accepted;
}

Account Account_Builder::build()
{

    if (!buildable())
        throw chocan_user_exception("Cannot create account with empty fields", {});

    if (account_info.at(current_type) == "Manager")
        return Account(name.value(), address.value(), Manager(), id_generator);

    if (account_info.at(current_type) == "Member")
        return Account(name.value(), address.value(), Member(), id_generator);

    if (account_info.at(current_type) == "Provider")
        return Account(name.value(), address.value(), Provider(), id_generator);

    throw chocan_user_exception("ERROR: account type not recognized", {});
}

Account_Builder &Account_Builder::reset()
{

    while (!fields.empty())
        fields.pop();

    fields.push(street_address);
    fields.push(full_name);
    fields.push(account_type);

    name.reset();
    address.reset();
    zip.reset();
    issues.reset();

    account_accepted = false;

    return *this;
}

void Account_Builder::set_current_field(const std::string &input)
{

    if (fields.empty())
    {

        account_accepted = confirm_account(input);

        if (!account_accepted)
            reset();

        return;
    }

    std::string current_field = fields.top();

    if (current_field == account_type)
    {
        std::string type = valid_account_type(input);

        if (type != "")
            account_info[current_type] = type;

        else
            issues.emplace(chocan_user_exception("Invalid Account type requested", {}));
    }
    else if (current_field == full_name)
    {
        parseName(input);

        try
        {

            name = Name(account_info.at(current_first), account_info.at(current_last));
        }
        catch (const chocan_user_exception &err)
        {

            issues.emplace(err);
        }
    }
    else if (current_field == street_address)
    {

        parseAddress(input);

        try
        {

            zip = std::stol(account_info.at(current_zip));
        }
        catch (const std::invalid_argument &err)
        {

            issues.emplace(chocan_user_exception("Zip must be 5 digit number", {}));
            zip.reset();
        }
        catch (const std::out_of_range &err)
        {
            issues.emplace(chocan_user_exception("Zip value was out of range", {}));
            zip.reset();
        }

        try
        {
            address = Address(account_info.at(current_street), account_info.at(current_city), account_info.at(current_state), zip.value());
        }
        catch (const chocan_user_exception &err)
        {
            issues.emplace(err);

        }catch(const std::bad_optional_access){
            
            issues.emplace(chocan_user_exception("Zip entered was not a number", {}));
            zip.reset();
        }
    }

    if (!issues)
        fields.pop();

    return;
}

const chocan_user_exception Account_Builder::get_current_issues()
{

    std::optional<chocan_user_exception> return_msg = issues;

    issues.reset();

    return return_msg.value_or(chocan_user_exception{"", {}});
}

const std::string Account_Builder::get_current_field() const
{

    if (fields.empty())
        return review_account();

    return fields.top();
}

void Account_Builder::parseName(const std::string &input)
{

    account_info[current_first] = input.substr(0, input.find(","));
    account_info[current_last] = input.substr(input.find(",") + 1, input.length());

    remove_leading_white_space(account_info[current_first]);
    remove_leading_white_space(account_info[current_last]);
}

void Account_Builder::parseAddress(const std::string &input)
{

    size_t delimiter_position = 0;
    const std::string delimiter = ",";
    std::string temp_input(input + delimiter);
    std::stack<field_types> current_field({current_zip, current_state, current_city, current_street});

    while ((delimiter_position = temp_input.find(delimiter)) != std::string::npos)
    {

        account_info[current_field.top()] = temp_input.substr(0, delimiter_position);

        remove_leading_white_space(account_info[current_field.top()]);

        current_field.pop();

        temp_input.erase(0, delimiter_position + delimiter.length());
    }
}

const std::string Account_Builder::valid_account_type(const std::string &input)
{

    std::string temp("");

    for (std::vector<std::string>::iterator it = valid_types.begin(); it <= valid_types.end(); ++it)
    {

        if (input == *it)
        {

            temp = *it;
            temp[0] = toupper(temp[0]);
            it = valid_types.end();
        }
    }

    remove_leading_white_space(temp);
    
    return temp;
}

const std::string Account_Builder::review_account() const
{

    return "(Y)es or (N)o to confirm/reject this account."
           "\nType   : " + account_info.at(current_type)   + " " + 
           "\nName   : " + account_info.at(current_first)  + " " 
                         + account_info.at(current_last)   + 
           "\nAddress: " + account_info.at(current_street) + " " 
                         + account_info.at(current_city)   + " " 
                         + account_info.at(current_state)  + " " 
                         + account_info.at(current_zip);
}

bool Account_Builder::confirm_account(const std::string &input) const
{

    switch (input[0])
    {

    case 'y':
    case 'Y':
        return true;
    case 'n':
    case 'N':
    default:;
    }

    return false;
}

void Account_Builder::remove_leading_white_space(std::string& str){

    if(str == "") return;

    int i = 0;

    while(str[i] == ' ') ++i;

    if(i) str = str.substr(i,str.length());

}
