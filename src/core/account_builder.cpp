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

bool Account_Type::parse_input(const std::string& input){ 
    
    switch(input[0]){

        case '1': account_type = Manager();
                  break;
        case '2': account_type = Member();
                  break;
        case '3': account_type = Provider();
                  break;
    
        default: return false;
    }

    return true; 
};

bool First_Name::parse_input(const std::string& input){ 

    return true; 
};

bool Last_Name::parse_input(const std::string& input){ 

    return true; 
};

bool Street::parse_input(const std::string& input){ 

    return true; 
};

bool City::parse_input(const std::string& input){ 

    return true; 
};

bool State::parse_input(const std::string& input){ 

    return true; 
};

bool Zip::parse_input(const std::string& input){ 

    return true; 
};


bool Account_Builder::buildable() const
{
    return true; 
}

Account Account_Builder::build()
{
    if (!buildable())
        throw invalid_account_build("Failed to build account", issues);
    
    return Account(
            Name(   std::get<First_Name>(build_phase).first_name.value_or(""),
                    std::get<Last_Name>(build_phase).last_name.value_or("")),
           Address( std::get<Street>(build_phase.street.value_or("")),
                    std::get<City>(build_phase.city.value_or("")),
                          build_phase.state.value_or(""),
                          build_phase.zip.value_or(0)), 
                  build_phase.account_type.value(),           
                  id_generator);                            

}

Account_Builder &Account_Builder::reset()
{

    while (!build_phase.empty())
        build_phase.pop();

    Build_Instructions building_phase;
    
    build_phase.push(building_phase.street_address);
    build_phase.push(building_phase.full_name);
    build_phase.push(building_phase.account_type);

    name.reset();
    address.reset();
    zip.reset();
    issues.reset();

    account_accepted = false;

    return *this;
}

void Account_Builder::set_current_field(const std::string &input)
{

    Build_Instructions building_phase;
   
    if (build_phase.empty())
    {

        account_accepted = confirm_account(input);

        if (!account_accepted){

            reset();
            issues.emplace(invalid_account_build(building_phase.account_rejected,{ "With input value: " + input}));

        }
        
        return;
    }

    std::string current_phase = build_phase.top();

    if (current_phase == building_phase.account_type)
    {
        std::string type = valid_account_type(input);

        if (type != "")
            account_field[Field_Types::Account_Type] = type;

        else
            issues.emplace(chocan_user_exception(building_phase.invalid_type_msg, {}));
    }
    else if (current_phase == building_phase.full_name)
    {
        parseName(input);

        try
        {

            name = Name(account_field.at(Field_Types::First_Name), account_field.at(Field_Types::Last_Name));
        }
        catch (const chocan_user_exception &err)
        {

            issues.emplace(err);
        }
    }
    else if (current_phase == building_phase.street_address)
    {

        parseAddress(input);

        try
        {
            zip = std::stol(account_field.at(Field_Types::Zip));
        }
        catch (const std::invalid_argument &err)
        {
            zip = 0;
        }
        catch (const std::out_of_range &err)
        {
            zip = 0;
        }

        try
        {
            address = Address(account_field.at(Field_Types::Street), account_field.at(Field_Types::City), account_field.at(Field_Types::State), zip.value());
        }
        catch (const chocan_user_exception &err)
        {
            issues.emplace(err);

        }
    }

    if (!issues)
        build_phase.pop();

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

    if (build_phase.empty())
        return review_account();

    return build_phase.top();
}

void Account_Builder::parseName(const std::string &input)
{

    size_t delimiter_position = input.find(",");

    if(delimiter_position == std::string::npos){
    
        account_field[Field_Types::First_Name] = "";
        account_field[Field_Types::Last_Name] = "";

    }else{

        account_field[Field_Types::First_Name] = input.substr(0, input.find(","));
        account_field[Field_Types::Last_Name] = input.substr(input.find(",") + 1, input.length());

        remove_leading_white_space(account_field[Field_Types::First_Name]);
        remove_leading_white_space(account_field[Field_Types::Last_Name]);
    }

}

void Account_Builder::parseAddress(const std::string &input)
{

    size_t delimiter_position = 0;
    const std::string delimiter = ",";
    std::string temp_input(input + delimiter);
    std::stack<Field_Types> current_field({Field_Types::Zip, Field_Types::State, Field_Types::City, Field_Types::Street});

    while (!current_field.empty() && (delimiter_position = temp_input.find(delimiter)) != std::string::npos)
    {
        account_field[current_field.top()] = temp_input.substr(0, delimiter_position);

        remove_leading_white_space(account_field[current_field.top()]);

        current_field.pop();

        temp_input.erase(0, delimiter_position + delimiter.length());
    }

    while(!current_field.empty()){
        account_field[current_field.top()] = "";
        current_field.pop();
    }

    account_field[Field_Types::Zip] = account_field[Field_Types::Zip].substr(0,5);
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
           "\nType   : " + account_field.at(Field_Types::Account_Type)   + " " + 
           "\nName   : " + account_field.at(Field_Types::First_Name)  + " " 
                         + account_field.at(Field_Types::Last_Name)   + 
           "\nAddress: " + account_field.at(Field_Types::Street) + " " 
                         + account_field.at(Field_Types::City)   + " " 
                         + account_field.at(Field_Types::State)  + " " 
                         + account_field.at(Field_Types::Zip);
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
