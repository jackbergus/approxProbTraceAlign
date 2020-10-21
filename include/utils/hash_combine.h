//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_HASH_COMBINE_H
#define FUZZYSTRINGMATCHING2_HASH_COMBINE_H

// @author: https://stackoverflow.com/a/50978188/1376095
#include <limits>
#include <cstdint>
#include <type_traits>
#include <string>

template<typename T>
T xorshift(const T& n,int i){
    return n^(n>>i);
}

uint32_t distribute(const uint32_t& n);

uint64_t hash(const uint64_t& n);

// if c++20 rotl is not available:
template <typename T,typename S>
typename std::enable_if<std::is_unsigned<T>::value,T>::type
constexpr rotl(const T n, const S i){
    const T m = (std::numeric_limits<T>::digits-1);
    const T c = i&m;
    return (n<<c)|(n>>((T(0)-c)&m)); // this is usually recognized by the compiler to mean rotation, also c++20 now gives us rotl directly
}

template <class T> size_t hash_combine(const std::size_t seed, const T& v)
{
    return rotl(seed,std::numeric_limits<size_t>::digits/3) ^ distribute(std::hash<T>{}(v));
}

/*namespace std {

    template <typename T>
    struct hash<std::vector<T>>
    {
        std::size_t operator()(const std::vector<T>& k) const
        {
            size_t seed = 31;
            for (size_t i = 0; i<k.size(); i++) {
                seed = hash_combine(seed, std::hash<T>(k[i]));
            }
            return seed;
        }
    };

}*/

#endif //FUZZYSTRINGMATCHING2_HASH_COMBINE_H
