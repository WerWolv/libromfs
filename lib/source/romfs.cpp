#include <romfs/romfs.hpp>

#include <map>

struct RomFs {
    static std::map<std::filesystem::path, romfs::Resource> resources;
    static std::vector<std::filesystem::path> paths;
};

const romfs::Resource& romfs::get(const std::filesystem::path &path){
    return RomFs::resources[path];
}

const std::vector<std::filesystem::path>& romfs::list() {
    return RomFs::paths;
}