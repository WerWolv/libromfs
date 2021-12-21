#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <span>

namespace romfs {

    struct Resource {
        std::span<std::byte> content;
    };

    Resource get(const std::string &path);

}