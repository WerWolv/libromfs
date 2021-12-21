#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <span>

namespace romfs {

    struct Resource {
        std::span<std::byte> content;
    };

    Resource get(std::string_view path);

}