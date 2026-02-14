#pragma once

#include <filesystem>
#include <optional>
#include <vector>

namespace hyprwal::fsutil {
std::filesystem::path picturesDir();
std::vector<std::filesystem::path> loadPictures();
std::optional<std::filesystem::path> hyprpaperWallpaper();
}
