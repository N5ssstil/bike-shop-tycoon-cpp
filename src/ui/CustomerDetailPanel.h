#pragma once

#include "UIManager.h"
#include "data/Types.h"
#include <functional>

namespace BikeShopTycoon {

/**
 * 顾客详情面板
 */
class CustomerDetailPanel : public UIPanel {
public:
    using ActionCallback = std::function<void(const std::string&, const Customer&)>;
    
    CustomerDetailPanel(TTF_Font* font);
    ~CustomerDetailPanel() override;
    
    void Render(SDL_Renderer* renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    
    void ShowCustomer(const Customer& customer);
    void SetOnAction(ActionCallback callback) { onAction_ = callback; }
    
private:
    void CreateUI();
    void UpdateDisplay();
    
    TTF_Font* font_;
    Customer currentCustomer_;
    ActionCallback onAction_;
    
    std::shared_ptr<UILabel> nameLabel_;
    std::shared_ptr<UILabel> typeLabel_;
    std::shared_ptr<UILabel> budgetLabel_;
    std::shared_ptr<UILabel> needsLabel_;
    std::shared_ptr<UILabel> storyLabel_;
    
    std::shared_ptr<UIButton> sellBtn_;
    std::shared_ptr<UIButton> repairBtn_;
    std::shared_ptr<UIButton> dismissBtn_;
    std::shared_ptr<UIButton> closeBtn_;
};

} // namespace BikeShopTycoon