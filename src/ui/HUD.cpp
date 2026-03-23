#include "HUD.h"
#include <sstream>
#include <iomanip>

namespace BikeShopTycoon {

HUD::HUD(TTF_Font* font)
    : UIPanel("HUD")
    , font_(font)
{
    // 设置HUD位置（顶部）
    SetPosition(0, 0);
    SetSize(1920, 60);
    SetBackgroundColor({25, 25, 30, 240});
    
    // 创建标签
    moneyLabel_ = std::make_shared<UILabel>("MoneyLabel", font);
    moneyLabel_->SetPosition(20, 15);
    moneyLabel_->SetText("¥ 10,000");
    moneyLabel_->SetColor({100, 200, 100, 255});
    AddChild(moneyLabel_);
    
    reputationLabel_ = std::make_shared<UILabel>("RepLabel", font);
    reputationLabel_->SetPosition(860, 15);
    reputationLabel_->SetText("口碑: 10");
    reputationLabel_->SetColor({230, 180, 50, 255});
    AddChild(reputationLabel_);
    
    dayLabel_ = std::make_shared<UILabel>("DayLabel", font);
    dayLabel_->SetPosition(1720, 15);
    dayLabel_->SetText("第 1 天");
    dayLabel_->SetColor({180, 180, 200, 255});
    AddChild(dayLabel_);
}

HUD::~HUD() = default;

void HUD::Update(const PlayerData& playerData) {
    // 格式化金钱
    std::ostringstream oss;
    if (playerData.money >= 10000) {
        oss << "¥ " << std::fixed << std::setprecision(1) << (playerData.money / 10000.0f) << "万";
    } else {
        oss << "¥ " << playerData.money;
    }
    moneyLabel_->SetText(oss.str());
    
    // 更新口碑
    reputationLabel_->SetText("口碑: " + std::to_string(playerData.reputation));
    
    // 更新天数
    dayLabel_->SetText("第 " + std::to_string(playerData.day) + " 天");
}

void HUD::Render(SDL_Renderer* renderer) {
    UIPanel::Render(renderer);
}

} // namespace BikeShopTycoon