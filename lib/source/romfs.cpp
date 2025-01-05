#include <romfs/romfs.hpp>

#include <ranges>

#if defined(LIBROMFS_COMPRESS_RESOURCES)
    #include <zlib.h>
#endif

std::span<romfs::impl::ResourceLocation> ROMFS_CONCAT(ROMFS_NAME, _get_resources)();
std::span<std::string_view> ROMFS_CONCAT(ROMFS_NAME, _get_paths)();
const char* ROMFS_CONCAT(ROMFS_NAME, _get_name)();

namespace romfs {

    ROMFS_VISIBILITY void impl::ROMFS_CONCAT(decompress_if_needed_, LIBROMFS_PROJECT_NAME)(std::vector<std::byte> &decompressedData, std::span<const std::byte> compressedData) {
        if (!decompressedData.empty() || compressedData.empty())
            return;

        #if defined(LIBROMFS_COMPRESS_RESOURCES)
            z_stream stream;
            stream.zalloc = Z_NULL;
            stream.zfree = Z_NULL;
            stream.opaque = Z_NULL;
            stream.avail_in = compressedData.size();
            stream.next_in = const_cast<std::uint8_t*>(reinterpret_cast<const std::uint8_t*>(compressedData.data()));
            stream.avail_out = 0x00;

            // Initialize the zlib inflate operation
            if (inflateInit(&stream) != Z_OK) {
                throw std::runtime_error("Failed to decompress romfs data!");
            }

            decompressedData.resize(compressedData.size() * 2); // Initial guess, adjust as needed

            int ret;
            do {
                // Ensure enough space in the output buffer
                if (stream.avail_out == 0) {
                    decompressedData.resize(decompressedData.size() * 2);
                    stream.avail_out = decompressedData.size() - stream.total_out;
                    stream.next_out = reinterpret_cast<std::uint8_t*>(decompressedData.data()) + stream.total_out;
                }

                // Perform the decompression
                ret = inflate(&stream, Z_NO_FLUSH);
                if (ret == Z_MEM_ERROR || ret == Z_DATA_ERROR) {
                    inflateEnd(&stream);
                    throw std::runtime_error("Failed to decompress romfs data! inflate() failed");
                }
            } while (ret != Z_STREAM_END);

            // Resize the output buffer to the actual size
            decompressedData.resize(stream.total_out);

            // Clean up
            inflateEnd(&stream);
        #else
            std::copy(compressedData.begin(), compressedData.end(), std::back_inserter(decompressedData));
        #endif
    }


    ROMFS_VISIBILITY const romfs::Resource &impl::ROMFS_CONCAT(get_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &path) {
        for (const auto &[resourcePath, resourceData] : ROMFS_CONCAT(ROMFS_NAME, _get_resources)()) {
            if (path == resourcePath)
                return resourceData;
        }

        throw std::invalid_argument(std::string("Invalid romfs resource path! File '") + std::string(romfs::name()) + "' : " + path.string());
    }

    ROMFS_VISIBILITY std::vector<std::filesystem::path> impl::ROMFS_CONCAT(list_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &parent) {
        std::vector<std::filesystem::path> result;
        for (const auto &pathString : ROMFS_CONCAT(ROMFS_NAME, _get_paths)()) {
            auto path = std::filesystem::path(pathString);
            if (std::ranges::mismatch(parent, path).in1 == parent.end())
                result.emplace_back(std::move(path));
        }

        return result;
    }

    ROMFS_VISIBILITY std::string_view impl::ROMFS_CONCAT(name_, LIBROMFS_PROJECT_NAME)() {
        return ROMFS_CONCAT(ROMFS_NAME, _get_name)();
    }

}