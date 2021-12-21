#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

int main() {
    std::ofstream outputFile("libromfs_resources.cpp");

    std::printf("libromfs: Resource Folder: %s\n", RESOURCE_LOCATION);

    outputFile << "#include <romfs/romfs.hpp>\n";
    outputFile << "#include <romfs/embedder.hpp>\n";
    outputFile << "#include <string>\n";
    outputFile << "#include <map>\n\n";

    outputFile << "/* Resource definitions */\n";

    std::vector<std::string> paths;
    std::uint64_t identifierCount = 0;
    for (const auto &entry : fs::recursive_directory_iterator(RESOURCE_LOCATION)) {
        auto relativePath = fs::relative(entry.path()).generic_string();

        outputFile << "RESOURCE(" << "resource" << identifierCount << ", \"" << relativePath << "\");\n";
        outputFile << "RESOURCE_EXPORT(" << "resource" << identifierCount << ");\n";

        while (std::string_view(relativePath).starts_with("../")) {
            relativePath = relativePath.substr(3);
        }

        if (std::string_view(relativePath).starts_with("romfs/"))
            relativePath = relativePath.substr(6);

        paths.push_back(relativePath);

        identifierCount++;
    }

    outputFile << "\n";
    outputFile << "/* Resource map */";
    outputFile << "std::map<std::string, romfs::Resource> resources = {\n";

    for (std::uint64_t i = 0; i < identifierCount; i++) {
        std::printf("libromfs: Bundling resource: %s\n", paths[i].c_str());

        outputFile << "    " << "{ \"" << paths[i] << "\", { { resource" << i << ", " << "size_t(resource" << i << "_size) } } " << "},\n";
    }

    outputFile << "};";

    outputFile << "\n\n";
}