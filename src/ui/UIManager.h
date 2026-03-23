#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace BikeShopTycoon {

/**
 * UI 元素基类
 */
class UIElement {
public:
    UIElement(const std::string& name);
    virtual ~UIElement() = default;
    
    virtual void Render(SDL_Renderer* renderer) = 0;
    virtual bool HandleEvent(const SDL_Event& event);
    
    void SetPosition(int x, int y);
    void SetSize(int w, int h);
    void SetVisible(bool visible);
    
    bool IsVisible() const { return visible_; }
    bool ContainsPoint(int x, int y) const;
    
protected:
    std::string name_;
    SDL_Rect rect_ = {0, 0, 0, 0};
    bool visible_ = true;
};

/**
 * 文本标签
 */
class UILabel : public UIElement {
public:
    UILabel(const std::string& name, TTF_Font* font);
    ~UILabel() override;
    
    void Render(SDL_Renderer* renderer) override;
    
    void SetText(const std::string& text);
    void SetColor(const SDL_Color& color);
    void SetAlignment(int alignment); // 0=left, 1=center, 2=right
    
private:
    void UpdateTexture(SDL_Renderer* renderer);
    
    TTF_Font* font_;
    std::string text_;
    SDL_Color color_ = {255, 255, 255, 255};
    SDL_Texture* texture_ = nullptr;
    int alignment_ = 0;
};

/**
 * 按钮
 */
class UIButton : public UIElement {
public:
    using ClickCallback = std::function<void()>;
    
    UIButton(const std::string& name, TTF_Font* font);
    ~UIButton() override;
    
    void Render(SDL_Renderer* renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    
    void SetText(const std::string& text);
    void SetColors(const SDL_Color& normal, const SDL_Color& hover, const SDL_Color& pressed);
    void SetOnClick(ClickCallback callback);
    
private:
    void UpdateTexture(SDL_Renderer* renderer);
    
    TTF_Font* font_;
    std::string text_;
    SDL_Texture* texture_ = nullptr;
    
    SDL_Color normalColor_ = {50, 100, 150, 255};
    SDL_Color hoverColor_ = {70, 120, 170, 255};
    SDL_Color pressedColor_ = {30, 80, 130, 255};
    
    bool isHovered_ = false;
    bool isPressed_ = false;
    ClickCallback onClick_;
};

/**
 * 面板容器
 */
class UIPanel : public UIElement {
public:
    UIPanel(const std::string& name);
    ~UIPanel() override;
    
    void Render(SDL_Renderer* renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    
    void AddChild(std::shared_ptr<UIElement> child);
    void RemoveChild(const std::string& name);
    void SetBackgroundColor(const SDL_Color& color);
    
protected:
    std::vector<std::shared_ptr<UIElement>> children_;
    SDL_Color bgColor_ = {30, 30, 35, 240};
};

/**
 * UI 管理器
 */
class UIManager {
public:
    static UIManager& Instance();
    
    bool Init(SDL_Renderer* renderer, TTF_Font* font);
    void Shutdown();
    
    void Render();
    void HandleEvent(const SDL_Event& event);
    
    // 面板管理
    void AddPanel(std::shared_ptr<UIPanel> panel);
    void RemovePanel(const std::string& name);
    UIPanel* GetPanel(const std::string& name);
    void ShowPanel(const std::string& name);
    void HidePanel(const std::string& name);
    
    // 工具方法
    SDL_Renderer* GetRenderer() const { return renderer_; }
    TTF_Font* GetFont() const { return font_; }
    
private:
    UIManager() = default;
    
    SDL_Renderer* renderer_ = nullptr;
    TTF_Font* font_ = nullptr;
    std::vector<std::shared_ptr<UIPanel>> panels_;
};

} // namespace BikeShopTycoon