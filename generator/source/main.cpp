#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

    std::string replace(std::string string, const std::string &from, const std::string &to) {
        if(!from.empty())
            for(size_t pos = 0; (pos = string.find(from, pos)) != std::string::npos; pos += to.size())
                string.replace(pos, from.size(), to);
        return string;
    }

    std::string toPathString(std::string string) {
        // Replace all backslashes with forward slashes on Windows
        #if defined (OS_WINDOWS)
            string = replace(string, "\\", "/");
        #endif

        // Replace all " with \"
        string = replace(string, "\"", "\\\"");

        return string;
    }

}

int main() {
    std::ofstream outputFile("libromfs_resources.cpp");

    std::printf("libromfs: Resource Folder: %s\n", RESOURCE_LOCATION);

    outputFile << "#include <romfs/romfs.hpp>\n\n";
    outputFile << "#include <map>\n";
    outputFile << "#include <string>\n";
    outputFile << "#include <filesystem>\n";
    outputFile << "#include <vector>\n";

    outputFile <<
               #include "embedder.hpp"

    outputFile << "\n\n";
    outputFile << "/* Resource definitions */\n";

    std::vector<std::filesystem::path> paths;
    std::uint64_t identifierCount = 0;
    for (const auto &entry : fs::recursive_directory_iterator(RESOURCE_LOCATION)) {
        if (!entry.is_regular_file()) continue;

        auto path = fs::canonical(fs::absolute(entry.path()));
        auto relativePath = fs::relative(entry.path(), fs::absolute(RESOURCE_LOCATION));

        outputFile << "RESOURCE(" << "resource_" LIBROMFS_PROJECT_NAME "_" << identifierCount << ", \"" << toPathString(path.string()) << "\");\n";
        outputFile << "RESOURCE_EXPORT(" << "resource_" LIBROMFS_PROJECT_NAME "_" << identifierCount << ");\n";

        paths.push_back(relativePath);

        identifierCount++;
    }

    outputFile << "\n";

    outputFile << "struct RomFs_" LIBROMFS_PROJECT_NAME " {\n";
    outputFile << "    static std::map<std::filesystem::path, romfs::Resource> resources;\n";
    outputFile << "    static std::vector<std::filesystem::path> paths;\n";
    outputFile << "};\n\n";

    {
        outputFile << "/* Resource map */\n";
        outputFile << "std::map<std::filesystem::path, romfs::Resource> RomFs_" LIBROMFS_PROJECT_NAME "::resources = {\n";

        for (std::uint64_t i = 0; i < identifierCount; i++) {
            std::printf("libromfs: Bundling resource: %s\n", paths[i].string().c_str());

            outputFile << "    " << "{ \"" << paths[i].string() << "\", romfs::Resource({ resource_" LIBROMFS_PROJECT_NAME "_" << i << ", " << "size_t(resource_" LIBROMFS_PROJECT_NAME "_" << i << "_size) }) " << "},\n";
        }
        outputFile << "};";
    }

    outputFile << "\n\n";

    {
        outputFile << "/* Resource paths */\n";
        outputFile << "std::vector<std::filesystem::path> RomFs_" LIBROMFS_PROJECT_NAME "::paths = {\n";

        for (std::uint64_t i = 0; i < identifierCount; i++) {
            outputFile << "    \"" << toPathString(paths[i].string()) << "\",\n";
        }
        outputFile << "};";
    }


    outputFile << "\n\n";
}