#pragma once

#include <cstdint>
#include <cstddef>
#include <filesystem>
#include <string>
#include <span>
#include <vector>

#define ROMFS_CONCAT_IMPL(x, y) x##y
#define ROMFS_CONCAT(x, y) ROMFS_CONCAT_IMPL(x, y)

#define ROMFS_NAME ROMFS_CONCAT(RomFs_, LIBROMFS_PROJECT_NAME)

namespace romfs {

    class Resource {
    public:
        constexpr Resource() : m_content() {}
        explicit constexpr Resource(const std::span<std::byte> &content) : m_content(content) {}

        [[nodiscard]]
        constexpr const std::byte* data() const {
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
        std::u8string_view u8string() const {
            return { reinterpret_cast<const char8_t *>(this->data()), this->size() + 1 };
        }

        [[nodiscard]]
        constexpr bool valid() const {
            return !this->m_content.empty() && this->m_content.data() != nullptr;
        }

    private:
        const std::span<const std::byte> m_content;
    };

    [[nodiscard]]
    const Resource& ROMFS_CONCAT(get_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &path);

    inline const Resource& get(const std::filesystem::path &path) { return ROMFS_CONCAT(get_, LIBROMFS_PROJECT_NAME)(path); }

    [[nodiscard]]
    const std::vector<std::filesystem::path>& list();

}