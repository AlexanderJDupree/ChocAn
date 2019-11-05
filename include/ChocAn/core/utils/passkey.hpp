/* 
 
File: passkey.hpp

Brief: PassKey is a generic access protection class that allows users to expose
       constructors or functions to specific classes. 

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_PASSKEY_HPP
#define CHOCAN_PASSKEY_HPP

template <typename T>
class Key { friend T; Key() {} Key(Key const&) {} };

#endif // CHOCAN_PASSKEY_HPP
