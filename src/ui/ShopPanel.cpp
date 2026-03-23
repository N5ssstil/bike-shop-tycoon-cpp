#include "ShopPanel.h"

namespace BikeShopTycoon {

ShopPanel::ShopPanel(TTF_Font* font, CustomerManager& customerManager, InventoryManager& inventoryManager)
    : UIPanel("ShopPanel")
    , font_(font)
    , customerManager_(customerManager)
    , inventoryManager_(inventoryManager)
{
    SetPosition(0, 60);
    SetSize(1920, 900);
    SetBackgroundColor({30, 30, 38, 255});
    
    CreateUI();
}

ShopPanel::~ShopPanel() = default;

void ShopPanel::CreateUI() {
    // 标题
    titleLabel_ = std::make_shared<UILabel>("TitleLabel", font_);
    titleLabel_->SetPosition(860, 50);
    titleLabel_->SetText("🚴 自行车店");
    titleLabel_->SetColor({255, 255, 255, 255});
    AddChild(titleLabel_);
    
    // 状态文本
    statusLabel_ = std::make_shared<UILabel>("StatusLabel", font_);
    statusLabel_->SetPosition(660, 400);
    statusLabel_->SetText("等待顾客光临...");
    statusLabel_->SetColor({150, 150, 160, 255});
    AddChild(statusLabel_);
}

void ShopPanel::Update() {
    UpdateCustomerList();
}

void ShopPanel::UpdateCustomerList() {
    const auto& customers = customerManager_.GetActiveCustomers();
    
    if (customers.empty()) {
        statusLabel_->SetText("等待顾客光临...");
        statusLabel_->SetVisible(true);
    } else {
        statusLabel_->SetVisible(false);
        // TODO: 更新顾客按钮列表
    }
}

void ShopPanel::Render(SDL_Renderer* renderer) {
    UIPanel::Render(renderer);
    
    // 绘制顾客数量
    int count = customerManager_.GetActiveCustomerCount();
    std::string countText = "当前顾客: " + std::to_string(count) + "/3";
    
    SDL_Color white = {200, 200, 210, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font_, countText.c_str(), white);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {20, 750, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

bool ShopPanel::HandleEvent(const SDL_Event& event) {
    return UIPanel::HandleEvent(event);
}

} // namespace BikeShopTycoon