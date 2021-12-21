#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <span>
#include <vector>

namespace romfs {

    struct Resource {
        std::span<std::byte> content;
    };

    [[nodiscard]]
    const Resource& get(std::string_view path);

    [[nodiscard]]
    const std::vector<std::string_view>& list();

}