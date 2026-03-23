#include "BottomNav.h"
#include "core/Game.h"
#include "core/TimeManager.h"

namespace BikeShopTycoon {

BottomNav::BottomNav(TTF_Font* font)
    : UIPanel("BottomNav")
    , font_(font)
{
    SetPosition(0, 1020);
    SetSize(1920, 60);
    SetBackgroundColor({20, 20, 25, 250});
    
    CreateButtons();
}

BottomNav::~BottomNav() = default;

void BottomNav::CreateButtons() {
    // 库存按钮
    inventoryBtn_ = std::make_shared<UIButton>("InventoryBtn", font_);
    inventoryBtn_->SetPosition(710, 8);
    inventoryBtn_->SetSize(100, 44);
    inventoryBtn_->SetText("📦");
    inventoryBtn_->SetOnClick([this]() {
        if (onNavClick_) onNavClick_("inventory");
    });
    AddChild(inventoryBtn_);
    
    // 维修按钮
    repairBtn_ = std::make_shared<UIButton>("RepairBtn", font_);
    repairBtn_->SetPosition(830, 8);
    repairBtn_->SetSize(100, 44);
    repairBtn_->SetText("🔧");
    repairBtn_->SetOnClick([this]() {
        if (onNavClick_) onNavClick_("repair");
    });
    AddChild(repairBtn_);
    
    // 保存按钮
    saveBtn_ = std::make_shared<UIButton>("SaveBtn", font_);
    saveBtn_->SetPosition(950, 8);
    saveBtn_->SetSize(100, 44);
    saveBtn_->SetText("💾");
    saveBtn_->SetOnClick([this]() {
        if (onNavClick_) onNavClick_("save");
    });
    AddChild(saveBtn_);
    
    // 暂停按钮
    pauseBtn_ = std::make_shared<UIButton>("PauseBtn", font_);
    pauseBtn_->SetPosition(1070, 8);
    pauseBtn_->SetSize(100, 44);
    pauseBtn_->SetText("⏸️");
    pauseBtn_->SetOnClick([this]() {
        auto& tm = TimeManager::Instance();
        tm.SetPaused(!tm.IsRunning());
        if (onNavClick_) onNavClick_("pause");
    });
    AddChild(pauseBtn_);
}

} // namespace BikeShopTycoon