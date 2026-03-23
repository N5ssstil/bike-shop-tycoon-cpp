#pragma once

#include "UIManager.h"
#include <functional>

namespace BikeShopTycoon {

/**
 * 底部导航栏
 */
class BottomNav : public UIPanel {
public:
    using NavCallback = std::function<void(const std::string&)>;
    
    BottomNav(TTF_Font* font);
    ~BottomNav() override;
    
    void SetOnNavClick(NavCallback callback) { onNavClick_ = callback; }
    
private:
    void CreateButtons();
    
    TTF_Font* font_;
    NavCallback onNavClick_;
    
    std::shared_ptr<UIButton> inventoryBtn_;
    std::shared_ptr<UIButton> repairBtn_;
    std::shared_ptr<UIButton> saveBtn_;
    std::shared_ptr<UIButton> pauseBtn_;
};

} // namespace BikeShopTycoon