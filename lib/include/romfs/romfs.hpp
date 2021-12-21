#pragma once

#include <cstdint>
#include <string>

namespace romfs {

    struct Resource {
        const unsigned char* data;
        size_t size;
    };

    Resource get(const std::string &path);

}