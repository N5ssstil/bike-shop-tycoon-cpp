#include "HUD.h"
#include "core/TimeManager.h"
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
    moneyLabel_->SetText(u8"¥ 10,000");
    moneyLabel_->SetColor({100, 200, 100, 255});
    AddChild(moneyLabel_);
    
    reputationLabel_ = std::make_shared<UILabel>("RepLabel", font);
    reputationLabel_->SetPosition(860, 15);
    reputationLabel_->SetText(u8"口碑: 10");
    reputationLabel_->SetColor({230, 180, 50, 255});
    AddChild(reputationLabel_);
    
    dayLabel_ = std::make_shared<UILabel>("DayLabel", font);
    dayLabel_->SetPosition(1720, 15);
    dayLabel_->SetText(u8"第 1 天");
    dayLabel_->SetColor({180, 180, 200, 255});
    AddChild(dayLabel_);
}

HUD::~HUD() = default;

void HUD::Update(const PlayerData& playerData) {
    // 格式化金钱
    std::ostringstream oss;
    if (playerData.money >= 10000) {
        oss << u8"¥ " << std::fixed << std::setprecision(1) << (playerData.money / 10000.0f) << u8"万";
    } else {
        oss << u8"¥ " << playerData.money;
    }
    moneyLabel_->SetText(oss.str());
    
    // 更新口碑
    reputationLabel_->SetText(u8"口碑: " + std::to_string(playerData.reputation));
    
    // 更新天数
    dayLabel_->SetText(u8"第 " + std::to_string(playerData.day) + u8" 天");
}

void HUD::Render(SDL_Renderer* renderer) {
    UIPanel::Render(renderer);
    
    // 渲染时间进度条
    auto& tm = TimeManager::Instance();
    float progress = tm.GetDayProgress();
    
    // 进度条位置（在天数标签下方）
    int barX = 1720;
    int barY = 45;
    int barWidth = 150;
    int barHeight = 8;
    
    // 背景
    SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255);
    SDL_Rect bgRect = {barX, barY, barWidth, barHeight};
    SDL_RenderFillRect(renderer, &bgRect);
    
    // 进度
    SDL_SetRenderDrawColor(renderer, 100, 150, 200, 255);
    SDL_Rect progressRect = {barX, barY, static_cast<int>(barWidth * progress), barHeight};
    SDL_RenderFillRect(renderer, &progressRect);
    
    // 边框
    SDL_SetRenderDrawColor(renderer, 80, 80, 90, 255);
    SDL_RenderDrawRect(renderer, &bgRect);
}

} // namespace BikeShopTycoon