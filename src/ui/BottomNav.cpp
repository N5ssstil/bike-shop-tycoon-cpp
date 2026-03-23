#include "BottomNav.h"
#include "core/Game.h"
#include "core/TimeManager.h"

namespace BikeShopTycoon {

BottomNav::BottomNav(TTF_Font* font)
    : UIPanel("BottomNav")
    , font_(font)
{
    // 底部导航栏位置
    SetPosition(0, 1020);
    SetSize(1920, 60);
    SetBackgroundColor({20, 20, 25, 250});
    
    CreateButtons();
}

BottomNav::~BottomNav() = default;

void BottomNav::CreateButtons() {
    int btnWidth = 120;
    int btnHeight = 40;
    int startX = 200;
    int y = 10;
    int spacing = 150;
    
    // 库存按钮
    inventoryBtn_ = std::make_shared<UIButton>("InventoryBtn", font_);
    inventoryBtn_->SetPosition(startX, y);
    inventoryBtn_->SetSize(btnWidth, btnHeight);
    inventoryBtn_->SetText(u8"📦 库存");
    inventoryBtn_->SetOnClick([this]() {
        if (onNavClick_) onNavClick_("inventory");
    });
    AddChild(inventoryBtn_);
    
    // 维修按钮
    repairBtn_ = std::make_shared<UIButton>("RepairBtn", font_);
    repairBtn_->SetPosition(startX + spacing, y);
    repairBtn_->SetSize(btnWidth, btnHeight);
    repairBtn_->SetText(u8"🔧 维修");
    repairBtn_->SetOnClick([this]() {
        if (onNavClick_) onNavClick_("repair");
    });
    AddChild(repairBtn_);
    
    // 保存按钮
    saveBtn_ = std::make_shared<UIButton>("SaveBtn", font_);
    saveBtn_->SetPosition(startX + spacing * 2, y);
    saveBtn_->SetSize(btnWidth, btnHeight);
    saveBtn_->SetText(u8"💾 保存");
    saveBtn_->SetOnClick([this]() {
        if (onNavClick_) onNavClick_("save");
    });
    AddChild(saveBtn_);
    
    // 暂停按钮
    pauseBtn_ = std::make_shared<UIButton>("PauseBtn", font_);
    pauseBtn_->SetPosition(startX + spacing * 3, y);
    pauseBtn_->SetSize(btnWidth, btnHeight);
    pauseBtn_->SetText(u8"⏸ 暂停");
    pauseBtn_->SetOnClick([this]() {
        auto& tm = TimeManager::Instance();
        tm.SetPaused(!tm.IsRunning());
        // 更新按钮文字
        if (tm.IsRunning()) {
            pauseBtn_->SetText(u8"⏸ 暂停");
        } else {
            pauseBtn_->SetText(u8"▶ 继续");
        }
        if (onNavClick_) onNavClick_("pause");
    });
    AddChild(pauseBtn_);
    
    // 帮助按钮
    auto helpBtn = std::make_shared<UIButton>("HelpBtn", font_);
    helpBtn->SetPosition(startX + spacing * 4, y);
    helpBtn->SetSize(btnWidth, btnHeight);
    helpBtn->SetText(u8"❓ 帮助");
    helpBtn->SetOnClick([this]() {
        if (onNavClick_) onNavClick_("help");
    });
    AddChild(helpBtn);
}

} // namespace BikeShopTycoon