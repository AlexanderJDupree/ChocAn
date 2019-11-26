/* 
 
File: parsers.hpp

Brief: Parsers utility provides a series of helpful string parsing tools

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_PARSERS_HPP
#define CHOCAN_PARSERS_HPP

#include <vector>
#include <string>
#include <ChocAn/core/entities/datetime.hpp>

namespace Parsers 
{

std::vector<std::string> split(std::string input, const std::string& delim)
{
    size_t pos = 0;
    std::vector<std::string> tokens;
    while ((pos = input.find(delim)) != std::string::npos) 
    {
        tokens.push_back(input.substr(0, pos));
        input.erase(0, pos + delim.length());
    }
    // push back whats left of the input if there's anything left
    (input != "") ? tokens.push_back(input) : void();
    return tokens;
}

std::string join(std::vector<std::string> tokens, const std::string& delim)
{
    if (tokens.empty()) { return ""; }

    std::string output = tokens.front();

    std::for_each(++tokens.begin(), tokens.end(), [&](const std::string& token)
    {
        output += delim + token;
    });

    return output;
}

// TODO add secondary delimiter for structure like this: MM-DD-YY HH:MM:SS
DateTime parse_date(std::string input, const std::string& structure, const std::string& delim)
{
    std::vector<std::string> tokens  = split(input, delim);
    std::vector<std::string> indices = split(structure, delim);

    auto find_token = [&](const std::string& unit)
    {
        for (size_t i = 0; i < indices.size(); ++i)
        {
            if(indices[i] == unit) { return std::stoi(tokens.at(i)); }
        }
        return 0;
    };

    try
    {
        return DateTime( Day   { find_token("DD")   }
                       , Month { find_token("MM")   }
                       , Year  { find_token("YYYY") } );
        
    }
    catch(const std::invalid_argument&)
    {
        throw invalid_datetime("Invalid Datetime, unrecognized values", {});
    }
    catch(const std::out_of_range&)
    {
        throw invalid_datetime("Invalid Datetime, unrecognized format", {});
    }
}

};


#endif // CHOCAN_PARSERS_HPP