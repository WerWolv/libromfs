#include <romfs/romfs.hpp>

#include <map>

extern std::map<std::string_view, romfs::Resource> resources;

romfs::Resource romfs::get(std::string_view path){
    return resources[path];
}