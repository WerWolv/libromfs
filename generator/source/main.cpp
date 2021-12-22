#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main() {
    std::ofstream outputFile("libromfs_resources.cpp");

    std::printf("libromfs: Resource Folder: %s\n", RESOURCE_LOCATION);

    outputFile << "#include <romfs/romfs.hpp>\n\n";
    outputFile << "#include <map>\n";
    outputFile << "#include <string_view>\n";
    outputFile << "#include <vector>\n\n";

    outputFile <<
               #include "embedder.hpp"

    outputFile << "\n\n";
    outputFile << "/* Resource definitions */\n";

    std::vector<std::string> paths;
    std::uint64_t identifierCount = 0;
    for (const auto &entry : fs::recursive_directory_iterator(RESOURCE_LOCATION)) {
        if (!entry.is_regular_file()) continue;

        auto path = fs::absolute(entry.path()).generic_string();
        auto relativePath = fs::relative(entry.path(), fs::absolute(RESOURCE_LOCATION)).generic_string();

        outputFile << "RESOURCE(" << "resource" << identifierCount << ", \"" << path << "\");\n";
        outputFile << "RESOURCE_EXPORT(" << "resource" << identifierCount << ");\n";

        paths.push_back(relativePath);

        identifierCount++;
    }

    outputFile << "\n";

    {
        outputFile << "/* Resource map */";
        outputFile << "std::map<std::string_view, romfs::Resource> resources = {\n";

        for (std::uint64_t i = 0; i < identifierCount; i++) {
            std::printf("libromfs: Bundling resource: %s\n", paths[i].c_str());

            outputFile << "    " << "{ \"" << paths[i] << "\", romfs::Resource({ resource" << i << ", " << "size_t(resource" << i << "_size) }) " << "},\n";
        }
        outputFile << "};";
    }

    outputFile << "\n";

    {
        outputFile << "/* Resource paths */";
        outputFile << "std::vector<std::string_view> paths = {\n";

        for (std::uint64_t i = 0; i < identifierCount; i++) {
            outputFile << "    \"" << paths[i] << "\",\n";
        }
        outputFile << "};";
    }


    outputFile << "\n\n";
}