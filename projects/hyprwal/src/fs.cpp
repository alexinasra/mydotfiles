#include "fs.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <string>

namespace fs = std::filesystem;

namespace {
std::string expandHome(const std::string& path, const std::string& home) {
    if (path.rfind("$HOME", 0) == 0)
        return home + path.substr(5);
    if (!path.empty() && path[0] == '~')
        return home + path.substr(1);
    return path;
}

bool isPictureExt(const fs::path& path) {
    if (!path.has_extension())
        return false;

    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });

    return ext == ".jpg" || ext == ".jpeg" || ext == ".jxl";
}
} // namespace

namespace hyprwal::fsutil {

fs::path picturesDir() {
    const char* homeEnv = std::getenv("HOME");
    const std::string home = homeEnv ? homeEnv : "";
    if (home.empty())
        return {};

    const fs::path configPath = fs::path(home) / ".config" / "user-dirs.dirs";
    if (fs::exists(configPath)) {
        std::ifstream in(configPath);
        std::string line;
        std::regex re(R"(^XDG_PICTURES_DIR=(.*)$)");
        std::smatch match;
        while (std::getline(in, line)) {
            if (std::regex_match(line, match, re)) {
                std::string value = match[1].str();
                value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
                value = expandHome(value, home);
                return fs::path(value);
            }
        }
    }

    return fs::path(home) / "Pictures";
}

std::vector<fs::path> loadPictures() {
    std::vector<fs::path> images;
    const fs::path pictures = picturesDir();
    if (pictures.empty() || !fs::exists(pictures) || !fs::is_directory(pictures))
        return images;

    for (const auto& entry : fs::directory_iterator(pictures)) {
        if (!entry.is_regular_file())
            continue;

        const auto path = entry.path();
        if (isPictureExt(path))
            images.push_back(path);
    }

    std::sort(images.begin(), images.end());
    return images;
}

std::optional<fs::path> hyprpaperWallpaper() {
    const char* homeEnv = std::getenv("HOME");
    const std::string home = homeEnv ? homeEnv : "";
    if (home.empty())
        return std::nullopt;

    const fs::path configPath = fs::path(home) / ".config" / "hypr" / "hyprpaper.conf";
    if (!fs::exists(configPath))
        return std::nullopt;

    std::ifstream in(configPath);
    std::string line;
    std::regex re(R"(^\s*wallpaper\s*=\s*([^,]+),(.*)$)");
    std::smatch match;
    while (std::getline(in, line)) {
        if (std::regex_match(line, match, re)) {
            std::string value = match[2].str();
            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            if (value.empty())
                continue;
            value = expandHome(value, home);
            return fs::path(value);
        }
    }

    return std::nullopt;
}

} // namespace hyprwal::fsutil
