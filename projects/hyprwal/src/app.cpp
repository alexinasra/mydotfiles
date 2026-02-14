#include "app.hpp"

#include "fs.hpp"

#include <algorithm>
#include <cmath>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

// (removed unused image decode helpers)

#include <Magick++.h>
#include <Magick++/STL.h>
#include <png.h>

#include <hyprtoolkit/core/Backend.hpp>
#include <hyprtoolkit/core/Input.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/Image.hpp>
#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/element/ScrollArea.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprtoolkit/palette/Palette.hpp>
#include <hyprtoolkit/types/FontTypes.hpp>
#include <hyprtoolkit/types/ImageTypes.hpp>
#include <hyprtoolkit/types/SizeType.hpp>
#include <hyprtoolkit/window/Window.hpp>

namespace hyprwal {

using Hyprtoolkit::CColumnLayoutBuilder;
using Hyprtoolkit::CImageBuilder;
using Hyprtoolkit::CPalette;
using Hyprtoolkit::CRectangleBuilder;
using Hyprtoolkit::CRowLayoutBuilder;
using Hyprtoolkit::CScrollAreaBuilder;
using Hyprtoolkit::CTextBuilder;
using Hyprtoolkit::CWindowBuilder;
using Hyprtoolkit::CDynamicSize;
using Hyprtoolkit::IElement;
using Hyprtoolkit::IMAGE_FIT_MODE_CONTAIN;
using Hyprtoolkit::IMAGE_FIT_MODE_COVER;
using Hyprtoolkit::HT_FONT_ALIGN_CENTER;
using Hyprtoolkit::HT_WINDOW_TOPLEVEL;

namespace {
namespace fs = std::filesystem;

struct Rgba {
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;
};

bool writeBlackPng(const fs::path& path, int width, int height) {
    if (width <= 0 || height <= 0)
        return false;

    FILE* fp = std::fopen(path.c_str(), "wb");
    if (!fp)
        return false;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        std::fclose(fp);
        return false;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        std::fclose(fp);
        return false;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        std::fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);

    std::vector<unsigned char> buffer(static_cast<size_t>(width) * static_cast<size_t>(height) * 4, 0);
    for (size_t i = 0; i < buffer.size(); i += 4)
        buffer[i + 3] = 255;

    std::vector<png_bytep> rows(static_cast<size_t>(height));
    for (int y = 0; y < height; ++y)
        rows[static_cast<size_t>(y)] = buffer.data() + static_cast<size_t>(y) * static_cast<size_t>(width) * 4;

