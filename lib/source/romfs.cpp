#include <romfs/romfs.hpp>

#include <map>

struct ROMFS_NAME {
    static std::map<std::filesystem::path, romfs::Resource> resources;
    static std::vector<std::filesystem::path> paths;
};

namespace romfs {

    const romfs::Resource &impl::ROMFS_CONCAT(get_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &path) {
        return ROMFS_NAME::resources[path];
    }

    std::vector<std::filesystem::path> impl::ROMFS_CONCAT(list_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &parent) {
        if (parent.empty()) {
            return ROMFS_NAME::paths;
        } else {
            std::vector<std::filesystem::path> result;
            for (const auto &p : ROMFS_NAME::paths)
                if (p.parent_path() == parent)
                    result.push_back(p);

            return result;
        }
    }

}