#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>

std::string cachePath = "/Users/simon/dev/cli_cacher/cache/";

std::string executeCommand(const std::string &cmd, int &exitCode) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");

    if (!pipe) {
        exitCode = -1;
        return "popen() failed!";
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    exitCode = pclose(pipe);

    return result;
}

std::string generateCacheFilename(const std::string &cmd) {
    std::hash<std::string> hasher;
    size_t hashed = hasher(cmd);
    return "cache_" + std::to_string(hashed) + ".txt";
}

void saveToCache(const std::string &cmd, const std::string &output) {
    if (!std::filesystem::exists(cachePath)) {
        std::filesystem::create_directory(cachePath);
    }
    std::string filename = generateCacheFilename(cmd);
    std::string filepath = cachePath + filename;
    std::ofstream ofs(filepath);
    ofs << output;
    ofs.close();
}

std::string readFromCache(const std::string &cmd) {
    std::string filename = generateCacheFilename(cmd);
    std::ifstream ifs(cachePath + filename);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

bool cacheExists(const std::string &cmd) {
    std::string filename = generateCacheFilename(cmd);
    return std::filesystem::exists(cachePath + filename);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> [args...]" << std::endl;
        return 1;
    }

    std::string cmd;
    for (int i = 1; i < argc; ++i) {
        cmd += argv[i];
        if (i < argc - 1) {
            cmd += " ";
        }
    }

    int exitCode;
    std::string output = executeCommand(cmd, exitCode);

    if (exitCode == 0) {
        saveToCache(cmd, output);
        std::cout << output;
    } else {
        if (cacheExists(cmd)) {
            std::cerr << std::endl << "Command failed, returning cached result:" << std::endl << std::endl;
            std::cout << readFromCache(cmd);
        } else {
            std::cerr << "Command failed and no cached result available 😢" << std::endl;
        }
    }

    return exitCode;
}

