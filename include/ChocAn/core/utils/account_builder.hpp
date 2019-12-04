/* 
File: account_builder.hpp

Brief:  

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_ACCOUNT_BUILDER_HPP
#define CHOCAN_ACCOUNT_BUILDER_HPP

#include <optional>
#include <functional>
#include <ChocAn/core/entities/account.hpp>

enum class Account_Build_State { Set_Type
                               , Set_FName
                               , Set_LName
                               , Construct_Name
                               , Set_Street
                               , Set_City
                               , Set_State
                               , Set_Zip
                               , Construct_Address
                               , Buildable 
                               , Update_Name
                               , Update_Address };

class Account_Builder
{
public:

    using Build_Stack = std::vector<Account_Build_State>;

    static const Build_Stack set_name_sequence;
    static const Build_Stack set_address_sequence;
    static const Build_Stack create_account_sequence;

    // Defaults to building full create account sequence
    Account_Builder( Build_Stack sequence = create_account_sequence )
        : build_stack ( sequence )
    { }

    bool buildable() const noexcept
    {
        return status() == Account_Build_State::Buildable;
    }

    bool can_update() const noexcept
    {
        return status() == Account_Build_State::Update_Address 
            || status() == Account_Build_State::Update_Name;
    }

    Account_Build_State status() const noexcept 
    { 
        return build_stack.back(); 
    }

    std::optional<Account> build(const ID_Generator& id_gen)
    {
        if(status() != Account_Build_State::Buildable)
        {
            return { };
        }

        return Account { name.value()
                       , address.value()
                       , type.value()
                       , id_gen };
    }

    Account& update(Account& account)
    {
        switch (status())
        {
        case Account_Build_State::Update_Name    : account.name() = name.value(); 
            break;
        case Account_Build_State::Update_Address : account.address() = address.value(); 
            break;
        default: break;
        }
        return account;
    }

    std::optional<chocan_user_exception> get_last_error() const noexcept { return error; }

    void set_current_field(const std::string& input)
    {
        error.reset();

        switch (status())
        {
        case Account_Build_State::Set_Type   : return set_type(input);
        case Account_Build_State::Set_FName  : return set_name(input);
        case Account_Build_State::Set_LName  : return set_name(input);
        case Account_Build_State::Set_Street : return set_address(input);
        case Account_Build_State::Set_City   : return set_address(input);
        case Account_Build_State::Set_State  : return set_address(input);
        case Account_Build_State::Set_Zip    : return set_address(input);
        default : return;
        }
    }

    void set_type(std::string input)
    {
        std::map<std::string, std::function<Account::Account_Type()>> type_constructor
        {
            { "manager",  [](){ return Manager();  } },
            { "provider", [](){ return Provider(); } },
            { "member",   [](){ return Member();   } }
        };

        // Lower Case the input
        std::for_each(input.begin(), input.end(), [](char& c){
            c = std::tolower(c);
        });

        try
        {
            // Lookup the type in the type constructor table
            type.emplace(type_constructor.at(input)());
            build_stack.pop_back();
        }
        catch(const std::out_of_range&)
        {
            error.emplace(chocan_user_exception("Invalid Account Type", {}));
        }
    }

    void set_name(const std::string& input)
    {
        inputs.push_back(input);
        build_stack.pop_back();

        // We have enough inputs to create the name field
        if (status() == Account_Build_State::Construct_Name)
        {
            try
            {
                name.emplace(inputs[0], inputs[1]);

                build_stack.pop_back();
            }
            catch(const chocan_user_exception& err)
            {
                error.emplace(err);

                // Restart Name build sequence
                build_stack.insert( build_stack.end()
                                  , { Account_Build_State::Set_LName
                                    , Account_Build_State::Set_FName 
                                    } );
            }
            inputs.clear();
        }
    }

    void set_address(const std::string& input)
    {
        inputs.push_back(input);
        build_stack.pop_back();

        if (status() == Account_Build_State::Construct_Address)
        {
            try
            {
                address.emplace(inputs[0], inputs[1], inputs[2], std::stoi(inputs[3]));

                inputs.clear();

                build_stack.pop_back();
            }
            catch(const chocan_user_exception& err)
            {
                error.emplace(err);

                build_stack.insert( build_stack.end()
                                  , { Account_Build_State::Set_Zip
                                    , Account_Build_State::Set_State
                                    , Account_Build_State::Set_City
                                    , Account_Build_State::Set_Street
                                    } );
            }
            inputs.clear();
        }
    }

private:

    std::vector<Account_Build_State> build_stack;

    std::vector<std::string> inputs;

    std::optional<Name> name;
    std::optional<Address> address;
    std::optional<Account::Account_Type> type;

    std::optional<chocan_user_exception> error;
};

#endif // CHOCAN_ACCOUNT_BUILDER_HPP