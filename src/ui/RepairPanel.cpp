#include "RepairPanel.h"
#include <sstream>

namespace BikeShopTycoon {

RepairPanel::RepairPanel(TTF_Font* font, RepairService& repairService)
    : UIPanel("RepairPanel")
    , font_(font)
    , repairService_(repairService)
{
    SetPosition(200, 150);
    SetSize(1520, 780);
    SetBackgroundColor({35, 38, 42, 250});
    
    CreateUI();
}

RepairPanel::~RepairPanel() = default;

void RepairPanel::CreateUI() {
    // 标题
    titleLabel_ = std::make_shared<UILabel>("Title", font_);
    titleLabel_->SetPosition(680, 20);
    titleLabel_->SetText("🔧 维修服务");
    titleLabel_->SetColor({255, 255, 255, 255});
    AddChild(titleLabel_);
    
    // 统计信息
    statsLabel_ = std::make_shared<UILabel>("Stats", font_);
    statsLabel_->SetPosition(50, 700);
    statsLabel_->SetText("进行中: 0 | 今日收入: ¥0");
    statsLabel_->SetColor({180, 180, 190, 255});
    AddChild(statsLabel_);
    
    // 新建工单按钮
    newJobBtn_ = std::make_shared<UIButton>("NewJobBtn", font_);
    newJobBtn_->SetPosition(1300, 700);
    newJobBtn_->SetSize(140, 44);
    newJobBtn_->SetText("新建工单");
    newJobBtn_->SetOnClick([this]() {
        createMode_ = !createMode_;
    });
    AddChild(newJobBtn_);
    
    // 关闭按钮
    closeBtn_ = std::make_shared<UIButton>("CloseBtn", font_);
    closeBtn_->SetPosition(1400, 20);
    closeBtn_->SetSize(80, 40);
    closeBtn_->SetText("关闭");
    closeBtn_->SetOnClick([this]() {
        SetVisible(false);
    });
    AddChild(closeBtn_);
}

void RepairPanel::AddJob(const RepairJob& job) {
    activeJobs_.push_back(job);
}

void RepairPanel::Refresh() {
    std::ostringstream oss;
    oss << "进行中: " << activeJobs_.size() << " | 今日收入: ¥" << todayIncome_;
    statsLabel_->SetText(oss.str());
}

void RepairPanel::Render(SDL_Renderer* renderer) {
    if (!IsVisible()) return;
    
    // 绘制背景
    SDL_SetRenderDrawColor(renderer, 35, 38, 42, 250);
    SDL_RenderFillRect(renderer, &rect_);
    
    // 绘制边框
    SDL_SetRenderDrawColor(renderer, 80, 80, 90, 255);
    SDL_RenderDrawRect(renderer, &rect_);
    
    // 渲染子元素
    for (auto& child : children_) {
        if (child->IsVisible()) {
            child->Render(renderer);
        }
    }
    
    // 渲染列表
    if (createMode_) {
        RenderCreateForm(renderer, 80);
    } else {
        RenderJobList(renderer, 80);
    }
}

void RepairPanel::RenderJobList(SDL_Renderer* renderer, int startY) {
    SDL_Color white = {220, 220, 230, 255};
    SDL_Color gray = {140, 140, 150, 255};
    SDL_Color green = {100, 200, 100, 255};
    SDL_Color yellow = {230, 180, 50, 255};
    
    if (activeJobs_.empty()) {
        SDL_Surface* surface = TTF_RenderText_Blended(font_, "暂无进行中的维修工单", gray);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {550, 350, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        return;
    }
    
    // 表头
    SDL_Surface* surface = TTF_RenderText_Blended(font_, "维修类型          顾客        费用    状态", gray);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {50, startY, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 分隔线
    SDL_SetRenderDrawColor(renderer, 60, 60, 70, 255);
    SDL_RenderDrawLine(renderer, 50, startY + 35, 1470, startY + 35);
    
    // 工单列表
    int y = startY + 50;
    for (const auto& job : activeJobs_) {
        // 类型
        std::string typeStr;
        switch (job.type) {
            case RepairType::FlatTire: typeStr = "🔧 爆胎修补"; break;
            case RepairType::GearAdjustment: typeStr = "⚙️ 变速调试"; break;
            case RepairType::BrakeService: typeStr = "🛑 刹车保养"; break;
            case RepairType::WheelTruing: typeStr = "⭕ 编圈调整"; break;
            case RepairType::FullService: typeStr = "🔩 全车保养"; break;
            case RepairType::CustomTuning: typeStr = "🎯 定制调校"; break;
        }
        
        surface = TTF_RenderText_Blended(font_, typeStr.c_str(), white);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {50, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        // 顾客
        surface = TTF_RenderText_Blended(font_, job.customer.name.c_str(), white);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {400, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        // 费用
        std::string costStr = "¥" + std::to_string(job.baseCost);
        surface = TTF_RenderText_Blended(font_, costStr.c_str(), green);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {650, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        // 状态
        std::string statusStr;
        SDL_Color statusColor = gray;
        switch (job.status) {
            case RepairJob::Status::Pending:
                statusStr = "等待中";
                statusColor = yellow;
                break;
            case RepairJob::Status::InProgress:
                statusStr = "进行中";
                statusColor = white;
                break;
            case RepairJob::Status::Completed:
                statusStr = "已完成";
                statusColor = green;
                break;
            case RepairJob::Status::Cancelled:
                statusStr = "已取消";
                statusColor = gray;
                break;
        }
        
        surface = TTF_RenderText_Blended(font_, statusStr.c_str(), statusColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {850, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        y += 50;
        if (y > 650) break;
    }
}

void RepairPanel::RenderCreateForm(SDL_Renderer* renderer, int startY) {
    SDL_Color white = {220, 220, 230, 255};
    SDL_Color gray = {140, 140, 150, 255};
    SDL_Color yellow = {230, 180, 50, 255};
    
    // 标题
    SDL_Surface* surface = TTF_RenderText_Blended(font_, "选择维修类型", yellow);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {50, startY, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 维修类型列表
    const char* repairTypes[] = {
        "🔧 爆胎修补 - ¥50 - 15分钟",
        "⚙️ 变速调试 - ¥80 - 30分钟",
        "🛑 刹车保养 - ¥60 - 20分钟",
        "⭕ 编圈调整 - ¥150 - 60分钟",
        "🔩 全车保养 - ¥300 - 120分钟",
        "🎯 定制调校 - ¥500 - 90分钟"
    };
    
    int y = startY + 60;
    for (int i = 0; i < 6; i++) {
        SDL_Color color = (i == selectedRepairType_) ? yellow : white;
        surface = TTF_RenderText_Blended(font_, repairTypes[i], color);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {100, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        y += 50;
    }
    
    // 提示
    surface = TTF_RenderText_Blended(font_, "点击选择维修类型，然后点击确认创建工单", gray);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {100, y + 20, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

bool RepairPanel::HandleEvent(const SDL_Event& event) {
    if (!IsVisible()) return false;
    
    if (createMode_ && event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;
        
        // 检查维修类型选择
        int startY = 140;
        for (int i = 0; i < 6; i++) {
            if (y >= startY + i * 50 && y < startY + (i + 1) * 50) {
                selectedRepairType_ = i;
                return true;
            }
        }
    }
    
    return UIPanel::HandleEvent(event);
}

} // namespace BikeShopTycoon