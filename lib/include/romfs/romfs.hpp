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
        std::string_view string() const {
            return { reinterpret_cast<const char*>(this->data()), this->size() + 1 };
        }

        [[nodiscard]]
        bool valid() const {
            return !this->m_content.empty() && this->m_content.data() != nullptr;
        }

    private:
        const std::span<std::byte> m_content;
    };

    [[nodiscard]]
    const Resource& get(const std::string &path);

    [[nodiscard]]
    const std::vector<std::string>& list();

}