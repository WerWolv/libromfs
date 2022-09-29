#include <romfs/romfs.hpp>

#include <map>

struct ROMFS_NAME {
    static std::map<std::filesystem::path, romfs::Resource> resources;
    static std::vector<std::filesystem::path> paths;
};

const romfs::Resource& romfs::ROMFS_CONCAT(get_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &path){
    return ROMFS_NAME::resources[path];
}

const std::vector<std::filesystem::path>& romfs::list() {
    return ROMFS_NAME::paths;
}