#ifndef RRENDERER_ENGINE_UTILITY_STRING_HASH_HPP
#define RRENDERER_ENGINE_UTILITY_STRING_HASH_HPP

#include <cstddef>
#include <functional>
#include <string_view>

namespace rr
{

/// \brief Struct that helps with calculating hashes for std::string_view
///
/// Implementation of a struct to help with hashing std::string_view is needed to store them in containers like
/// std::unordered_set or similar.
///
/// \author Felix Hommel
/// \date 7/18/2025
struct StringHash
{
    using is_transparent = void; // NOTE: needed for heterogeneous operations

    std::size_t operator()(std::string_view sv) const
    {
        std::hash<std::string_view> hasher;
        return hasher(sv);
    }
};

} // !rr

#endif // !RRENDERER_ENGINE_UTILITY_STRING_HASH_HPP