    png_write_image(png_ptr, rows.data());
    png_write_end(png_ptr, nullptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    std::fclose(fp);
    return true;
}

Rgba colorToRgba(const Magick::Color& color) {
    Rgba out;
    constexpr double q = 65535.0;
    auto clamp = [](int v) {
        if (v < 0)
            return 0;
        if (v > 255)
            return 255;
        return v;
    };
    out.r = clamp(static_cast<int>(color.quantumRed() * 255.0 / q + 0.5));
    out.g = clamp(static_cast<int>(color.quantumGreen() * 255.0 / q + 0.5));
    out.b = clamp(static_cast<int>(color.quantumBlue() * 255.0 / q + 0.5));
    out.a = clamp(static_cast<int>(255.0 - (color.quantumAlpha() * 255.0 / q + 0.5)));
    return out;
}

} // namespace

int App::run() {
    Magick::InitializeMagick(nullptr);
    backend_ = Hyprtoolkit::IBackend::create();
    if (!backend_)
        return 1;

    palette_ = backend_->getPalette();
    if (!palette_)
        palette_ = CPalette::palette();

    window_ = CWindowBuilder::begin()
                  ->type(HT_WINDOW_TOPLEVEL)
                  ->appTitle("hyprwal")
                  ->appClass("hyprwal")
                  ->preferredSize({900, 700})
                  ->minSize({640, 480})
                  ->commence();

    picturesDir_ = fsutil::picturesDir();
    images_ = fsutil::loadPictures();
    initialSelected_ = fsutil::hyprpaperWallpaper();
    if (initialSelected_) {
        const auto it = std::find(images_.begin(), images_.end(), *initialSelected_);
        if (it != images_.end()) {
            std::rotate(images_.begin(), it, it + 1);
        } else if (fs::exists(*initialSelected_)) {
            images_.insert(images_.begin(), *initialSelected_);
        } else {
            initialSelected_.reset();
        }
    }

    buildUi();

    closeListener_ = window_->m_events.closeRequest.listen([]() { std::exit(0); });
    resizeListener_ = window_->m_events.resized.listen([this](const Hyprutils::Math::Vector2D& size) { onResize(size); });

    window_->open();
    backend_->addIdle([this]() {
        if (!gridBuilt_)
            buildGrid();
    });
    backend_->enterLoop();

    return 0;
}

void App::buildUi() {
    std::cerr << "hyprwal: buildUi start\n";
    auto palette = palette_;
    background_ = CRectangleBuilder::begin()
                      ->color([palette]() {
                          if (palette)
                              return palette->m_colors.background;
                          return Hyprtoolkit::CHyprColor(0.08F, 0.08F, 0.08F, 1.F);
                      })
                      ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {1.F, 1.F}))
                      ->commence();
    background_->setPositionMode(IElement::HT_POSITION_ABSOLUTE);
    background_->setPositionFlag(IElement::HT_POSITION_FLAG_LEFT, true);
    background_->setPositionFlag(IElement::HT_POSITION_FLAG_TOP, true);
    background_->setAbsolutePosition({0, 0});
    background_->setGrow(true, true);

    mainColumn_ = CColumnLayoutBuilder::begin()
                      ->gap(16)
                      ->size(CDynamicSize(CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1.F, 0.F}))
                      ->commence();
    mainColumn_->setGrow(true, false);
    std::cerr << "hyprwal: mainColumn built\n";

    previewHolder_ = CColumnLayoutBuilder::begin()
                         ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {720.F, 320.F}))
                         ->commence();
    previewHolder_->setGrow(true, false);

    blackPngPath_ = "/tmp/hyprwal_black.png";
    if (!fs::exists(blackPngPath_))
        writeBlackPng(blackPngPath_, 64, 64);

    previewImage_ = CImageBuilder::begin()
                        ->fitMode(IMAGE_FIT_MODE_CONTAIN)
                        ->a(1.0F)
                        ->sync(true)
                        ->path(blackPngPath_.string())
                        ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {533.F, 300.F}))
                        ->commence();
    previewImage_->setGrow(false, false);

    dominantSwatch_ = CRectangleBuilder::begin()
                          ->color([]() { return Hyprtoolkit::CHyprColor(1.F, 1.F, 1.F, 1.F); })
                          ->borderColor([palette]() {
                              if (palette)
                                  return palette->m_colors.text;
                              return Hyprtoolkit::CHyprColor(1.F, 1.F, 1.F, 1.F);
                          })
                          ->borderThickness(1)
                          ->rounding(8)
                          ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {80, 80}))
                          ->commence();

    auto previewRow = CRowLayoutBuilder::begin()
                          ->gap(12)
                          ->size(CDynamicSize(CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}))
                          ->commence();
    previewRow->setGrow(true, true);
    previewRow->addChild(previewImage_);
    previewRow->addChild(dominantSwatch_);
    previewHolder_->addChild(previewRow);

    selectionLabel_ = CTextBuilder::begin()
                          ->text(std::string("Image Selection"))
                          ->align(HT_FONT_ALIGN_CENTER)
                          ->fontSize(Hyprtoolkit::CFontSize(Hyprtoolkit::CFontSize::HT_FONT_ABSOLUTE, 16.F))
                          ->color([palette]() {
                              if (palette)
                                  return palette->m_colors.text;
                              return Hyprtoolkit::CHyprColor(1.F, 1.F, 1.F, 1.F);
                          })
                          ->size(CDynamicSize(CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {0, 0}))
                          ->commence();

    gridColumn_ = CColumnLayoutBuilder::begin()
                      ->gap(8)
                      ->size(CDynamicSize(CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1.F, 0.F}))
                      ->commence();
    gridColumn_->setGrow(true, false);

    gridScroll_ = CScrollAreaBuilder::begin()
                      ->scrollY(true)
                      ->scrollX(false)
                      ->size(CDynamicSize(CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_ABSOLUTE, {1.F, 240.F}))
                      ->commence();
    gridScroll_->addChild(gridColumn_);

    if (images_.empty()) {
        auto emptyText = CTextBuilder::begin()
                             ->text("No images found in XDG Pictures")
                             ->align(HT_FONT_ALIGN_CENTER)
                             ->fontSize(Hyprtoolkit::CFontSize(Hyprtoolkit::CFontSize::HT_FONT_ABSOLUTE, 14.F))
                             ->color([]() { return Hyprtoolkit::CHyprColor(1.F, 1.F, 1.F, 1.F); })
                             ->size(CDynamicSize(CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {0, 0}))
                             ->commence();
        mainColumn_->addChild(emptyText);
        std::cerr << "hyprwal: empty state\n";
    } else {
        mainColumn_->addChild(previewHolder_);
        mainColumn_->addChild(selectionLabel_);
        mainColumn_->addChild(gridScroll_);
        std::cerr << "hyprwal: content children added\n";
    }

    mainScroll_ = CScrollAreaBuilder::begin()
                      ->scrollY(true)
                      ->scrollX(false)
                      ->size(CDynamicSize(CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}))
                      ->commence();
    mainScroll_->addChild(mainColumn_);

    window_->m_rootElement->addChild(background_);
    window_->m_rootElement->addChild(mainScroll_);
    std::cerr << "hyprwal: buildUi end\n";
}

