#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <span>
#include <vector>

namespace romfs {

    class Resource {
    public:
        constexpr Resource() : m_content() {}
        explicit constexpr Resource(const std::span<std::byte> &content) : m_content(content) {}

        [[nodiscard]]
        constexpr std::byte* data() const {
            return this->m_content.data();
        }

        [[nodiscard]]
        constexpr std::size_t size() const {
            return this->m_content.size();
        }

        [[nodiscard]]
        bool valid() const {
            return !this->m_content.empty() && this->m_content.data() != nullptr;
        }

    private:
        const std::span<std::byte> m_content;
    };

    [[nodiscard]]
    const Resource& get(std::string_view path);

    [[nodiscard]]
    const std::vector<std::string_view>& list();

}