#pragma once

#include <filesystem>
#include <optional>
#include <unordered_map>
#include <vector>

#include <hyprutils/memory/SharedPtr.hpp>
#include <hyprutils/signal/Listener.hpp>
#include <hyprutils/math/Vector2D.hpp>

namespace Hyprtoolkit {
class IBackend;
class IWindow;
class IElement;
class CImageElement;
class CTextElement;
class CRectangleElement;
class CColumnLayoutElement;
class CScrollAreaElement;
class CPalette;
}

namespace hyprwal {

class App {
  public:
    int run();

  private:
    void buildUi();
    void buildGrid();
    void updatePreview(const std::filesystem::path& path);
    void updateDominantColor(const std::filesystem::path& path);
    void updateSelectionHighlight();
    void onResize(const Hyprutils::Math::Vector2D& size);

    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IBackend> backend_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IWindow> window_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CPalette> palette_;

    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CRectangleElement> background_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CScrollAreaElement> mainScroll_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CColumnLayoutElement> mainColumn_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CColumnLayoutElement> previewHolder_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CImageElement> previewImage_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CRectangleElement> dominantSwatch_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CColumnLayoutElement> gridColumn_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CScrollAreaElement> gridScroll_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CTextElement> selectedLabel_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CTextElement> selectionLabel_;
    std::filesystem::path blackPngPath_;

    std::filesystem::path picturesDir_;
    std::vector<std::filesystem::path> images_;
    std::optional<std::filesystem::path> initialSelected_;
    std::optional<std::filesystem::path> selectedPath_;
    std::unordered_map<std::string, Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CRectangleElement>> thumbBoxes_;
    bool gridBuilt_ = false;
    size_t gridColumns_ = 4;
    float thumbSize_ = 120.F;
    size_t thumbIndex_ = 0;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::IElement> currentRow_;
    Hyprutils::Memory::CSharedPointer<Hyprtoolkit::CImageElement> firstThumb_;
    void scheduleNextThumbBatch();

    Hyprutils::Signal::CHyprSignalListener resizeListener_;
    Hyprutils::Signal::CHyprSignalListener closeListener_;
};

} // namespace hyprwal
