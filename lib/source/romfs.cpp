#include <romfs/romfs.hpp>

#include <map>

extern std::map<std::string_view, romfs::Resource> resources;
extern std::vector<std::string_view> paths;

const romfs::Resource& romfs::get(std::string_view path){
    return resources[path];
}

const std::vector<std::string_view>& romfs::list() {
    return paths;
}