void App::updatePreview(const std::filesystem::path& path) {
    if (!previewImage_)
        return;

    std::cerr << "hyprwal: preview set " << path << "\n";
    selectedPath_ = path;

    previewImage_->rebuild()->path(path.string())->commence();
    if (previewHolder_)
        previewHolder_->forceReposition();

    updateSelectionHighlight();
    updateDominantColor(path);
}

void App::updateDominantColor(const std::filesystem::path& path) {
    if (!dominantSwatch_)
        return;
    try {
        Magick::Image image(path.string());
        image.resize(Magick::Geometry(64, 64));
        image.quantizeColors(8);
        image.quantize();

        std::vector<std::pair<Magick::Color, size_t>> colors;
        Magick::colorHistogram(&colors, image);
        if (!colors.empty()) {
            auto it = std::max_element(colors.begin(), colors.end(),
                                       [](const auto& a, const auto& b) { return a.second < b.second; });
            const auto c = colorToRgba(it->first);
            dominantSwatch_->rebuild()
                ->color([c]() {
                    return Hyprtoolkit::CHyprColor(c.r / 255.F, c.g / 255.F, c.b / 255.F, 1.F);
                })
                ->commence();
            dominantSwatch_->forceReposition();
        }
    } catch (const Magick::Exception& ex) {
        std::cerr << "hyprwal: dominant color error: " << ex.what() << "\n";
    }
}

void App::updateSelectionHighlight() {
    const std::string selected = selectedPath_ ? selectedPath_->string() : "";
    for (auto& [path, box] : thumbBoxes_) {
        if (!box)
            continue;
        const bool isSelected = !selected.empty() && path == selected;
        box->rebuild()
            ->borderColor([this]() {
                if (palette_) {
                    auto c = palette_->m_colors.accent;
                    c.a = 1.F;
                    return c;
                }
                return Hyprtoolkit::CHyprColor(1.F, 1.F, 1.F, 1.F);
            })
            ->borderThickness(isSelected ? 6 : 2)
            ->commence();
    }
    if (gridColumn_)
        gridColumn_->forceReposition();
}

