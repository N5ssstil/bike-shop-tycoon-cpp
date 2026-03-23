#include "UIManager.h"
#include <algorithm>

namespace BikeShopTycoon {

// UIElement 实现
UIElement::UIElement(const std::string& name) : name_(name) {}

bool UIElement::HandleEvent(const SDL_Event& event) {
    return false;
}

void UIElement::SetPosition(int x, int y) {
    rect_.x = x;
    rect_.y = y;
}

void UIElement::SetSize(int w, int h) {
    rect_.w = w;
    rect_.h = h;
}

void UIElement::SetVisible(bool visible) {
    visible_ = visible;
}

bool UIElement::ContainsPoint(int x, int y) const {
    return x >= rect_.x && x <= rect_.x + rect_.w &&
           y >= rect_.y && y <= rect_.y + rect_.h;
}

// UILabel 实现
UILabel::UILabel(const std::string& name, TTF_Font* font)
    : UIElement(name), font_(font) {}

UILabel::~UILabel() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
    }
}

void UILabel::Render(SDL_Renderer* renderer) {
    if (!visible_) return;
    
    // 如果纹理需要更新，先更新
    if (!texture_ && !text_.empty() && font_) {
        UpdateTexture(renderer);
    }
    
    if (!texture_) return;
    
    SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
}

void UILabel::SetText(const std::string& text) {
    if (text_ != text) {
        text_ = text;
        texture_ = nullptr; // 延迟更新
    }
}

void UILabel::SetColor(const SDL_Color& color) {
    color_ = color;
    texture_ = nullptr;
}

void UILabel::SetAlignment(int alignment) {
    alignment_ = alignment;
}

void UILabel::UpdateTexture(SDL_Renderer* renderer) {
    if (texture_) {
        SDL_DestroyTexture(texture_);
    }
    
    if (text_.empty() || !font_) {
        texture_ = nullptr;
        return;
    }
    
    // 使用 UTF8 版本渲染中文
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, text_.c_str(), color_);
    if (surface) {
        texture_ = SDL_CreateTextureFromSurface(renderer, surface);
        rect_.w = surface->w;
        rect_.h = surface->h;
        SDL_FreeSurface(surface);
    }
}

// UIButton 实现
UIButton::UIButton(const std::string& name, TTF_Font* font)
    : UIElement(name), font_(font) {}

UIButton::~UIButton() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
    }
}

void UIButton::Render(SDL_Renderer* renderer) {
    if (!visible_) return;
    
    // 选择颜色
    SDL_Color color = normalColor_;
    if (isPressed_) {
        color = pressedColor_;
    } else if (isHovered_) {
        color = hoverColor_;
    }
    
    // 绘制背景
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect_);
    
    // 绘制边框
    SDL_SetRenderDrawColor(renderer, 100, 100, 110, 255);
    SDL_RenderDrawRect(renderer, &rect_);
    
    // 更新并绘制文本
    if (!texture_ && !text_.empty() && font_) {
        UpdateTexture(renderer);
    }
    
    if (texture_) {
        SDL_Rect textRect = rect_;
        int texW, texH;
        SDL_QueryTexture(texture_, nullptr, nullptr, &texW, &texH);
        textRect.x = rect_.x + (rect_.w - texW) / 2;
        textRect.y = rect_.y + (rect_.h - texH) / 2;
        textRect.w = texW;
        textRect.h = texH;
        SDL_RenderCopy(renderer, texture_, nullptr, &textRect);
    }
}

bool UIButton::HandleEvent(const SDL_Event& event) {
    if (!visible_) return false;
    
    switch (event.type) {
        case SDL_MOUSEMOTION: {
            bool wasHovered = isHovered_;
            isHovered_ = ContainsPoint(event.motion.x, event.motion.y);
            return isHovered_ != wasHovered;
        }
        
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT && isHovered_) {
                isPressed_ = true;
                return true;
            }
            break;
            
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT && isPressed_) {
                isPressed_ = false;
                if (isHovered_ && onClick_) {
                    onClick_();
                }
                return true;
            }
            break;
    }
    
    return false;
}

void UIButton::SetText(const std::string& text) {
    text_ = text;
    texture_ = nullptr;
}

void UIButton::SetColors(const SDL_Color& normal, const SDL_Color& hover, const SDL_Color& pressed) {
    normalColor_ = normal;
    hoverColor_ = hover;
    pressedColor_ = pressed;
}

void UIButton::SetOnClick(ClickCallback callback) {
    onClick_ = callback;
}

void UIButton::UpdateTexture(SDL_Renderer* renderer) {
    if (texture_) {
        SDL_DestroyTexture(texture_);
    }
    
    SDL_Color white = {255, 255, 255, 255};
    // 使用 UTF8 版本渲染中文
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, text_.c_str(), white);
    if (surface) {
        texture_ = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

// UIPanel 实现
UIPanel::UIPanel(const std::string& name) : UIElement(name) {}

UIPanel::~UIPanel() = default;

void UIPanel::Render(SDL_Renderer* renderer) {
    if (!visible_) return;
    
    // 绘制背景
    SDL_SetRenderDrawColor(renderer, bgColor_.r, bgColor_.g, bgColor_.b, bgColor_.a);
    SDL_RenderFillRect(renderer, &rect_);
    
    // 绘制子元素
    for (auto& child : children_) {
        if (child->IsVisible()) {
            child->Render(renderer);
        }
    }
}

bool UIPanel::HandleEvent(const SDL_Event& event) {
    if (!visible_) return false;
    
    for (auto& child : children_) {
        if (child->HandleEvent(event)) {
            return true;
        }
    }
    return false;
}

void UIPanel::AddChild(std::shared_ptr<UIElement> child) {
    children_.push_back(child);
}

void UIPanel::RemoveChild(const std::string& name) {
    children_.erase(
        std::remove_if(children_.begin(), children_.end(),
            [&](const std::shared_ptr<UIElement>& e) {
                // 需要dynamic_cast来访问name，这里简化处理
                return false;
            }),
        children_.end()
    );
}

void UIPanel::SetBackgroundColor(const SDL_Color& color) {
    bgColor_ = color;
}

// UIManager 实现
UIManager& UIManager::Instance() {
    static UIManager instance;
    return instance;
}

bool UIManager::Init(SDL_Renderer* renderer, TTF_Font* font) {
    renderer_ = renderer;
    font_ = font;
    return true;
}

void UIManager::Shutdown() {
    panels_.clear();
}

void UIManager::Render() {
    for (auto& panel : panels_) {
        if (panel->IsVisible()) {
            panel->Render(renderer_);
        }
    }
}

void UIManager::HandleEvent(const SDL_Event& event) {
    for (auto& panel : panels_) {
        if (panel->IsVisible()) {
            if (panel->HandleEvent(event)) {
                break;
            }
        }
    }
}

void UIManager::AddPanel(std::shared_ptr<UIPanel> panel) {
    panels_.push_back(panel);
}

void UIManager::RemovePanel(const std::string& name) {
    panels_.erase(
        std::remove_if(panels_.begin(), panels_.end(),
            [&](const std::shared_ptr<UIPanel>& p) { return false; }),
        panels_.end()
    );
}

UIPanel* UIManager::GetPanel(const std::string& name) {
    for (auto& panel : panels_) {
        // 简化实现
    }
    return nullptr;
}

void UIManager::ShowPanel(const std::string& name) {
    // 简化实现
}

void UIManager::HidePanel(const std::string& name) {
    // 简化实现
}

} // namespace BikeShopTycoon