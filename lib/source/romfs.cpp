#include <romfs/romfs.hpp>

#include <map>

struct RomFs {
    static std::map<std::string, romfs::Resource> resources;
    static std::vector<std::string> paths;
};

const romfs::Resource& romfs::get(const std::string &path){
    return RomFs::resources[path];
}

const std::vector<std::string>& romfs::list() {
    return RomFs::paths;
}