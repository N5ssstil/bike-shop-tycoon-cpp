#pragma once

#include "UIManager.h"
#include "core/TimeManager.h"
#include "data/Types.h"

namespace BikeShopTycoon {

/**
 * HUD - 顶部状态栏
 */
class HUD : public UIPanel {
public:
    HUD(TTF_Font* font);
    ~HUD() override;
    
    void Update(const PlayerData& playerData);
    void Render(SDL_Renderer* renderer) override;
    
private:
    std::shared_ptr<UILabel> moneyLabel_;
    std::shared_ptr<UILabel> reputationLabel_;
    std::shared_ptr<UILabel> dayLabel_;
    
    TTF_Font* font_;
};

} // namespace BikeShopTycoon