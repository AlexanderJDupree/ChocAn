/* 
 
File: serializable.hpp

Brief: Serializable defines an interface for objects that can be serialized.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_SERIALIZABLE_HPP
#define CHOCAN_SERIALIZABLE_HPP

#include <map>

// T is the serializable type, K is the key type, V is the value type
template <typename T, typename K, typename V>
class Serializable
{
public:

    using Data_Table = std::map<K,V>;

    virtual Data_Table serialize() const = 0;
};

#endif // CHOCAN_SERIALIZABLE_HPP