void App::buildGrid() {
    if (gridBuilt_ || !gridColumn_)
        return;
    gridBuilt_ = true;
    std::cerr << "hyprwal: buildGrid start\n";

    gridColumn_->clearChildren();
    thumbBoxes_.clear();
    thumbIndex_ = 0;
    currentRow_.reset();

    if (!images_.empty()) {
        auto row = CRowLayoutBuilder::begin()
                       ->gap(8)
                       ->size(CDynamicSize(CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {0, 0}))
                       ->commence();
        auto firstThumbImage = CImageBuilder::begin()
                                   ->path(blackPngPath_.string())
                                   ->fitMode(IMAGE_FIT_MODE_COVER)
                                   ->a(1.0F)
                                   ->sync(true)
                                   ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE,
                                                       {std::max(40.F, thumbSize_ - 8.F), std::max(40.F, thumbSize_ - 8.F)}))
                                   ->commence();
        firstThumbImage->setPositionMode(IElement::HT_POSITION_ABSOLUTE);
        firstThumbImage->setPositionFlag(IElement::HT_POSITION_FLAG_CENTER, true);
        firstThumbImage->setAbsolutePosition({0, 0});
        const bool isSelected = selectedPath_.has_value() && *selectedPath_ == images_.front();
        auto firstThumb = CRectangleBuilder::begin()
                              ->color([]() { return Hyprtoolkit::CHyprColor(0.F, 0.F, 0.F, 0.F); })
                              ->borderColor([this]() {
                                  if (palette_)
                                      return palette_->m_colors.accent;
                                  return Hyprtoolkit::CHyprColor(1.F, 1.F, 1.F, 1.F);
                              })
                              ->borderThickness(isSelected ? 3 : 0)
                              ->rounding(6)
                              ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE,
                                                  {thumbSize_, thumbSize_}))
                              ->commence();
        firstThumb->addChild(firstThumbImage);
        const auto firstPath = images_.front();
        thumbBoxes_[firstPath.string()] = firstThumb;
        firstThumb->setReceivesMouse(true);
        firstThumb->setMouseButton([this, firstPath](Hyprtoolkit::Input::eMouseButton button, bool pressed) {
            if (button == Hyprtoolkit::Input::MOUSE_BUTTON_LEFT && pressed)
                updatePreview(firstPath);
        });
        row->addChild(firstThumb);
        gridColumn_->addChild(row);
        currentRow_ = row;
        firstThumb_ = firstThumbImage;
        thumbIndex_ = 1;
    }

    if (!images_.empty() && background_) {
        auto warmup = CImageBuilder::begin()
                          ->path(images_.front().string())
                          ->fitMode(IMAGE_FIT_MODE_COVER)
                          ->a(0.0F)
                          ->sync(true)
                          ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {1, 1}))
                          ->commence();
        background_->addChild(warmup);
        backend_->addTimer(
            std::chrono::milliseconds(200),
            [this, warmup](Hyprutils::Memory::CAtomicSharedPointer<Hyprtoolkit::CTimer> t, void*) {
                if (background_)
                    background_->removeChild(warmup);
                scheduleNextThumbBatch();
                t.reset();
            },
            nullptr);
    } else {
        scheduleNextThumbBatch();
    }
}

void App::scheduleNextThumbBatch() {
    backend_->addTimer(
        std::chrono::milliseconds(10),
        [this](Hyprutils::Memory::CAtomicSharedPointer<Hyprtoolkit::CTimer> timer, void*) {
            constexpr size_t kBatch = 6;
            size_t added = 0;
            while (thumbIndex_ < images_.size() && added < kBatch) {
                if (!currentRow_ || thumbIndex_ % gridColumns_ == 0) {
                    currentRow_ = CRowLayoutBuilder::begin()
                                      ->gap(8)
                                      ->size(CDynamicSize(CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {0, 0}))
                                      ->commence();
                    gridColumn_->addChild(currentRow_);
                }
                const auto& path = images_[thumbIndex_];
                auto thumbImage = CImageBuilder::begin()
                                      ->path(path.string())
                                      ->fitMode(IMAGE_FIT_MODE_COVER)
                                      ->a(1.0F)
                                      ->sync(true)
                                      ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE,
                                                          {std::max(40.F, thumbSize_ - 8.F), std::max(40.F, thumbSize_ - 8.F)}))
                                      ->commence();
                thumbImage->setPositionMode(IElement::HT_POSITION_ABSOLUTE);
                thumbImage->setPositionFlag(IElement::HT_POSITION_FLAG_CENTER, true);
                thumbImage->setAbsolutePosition({0, 0});
                const bool isSelected = selectedPath_.has_value() && *selectedPath_ == path;
                auto thumb = CRectangleBuilder::begin()
                                 ->color([]() { return Hyprtoolkit::CHyprColor(0.F, 0.F, 0.F, 0.F); })
                                 ->borderColor([this]() {
                                     if (palette_)
                                         return palette_->m_colors.accent;
                                     return Hyprtoolkit::CHyprColor(1.F, 1.F, 1.F, 1.F);
                                 })
                                 ->borderThickness(isSelected ? 3 : 0)
                                 ->rounding(6)
                                 ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE,
                                                     {thumbSize_, thumbSize_}))
                                 ->commence();
                thumb->addChild(thumbImage);
                thumbBoxes_[path.string()] = thumb;

                thumb->setReceivesMouse(true);
                thumb->setMouseButton([this, path](Hyprtoolkit::Input::eMouseButton button, bool pressed) {
                    if (button == Hyprtoolkit::Input::MOUSE_BUTTON_LEFT && pressed)
                        updatePreview(path);
                });

                if (currentRow_)
                    currentRow_->addChild(thumb);
                if (thumbIndex_ == 0)
                    firstThumb_ = thumbImage;

                ++thumbIndex_;
                ++added;
            }

            gridColumn_->forceReposition();
            updateSelectionHighlight();

            if (thumbIndex_ < images_.size()) {
                scheduleNextThumbBatch();
            } else {
                std::cerr << "hyprwal: buildGrid done\n";
                if (!images_.empty()) {
                    backend_->addTimer(
                        std::chrono::milliseconds(150),
                        [this](Hyprutils::Memory::CAtomicSharedPointer<Hyprtoolkit::CTimer> t, void*) {
                            if (firstThumb_)
                                firstThumb_->rebuild()->path(images_.front().string())->commence();
                            updatePreview(images_.front());
                            t.reset();
                        },
                        nullptr);
                }
            }
            timer.reset();
        },
        nullptr);
}

