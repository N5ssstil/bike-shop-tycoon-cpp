#pragma once

#include "UIManager.h"
#include <functional>

namespace BikeShopTycoon {

/**
 * 主菜单
 */
class MainMenu : public UIPanel {
public:
    using MenuCallback = std::function<void(const std::string&)>;
    
    MainMenu(TTF_Font* font);
    ~MainMenu() override;
    
    void Render(SDL_Renderer* renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    
    void SetOnMenuAction(MenuCallback callback) { onMenuAction_ = callback; }
    
private:
    void CreateButtons();
    
    TTF_Font* font_;
    MenuCallback onMenuAction_;
    
    std::shared_ptr<UILabel> titleLabel_;
    std::shared_ptr<UILabel> subtitleLabel_;
    std::shared_ptr<UIButton> newGameBtn_;
    std::shared_ptr<UIButton> continueBtn_;
    std::shared_ptr<UIButton> quitBtn_;
};

} // namespace BikeShopTycoon