#ifndef CheckHashable_hpp
#define CheckHashable_hpp
#include <functional>
#include <type_traits>
template <typename T, typename = std::void_t<>>
struct is_std_hashable : std::false_type { };

template <typename T>
struct is_std_hashable<T, std::void_t<decltype(std::declval<std::hash<T>>()(std::declval<T>()))>> : std::true_type { };

template <typename T>
constexpr bool is_std_hashable_v = is_std_hashable<T>::value; 
#endif