void App::onResize(const Hyprutils::Math::Vector2D& size) {
    float w = size.x;
    float h = size.y;
    if (w > 10000 || h > 10000) {
        w *= 0.01F;
        h *= 0.01F;
    }
    if (w < 50 || h < 50)
        return;

    if (background_) {
        background_->rebuild()
            ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {w, h}))
            ->commence();
        background_->forceReposition();
    }
    const float contentW = std::max(320.F, w - 40.F);
    if (mainScroll_)
        mainScroll_->forceReposition();
    if (mainColumn_) {
        mainColumn_->rebuild()
            ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_AUTO, {w, 0.F}))
            ->commence();
        mainColumn_->forceReposition();
    }
    const float previewH = std::max(220.F, h - 320.F);
    if (previewHolder_) {
        previewHolder_->rebuild()
            ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {contentW, previewH}))
            ->commence();
        previewHolder_->forceReposition();
    }
    if (previewImage_) {
        const float maxW = std::max(320.F, contentW - 40.F);
        const float maxH = std::max(180.F, previewH);
        float imageW = maxW;
        float imageH = imageW * 9.F / 16.F;
        if (imageH > maxH) {
            imageH = maxH;
            imageW = imageH * 16.F / 9.F;
        }
        previewImage_->rebuild()
            ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_ABSOLUTE, {imageW, imageH}))
            ->commence();
        previewImage_->forceReposition();
    }
    if (gridScroll_)
        gridScroll_->forceReposition();
    if (gridColumn_) {
        gridColumn_->rebuild()
            ->size(CDynamicSize(CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_AUTO, {1.F, 0.F}))
            ->commence();
        gridColumn_->forceReposition();
    }

    const float gap = 8.F;
    const float minThumb = 110.F;
    const float maxThumb = 200.F;
    size_t cols = static_cast<size_t>((contentW + gap) / (minThumb + gap));
    cols = std::clamp(cols, static_cast<size_t>(2), static_cast<size_t>(6));
    float newThumb = (contentW - gap * (cols - 1)) / static_cast<float>(cols);
    newThumb = std::clamp(newThumb, minThumb, maxThumb);

    if (cols != gridColumns_ || std::abs(newThumb - thumbSize_) > 1.0F) {
        gridColumns_ = cols;
        thumbSize_ = newThumb;
        gridBuilt_ = false;
        buildGrid();
    }
    if (gridColumn_) {
        gridColumn_->rebuild()
            ->size(CDynamicSize(CDynamicSize::HT_SIZE_ABSOLUTE, CDynamicSize::HT_SIZE_AUTO, {contentW, 0.F}))
            ->commence();
        gridColumn_->forceReposition();
    }
    if (previewHolder_)
        previewHolder_->forceReposition();
    if (gridColumn_)
        gridColumn_->forceReposition();
    if (gridScroll_)
        gridScroll_->forceReposition();
    std::cerr << "hyprwal: onResize " << w << "x" << h << "\n";
}

} // namespace hyprwal
