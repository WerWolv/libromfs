#include <romfs/romfs.hpp>

#include <map>

const std::map<std::filesystem::path, romfs::Resource>& ROMFS_CONCAT(ROMFS_NAME, _get_resources)();
const std::vector<std::filesystem::path>& ROMFS_CONCAT(ROMFS_NAME, _get_paths)();
const std::string& ROMFS_CONCAT(ROMFS_NAME, _get_name)();

namespace romfs {

    const romfs::Resource &impl::ROMFS_CONCAT(get_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &path) {
        try {
            return ROMFS_CONCAT(ROMFS_NAME, _get_resources)().at(path);
        } catch (std::out_of_range &ignored) {
            throw std::invalid_argument(std::string("Invalid romfs resource path for '" + romfs::name() + "' : ") + path.string());
        }
    }

    std::vector<std::filesystem::path> impl::ROMFS_CONCAT(list_, LIBROMFS_PROJECT_NAME)(const std::filesystem::path &parent) {
        if (parent.empty()) {
            return ROMFS_CONCAT(ROMFS_NAME, _get_paths)();
        } else {
            std::vector<std::filesystem::path> result;
            for (const auto &p : ROMFS_CONCAT(ROMFS_NAME, _get_paths)())
                if (p.parent_path() == parent)
                    result.push_back(p);

            return result;
        }
    }

    const std::string &impl::ROMFS_CONCAT(name_, LIBROMFS_PROJECT_NAME)() {
        return ROMFS_CONCAT(ROMFS_NAME, _get_name)();
    }

}