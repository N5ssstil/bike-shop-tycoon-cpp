#pragma once

#include "UIManager.h"
#include "systems/RepairService.h"
#include "data/Types.h"
#include <vector>

namespace BikeShopTycoon {

/**
 * 维修面板 - 显示工单、创建维修
 */
class RepairPanel : public UIPanel {
public:
    RepairPanel(TTF_Font* font, RepairService& repairService);
    ~RepairPanel() override;
    
    void Render(SDL_Renderer* renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    
    void AddJob(const RepairJob& job);
    void Refresh();
    
private:
    void CreateUI();
    void RenderJobList(SDL_Renderer* renderer, int startY);
    void RenderCreateForm(SDL_Renderer* renderer, int startY);
    
    TTF_Font* font_;
    RepairService& repairService_;
    
    // 工单列表
    std::vector<RepairJob> activeJobs_;
    int todayIncome_ = 0;
    
    // 创建模式
    bool createMode_ = false;
    int selectedRepairType_ = 0;
    
    // UI 元素
    std::shared_ptr<UILabel> titleLabel_;
    std::shared_ptr<UILabel> statsLabel_;
    std::shared_ptr<UIButton> newJobBtn_;
    std::shared_ptr<UIButton> closeBtn_;
};

} // namespace BikeShopTycoon