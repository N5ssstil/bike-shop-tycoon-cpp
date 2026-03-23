#include "InventoryPanel.h"
#include "core/Game.h"
#include <sstream>

namespace BikeShopTycoon {

InventoryPanel::InventoryPanel(TTF_Font* font, InventoryManager& inventoryManager)
    : UIPanel("InventoryPanel")
    , font_(font)
    , inventoryManager_(inventoryManager)
{
    SetPosition(200, 150);
    SetSize(1520, 780);
    SetBackgroundColor({35, 35, 42, 250});
    
    CreateUI();
}

InventoryPanel::~InventoryPanel() = default;

void InventoryPanel::CreateUI() {
    // 标题
    titleLabel_ = std::make_shared<UILabel>("Title", font_);
    titleLabel_->SetPosition(660, 20);
    titleLabel_->SetText("📦 库存管理");
    titleLabel_->SetColor({255, 255, 255, 255});
    AddChild(titleLabel_);
    
    // 统计信息
    statsLabel_ = std::make_shared<UILabel>("Stats", font_);
    statsLabel_->SetPosition(50, 700);
    statsLabel_->SetText("容量: 0/50 | 总价值: ¥0");
    statsLabel_->SetColor({180, 180, 190, 255});
    AddChild(statsLabel_);
    
    // 进货按钮
    purchaseBtn_ = std::make_shared<UIButton>("PurchaseBtn", font_);
    purchaseBtn_->SetPosition(1300, 700);
    purchaseBtn_->SetSize(120, 44);
    purchaseBtn_->SetText("进货");
    purchaseBtn_->SetOnClick([this]() {
        purchaseMode_ = !purchaseMode_;
        purchaseList_.clear();
        totalPurchaseCost_ = 0;
    });
    AddChild(purchaseBtn_);
    
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

void InventoryPanel::RefreshInventory() {
    auto& inventory = inventoryManager_.GetInventory();
    
    std::ostringstream oss;
    oss << "容量: " << inventory.usedCapacity << "/" << inventory.maxCapacity
        << " | 总价值: ¥" << inventory.GetTotalValue();
    statsLabel_->SetText(oss.str());
}

void InventoryPanel::ShowPurchaseMode(bool show) {
    purchaseMode_ = show;
    if (show) {
        purchaseList_.clear();
        totalPurchaseCost_ = 0;
    }
}

void InventoryPanel::Render(SDL_Renderer* renderer) {
    if (!IsVisible()) return;
    
    // 绘制背景
    SDL_SetRenderDrawColor(renderer, 35, 35, 42, 250);
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
    if (purchaseMode_) {
        RenderPurchaseList(renderer, 80);
    } else {
        RenderInventoryList(renderer, 80);
    }
}

void InventoryPanel::RenderInventoryList(SDL_Renderer* renderer, int startY) {
    auto& inventory = inventoryManager_.GetInventory();
    
    SDL_Color white = {220, 220, 230, 255};
    SDL_Color gray = {140, 140, 150, 255};
    SDL_Color green = {100, 200, 100, 255};
    
    // 表头
    SDL_Surface* surface = TTF_RenderText_Blended(font_, "商品名称                    数量    进价    售价", gray);
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
    
    // 商品列表
    int y = startY + 50;
    for (const auto& item : inventory.items) {
        // 名称
        std::string line = item.itemData.name;
        surface = TTF_RenderText_Blended(font_, line.c_str(), white);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {50, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        // 数量
        line = "x" + std::to_string(item.quantity);
        surface = TTF_RenderText_Blended(font_, line.c_str(), white);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {500, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        // 进价
        line = "¥" + std::to_string(item.purchasePrice);
        surface = TTF_RenderText_Blended(font_, line.c_str(), gray);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {650, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        // 售价
        line = "¥" + std::to_string(item.itemData.sellPrice);
        surface = TTF_RenderText_Blended(font_, line.c_str(), green);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {850, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        y += 40;
        if (y > 650) break; // 限制显示数量
    }
}

void InventoryPanel::RenderPurchaseList(SDL_Renderer* renderer, int startY) {
    auto& products = ProductDatabase::Instance().GetAllItems();
    
    SDL_Color white = {220, 220, 230, 255};
    SDL_Color gray = {140, 140, 150, 255};
    SDL_Color green = {100, 200, 100, 255};
    SDL_Color yellow = {230, 180, 50, 255};
    
    // 标题
    SDL_Surface* surface = TTF_RenderText_Blended(font_, "选择商品进货 (点击+/-调整数量)", yellow);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {50, startY, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 表头
    surface = TTF_RenderText_Blended(font_, "商品名称                    进价    售价    数量", gray);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {50, startY + 40, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 分隔线
    SDL_SetRenderDrawColor(renderer, 60, 60, 70, 255);
    SDL_RenderDrawLine(renderer, 50, startY + 75, 1470, startY + 75);
    
    // 商品列表
    int y = startY + 90;
    for (const auto& product : products) {
        // 名称
        std::string line = product.name;
        surface = TTF_RenderText_Blended(font_, line.c_str(), white);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {50, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        // 进价
        line = "¥" + std::to_string(product.purchasePrice);
        surface = TTF_RenderText_Blended(font_, line.c_str(), gray);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {500, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        // 售价
        line = "¥" + std::to_string(product.sellPrice);
        surface = TTF_RenderText_Blended(font_, line.c_str(), green);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {650, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        // 数量
        auto it = std::find_if(purchaseList_.begin(), purchaseList_.end(),
            [&](const PurchaseItem& p) { return p.itemId == product.id; });
        int qty = it != purchaseList_.end() ? it->quantity : 0;
        
        line = std::to_string(qty);
        surface = TTF_RenderText_Blended(font_, line.c_str(), white);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {850, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        
        y += 40;
        if (y > 650) break;
    }
    
    // 总价
    std::ostringstream oss;
    oss << "进货总价: ¥" << totalPurchaseCost_;
    surface = TTF_RenderText_Blended(font_, oss.str().c_str(), yellow);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {1100, 700, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

bool InventoryPanel::HandleEvent(const SDL_Event& event) {
    if (!IsVisible()) return false;
    return UIPanel::HandleEvent(event);
}

} // namespace BikeShopTycoon