#include <romfs/romfs.hpp>

#include <map>

extern std::map<std::string, romfs::Resource> resources;

romfs::Resource romfs::get(const std::string &path){
    return resources[path];
}