#pragma once

#include <cinttypes>
#include <bitset>

namespace ECS {
    using Entity = std::uint32_t;
    constexpr Entity MAX_ENTITIES = 1024;

    using ComponentType = std::uint8_t;
    constexpr ComponentType MAX_COMPONENTS = 16;

    using Signature = std::bitset<MAX_COMPONENTS>;
};