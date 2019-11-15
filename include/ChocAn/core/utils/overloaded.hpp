/* 

File: overloaded.cpp

Brief: Allows for overloaded lambdas 
       https://en.cppreference.com/w/cpp/utility/variant/visit

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_OVERLOADED_LAMBDA_HPP
#define CHOCAN_OVERLOADED_LAMBDA_HPP

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

#endif // CHOCAN_OVERLOADED_LAMBDA_HPP
