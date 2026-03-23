#pragma once

#include "UIManager.h"
#include "systems/InventoryManager.h"
#include "data/ProductDatabase.h"
#include <vector>

namespace BikeShopTycoon {

/**
 * 库存面板 - 显示库存、进货
 */
class InventoryPanel : public UIPanel {
public:
    InventoryPanel(TTF_Font* font, InventoryManager& inventoryManager);
    ~InventoryPanel() override;
    
    void Render(SDL_Renderer* renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    
    void RefreshInventory();
    void ShowPurchaseMode(bool show);
    
private:
    void CreateUI();
    void RenderInventoryList(SDL_Renderer* renderer, int startY);
    void RenderPurchaseList(SDL_Renderer* renderer, int startY);
    
    TTF_Font* font_;
    InventoryManager& inventoryManager_;
    
    // 模式
    bool purchaseMode_ = false;
    
    // 进货列表
    struct PurchaseItem {
        std::string itemId;
        int quantity;
    };
    std::vector<PurchaseItem> purchaseList_;
    int totalPurchaseCost_ = 0;
    
    // UI 元素
    std::shared_ptr<UILabel> titleLabel_;
    std::shared_ptr<UILabel> statsLabel_;
    std::shared_ptr<UIButton> purchaseBtn_;
    std::shared_ptr<UIButton> closeBtn_;
    std::shared_ptr<UIButton> confirmPurchaseBtn_;
};

} // namespace BikeShopTycoon