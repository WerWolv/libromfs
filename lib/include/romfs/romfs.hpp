#pragma once

#include <cstdint>
#include <cstddef>
#include <filesystem>
#include <string_view>
#include <span>
#include <vector>

#define ROMFS_CONCAT_IMPL(x, y) x##y
#define ROMFS_CONCAT(x, y) ROMFS_CONCAT_IMPL(x, y)

#define ROMFS_NAME ROMFS_CONCAT(RomFs_, LIBROMFS_PROJECT_NAME)

#if !defined(WIN32)
    #define ROMFS_VISIBILITY [[gnu::visibility("hidden")]]
#else
    #define ROMFS_VISIBILITY
#endif

template<typename T>
concept ByteType = std::is_trivial_v<T> && sizeof(T) == sizeof(std::byte);

namespace romfs {

    namespace impl {
        ROMFS_VISIBILITY void ROMFS_CONCAT(decompress_if_needed_, LIBROMFS_PROJECT_NAME)(std::vector<std::byte> &decompressedData, std::span<const std::byte> compressedData);
    }

    class Resource {
    public:
        constexpr Resource() = default;
        explicit constexpr Resource(const std::span<std::byte> &content) : m_compressedData(content) {}

        template<ByteType T = std::byte>
        [[nodiscard]] const T* data() const {
            impl::ROMFS_CONCAT(decompress_if_needed_, LIBROMFS_PROJECT_NAME)(m_decompressedData, m_compressedData);
            if (!m_decompressedData.empty()) [[likely]]
                return reinterpret_cast<const T*>(this->m_decompressedData.data());
            else
                return reinterpret_cast<const T*>(this->m_compressedData.data());
        }

        template<ByteType T = std::byte>
        [[nodiscard]] std::span<const T> span() const {
            return { this->data<T>(), this->size() };
        }

        [[nodiscard]]
        std::size_t size() const {
            impl::ROMFS_CONCAT(decompress_if_needed_, LIBROMFS_PROJECT_NAME)(m_decompressedData, m_compressedData);

            if (!this->m_decompressedData.empty()) [[likely]]
                return this->m_decompressedData.size() - 1;
            else
                return m_compressedData.size_bytes() - 1;
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
        bool valid() const {
            impl::ROMFS_CONCAT(decompress_if_needed_, LIBROMFS_PROJECT_NAME)(m_decompressedData, m_compressedData);
            return !this->m_decompressedData.empty() && this->m_decompressedData.data() != nullptr;
        }

    private:
        mutable std::vector<std::byte> m_decompressedData;
        std::span<const std::byte> m_compressedData;
    };

    namespace impl {

        struct ResourceLocation {
            std::string_view path;
            Resource resource;
        };

        [[nodiscard]] ROMFS_VISIBILITY const Resource& ROMFS_CONCAT(get_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &path);
        [[nodiscard]] ROMFS_VISIBILITY std::vector<std::filesystem::path> ROMFS_CONCAT(list_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &path);
        [[nodiscard]] ROMFS_VISIBILITY std::string_view ROMFS_CONCAT(name_, LIBROMFS_PROJECT_NAME)();

    }

    [[nodiscard]] ROMFS_VISIBILITY inline const Resource& get(const std::filesystem::path &path) { return impl::ROMFS_CONCAT(get_, LIBROMFS_PROJECT_NAME)(path); }
    [[nodiscard]] ROMFS_VISIBILITY inline std::vector<std::filesystem::path> list(const std::filesystem::path &path = {}) { return impl::ROMFS_CONCAT(list_, LIBROMFS_PROJECT_NAME)(path); }
    [[nodiscard]] ROMFS_VISIBILITY inline std::string_view name() { return impl::ROMFS_CONCAT(name_, LIBROMFS_PROJECT_NAME)(); }


}

#ifndef ROMFS_KEEP_MACROS
    #undef ROMFS_CONCAT_IMPL
    #undef ROMFS_CONCAT
    #undef ROMFS_NAME
    #undef ROMFS_VISIBILITY
#endif