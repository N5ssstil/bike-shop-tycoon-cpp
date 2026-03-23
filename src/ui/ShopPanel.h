#pragma once

#include "UIManager.h"
#include "systems/CustomerManager.h"
#include "systems/InventoryManager.h"

namespace BikeShopTycoon {

/**
 * 店铺面板
 */
class ShopPanel : public UIPanel {
public:
    ShopPanel(TTF_Font* font, CustomerManager& customerManager, InventoryManager& inventoryManager);
    ~ShopPanel() override;
    
    void Render(SDL_Renderer* renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    
    void Update();
    
private:
    void CreateUI();
    void UpdateCustomerList();
    
    TTF_Font* font_;
    CustomerManager& customerManager_;
    InventoryManager& inventoryManager_;
    
    std::shared_ptr<UILabel> titleLabel_;
    std::shared_ptr<UILabel> statusLabel_;
    std::vector<std::shared_ptr<UIButton>> customerButtons_;
};

} // namespace BikeShopTycoon