#ifndef RRENDERER_ENGINE_UTILITY_STRING_HASH_HPP
#define RRENDERER_ENGINE_UTILITY_STRING_HASH_HPP

#include <cstddef>
#include <string_view>

namespace rr
{

struct StringHash
{
    using isTransparent = void; // NOTE: needed for heterogenous operations

    std::size_t operator()(std::string_view sv) const
    {
        std::hash<std::string_view> hasher;
        return hasher(sv);
    }
};

} // !rr

#endif // !RRENDERER_ENGINE_UTILITY_STRING_HASH_HPP
