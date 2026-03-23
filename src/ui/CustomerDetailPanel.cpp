#include "CustomerDetailPanel.h"

namespace BikeShopTycoon {

CustomerDetailPanel::CustomerDetailPanel(TTF_Font* font)
    : UIPanel("CustomerDetailPanel")
    , font_(font)
{
    SetPosition(460, 150);
    SetSize(1000, 780);
    SetBackgroundColor({40, 40, 48, 250});
    SetVisible(false); // 默认隐藏
    
    CreateUI();
}

CustomerDetailPanel::~CustomerDetailPanel() = default;

void CustomerDetailPanel::CreateUI() {
    // 名字
    nameLabel_ = std::make_shared<UILabel>("NameLabel", font_);
    nameLabel_->SetPosition(50, 50);
    nameLabel_->SetText(u8"顾客名称");
    nameLabel_->SetColor({255, 255, 255, 255});
    AddChild(nameLabel_);
    
    // 类型
    typeLabel_ = std::make_shared<UILabel>("TypeLabel", font_);
    typeLabel_->SetPosition(50, 100);
    typeLabel_->SetText(u8"类型");
    typeLabel_->SetColor({180, 180, 190, 255});
    AddChild(typeLabel_);
    
    // 需求
    needsLabel_ = std::make_shared<UILabel>("NeedsLabel", font_);
    needsLabel_->SetPosition(50, 200);
    needsLabel_->SetText(u8"需求:");
    needsLabel_->SetColor({200, 200, 210, 255});
    AddChild(needsLabel_);
    
    // 故事
    storyLabel_ = std::make_shared<UILabel>("StoryLabel", font_);
    storyLabel_->SetPosition(50, 400);
    storyLabel_->SetText(u8"");
    storyLabel_->SetColor({230, 180, 50, 255});
    AddChild(storyLabel_);
    
    // 推荐商品按钮
    sellBtn_ = std::make_shared<UIButton>("SellBtn", font_);
    sellBtn_->SetPosition(100, 600);
    sellBtn_->SetSize(180, 50);
    sellBtn_->SetText(u8"推荐商品");
    sellBtn_->SetOnClick([this]() {
        if (onAction_) onAction_("sell", currentCustomer_);
    });
    AddChild(sellBtn_);
    
    // 维修服务按钮
    repairBtn_ = std::make_shared<UIButton>("RepairBtn", font_);
    repairBtn_->SetPosition(300, 600);
    repairBtn_->SetSize(180, 50);
    repairBtn_->SetText(u8"维修服务");
    repairBtn_->SetOnClick([this]() {
        if (onAction_) onAction_("repair", currentCustomer_);
    });
    AddChild(repairBtn_);
    
    // 送客按钮
    dismissBtn_ = std::make_shared<UIButton>("DismissBtn", font_);
    dismissBtn_->SetPosition(500, 600);
    dismissBtn_->SetSize(180, 50);
    dismissBtn_->SetText(u8"送客");
    dismissBtn_->SetOnClick([this]() {
        if (onAction_) onAction_("dismiss", currentCustomer_);
        SetVisible(false);
    });
    AddChild(dismissBtn_);
    
    // 关闭按钮
    closeBtn_ = std::make_shared<UIButton>("CloseBtn", font_);
    closeBtn_->SetPosition(900, 20);
    closeBtn_->SetSize(80, 40);
    closeBtn_->SetText(u8"关闭");
    closeBtn_->SetOnClick([this]() {
        SetVisible(false);
    });
    AddChild(closeBtn_);
}

void CustomerDetailPanel::ShowCustomer(const Customer& customer) {
    currentCustomer_ = customer;
    SetVisible(true);
    UpdateDisplay();
}

void CustomerDetailPanel::UpdateDisplay() {
    // 名字
    nameLabel_->SetText(currentCustomer_.name);
    
    // 类型
    std::string typeStr;
    switch (currentCustomer_.type) {
        case CustomerType::Student: typeStr = u8"🎓 学生党"; break;
        case CustomerType::Commuter: typeStr = u8"🚴 通勤族"; break;
        case CustomerType::CyclingEnthusiast: typeStr = u8"💪 骑行爱好者"; break;
        case CustomerType::Racer: typeStr = u8"🏆 竞赛车手"; break;
        case CustomerType::Influencer: typeStr = u8"📱 网红博主"; break;
    }
    typeLabel_->SetText(typeStr);
    
    // 需求
    std::string needs = u8"需求:\n";
    if (currentCustomer_.needs.needForRacing) needs += u8"  🏁 竞赛用车\n";
    if (currentCustomer_.needs.needForCommuting) needs += u8"  🚲 通勤代步\n";
    if (currentCustomer_.needs.needForTraining) needs += u8"  💪 训练健身\n";
    if (currentCustomer_.needs.needForBeginners) needs += u8"  🎓 新手入门\n";
    if (currentCustomer_.needs.needHighVisual) needs += u8"  ✨ 高颜值\n";
    if (!currentCustomer_.needs.preferredBrand.empty()) 
        needs += u8"  🏷️ 偏好品牌: " + currentCustomer_.needs.preferredBrand + u8"\n";
    if (!currentCustomer_.needs.preferredColor.empty())
        needs += u8"  🎨 偏好颜色: " + currentCustomer_.needs.preferredColor + u8"\n";
    
    needsLabel_->SetText(needs);
    
    // 故事
    if (!currentCustomer_.storyId.empty()) {
        storyLabel_->SetText(u8"📝 这位顾客似乎有故事...");
    } else {
        storyLabel_->SetText(u8"");
    }
}

void CustomerDetailPanel::Render(SDL_Renderer* renderer) {
    if (!IsVisible()) return;
    
    // 绘制背景
    SDL_SetRenderDrawColor(renderer, 40, 40, 48, 250);
    SDL_RenderFillRect(renderer, &rect_);
    
    // 绘制边框
    SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255);
    SDL_RenderDrawRect(renderer, &rect_);
    
