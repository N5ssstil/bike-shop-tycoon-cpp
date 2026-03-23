#include "CustomerDetailPanel.h"

namespace BikeShopTycoon {

CustomerDetailPanel::CustomerDetailPanel(TTF_Font* font)
    : UIPanel("CustomerDetailPanel")
    , font_(font)
{
    SetPosition(460, 150);
    SetSize(1000, 780);
    SetBackgroundColor({40, 40, 48, 250});
    
    CreateUI();
}

CustomerDetailPanel::~CustomerDetailPanel() = default;

void CustomerDetailPanel::CreateUI() {
    // 名字
    nameLabel_ = std::make_shared<UILabel>("NameLabel", font_);
    nameLabel_->SetPosition(50, 30);
    nameLabel_->SetText("顾客名称");
    nameLabel_->SetColor({255, 255, 255, 255});
    AddChild(nameLabel_);
    
    // 类型
    typeLabel_ = std::make_shared<UILabel>("TypeLabel", font_);
    typeLabel_->SetPosition(50, 80);
    typeLabel_->SetText("类型");
    typeLabel_->SetColor({180, 180, 190, 255});
    AddChild(typeLabel_);
    
    // 预算
    budgetLabel_ = std::shared_ptr<UILabel>();
    
    // 需求
    needsLabel_ = std::make_shared<UILabel>("NeedsLabel", font_);
    needsLabel_->SetPosition(50, 200);
    needsLabel_->SetText("需求:");
    needsLabel_->SetColor({200, 200, 210, 255});
    AddChild(needsLabel_);
    
    // 故事
    storyLabel_ = std::make_shared<UILabel>("StoryLabel", font_);
    storyLabel_->SetPosition(50, 350);
    storyLabel_->SetText("");
    storyLabel_->SetColor({230, 180, 50, 255});
    AddChild(storyLabel_);
    
    // 操作按钮
    sellBtn_ = std::make_shared<UIButton>("SellBtn", font_);
    sellBtn_->SetPosition(100, 550);
    sellBtn_->SetSize(180, 50);
    sellBtn_->SetText("推荐商品");
    sellBtn_->SetOnClick([this]() {
        if (onAction_) onAction_("sell", currentCustomer_);
    });
    AddChild(sellBtn_);
    
    repairBtn_ = std::make_shared<UIButton>("RepairBtn", font_);
    repairBtn_->SetPosition(300, 550);
    repairBtn_->SetSize(180, 50);
    repairBtn_->SetText("维修服务");
    repairBtn_->SetOnClick([this]() {
        if (onAction_) onAction_("repair", currentCustomer_);
    });
    AddChild(repairBtn_);
    
    dismissBtn_ = std::make_shared<UIButton>("DismissBtn", font_);
    dismissBtn_->SetPosition(500, 550);
    dismissBtn_->SetSize(180, 50);
    dismissBtn_->SetText("送客");
    dismissBtn_->SetOnClick([this]() {
        if (onAction_) onAction_("dismiss", currentCustomer_);
        SetVisible(false);
    });
    AddChild(dismissBtn_);
    
    // 关闭按钮
    closeBtn_ = std::shared_ptr<UIButton>();
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
        case CustomerType::Student: typeStr = "🎓 学生党"; break;
        case CustomerType::Commuter: typeStr = "🚴 通勤族"; break;
        case CustomerType::CyclingEnthusiast: typeStr = "💪 骑行爱好者"; break;
        case CustomerType::Racer: typeStr = "🏆 竞赛车手"; break;
        case CustomerType::Influencer: typeStr = "📱 网红博主"; break;
    }
    typeLabel_->SetText(typeStr);
    
    // 预算
    std::string budgetStr = "预算: ¥" + std::to_string(currentCustomer_.budget);
    
    // 需求
    std::string needs = "需求:\n";
    if (currentCustomer_.needs.needForRacing) needs += "  🏁 竞赛用车\n";
    if (currentCustomer_.needs.needForCommuting) needs += "  🚲 通勤代步\n";
    if (currentCustomer_.needs.needForTraining) needs += "  💪 训练健身\n";
    if (currentCustomer_.needs.needForBeginners) needs += "  🎓 新手入门\n";
    if (currentCustomer_.needs.needHighVisual) needs += "  ✨ 高颜值\n";
    if (!currentCustomer_.needs.preferredBrand.empty()) 
        needs += "  🏷️ 偏好品牌: " + currentCustomer_.needs.preferredBrand + "\n";
    if (!currentCustomer_.needs.preferredColor.empty())
        needs += "  🎨 偏好颜色: " + currentCustomer_.needs.preferredColor + "\n";
    
    needsLabel_->SetText(needs);
    
    // 故事
    if (!currentCustomer_.storyId.empty()) {
        storyLabel_->SetText("📝 这位顾客似乎有故事...");
    } else {
        storyLabel_->SetText("");
    }
}

void CustomerDetailPanel::Render(SDL_Renderer* renderer) {
    if (!IsVisible()) return;
    
    // 绘制背景
    SDL_SetRenderDrawColor(renderer, 40, 40, 48, 250);
    SDL_RenderFillRect(renderer, &rect_);
    
    // 绘制边框
    SDL_SetRenderDrawColor(renderer, 80, 80, 90, 255);
    SDL_RenderDrawRect(renderer, &rect_);
    
    // 标题
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font_, "顾客详情", white);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {400, 10, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 预算
    SDL_Color green = {100, 200, 100, 255};
    std::string budgetStr = "预算: ¥" + std::to_string(currentCustomer_.budget);
    surface = TTF_RenderText_Blended(font_, budgetStr.c_str(), green);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {50, 130, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 满意度
    SDL_Color yellow = {230, 180, 50, 255};
    std::string satStr = "满意度: " + std::to_string(currentCustomer_.satisfaction) + "%";
    surface = TTF_RenderText_Blended(font_, satStr.c_str(), yellow);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {250, 130, surface->w, surface->h};
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