    // 标题
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, u8"顾客详情", white);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {400, 20, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 预算
    SDL_Color green = {100, 200, 100, 255};
    std::string budgetStr = u8"预算: ¥" + std::to_string(currentCustomer_.budget);
    surface = TTF_RenderUTF8_Blended(font_, budgetStr.c_str(), green);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {50, 150, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 满意度
    SDL_Color yellow = {230, 180, 50, 255};
    std::string satStr = u8"满意度: " + std::to_string(currentCustomer_.satisfaction) + u8"%";
    surface = TTF_RenderUTF8_Blended(font_, satStr.c_str(), yellow);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {300, 150, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 耐心条
    int barX = 50;
    int barY = 180;
    int barWidth = 300;
    int barHeight = 15;
    
    SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255);
    SDL_Rect bgRect = {barX, barY, barWidth, barHeight};
    SDL_RenderFillRect(renderer, &bgRect);
    
    float patiencePercent = currentCustomer_.patience / 100.0f;
    SDL_Color patienceColor;
    if (patiencePercent > 0.6f) {
        patienceColor = {100, 200, 100, 255};
    } else if (patiencePercent > 0.3f) {
        patienceColor = {230, 180, 50, 255};
    } else {
        patienceColor = {200, 80, 80, 255};
    }
    SDL_SetRenderDrawColor(renderer, patienceColor.r, patienceColor.g, patienceColor.b, patienceColor.a);
    SDL_Rect fillRect = {barX, barY, static_cast<int>(barWidth * patiencePercent), barHeight};
    SDL_RenderFillRect(renderer, &fillRect);
    
    // 耐心标签
    SDL_Color gray = {140, 140, 150, 255};
    surface = TTF_RenderUTF8_Blended(font_, u8"耐心值", gray);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {barX + barWidth + 10, barY - 2, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 渲染子元素
    for (auto& child : children_) {
        if (child->IsVisible()) {
            child->Render(renderer);
        }
    }
}

bool CustomerDetailPanel::HandleEvent(const SDL_Event& event) {
    if (!IsVisible()) return false;
    return UIPanel::HandleEvent(event);
}

} // namespace BikeShopTycoon