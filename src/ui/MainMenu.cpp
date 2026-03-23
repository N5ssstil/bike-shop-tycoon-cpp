#include "MainMenu.h"
#include "core/SaveSystem.h"
#include "core/Game.h"

namespace BikeShopTycoon {

MainMenu::MainMenu(TTF_Font* font)
    : UIPanel("MainMenu")
    , font_(font)
{
    SetPosition(0, 0);
    SetSize(1920, 1080);
    SetBackgroundColor({25, 25, 30, 255});
    
    CreateButtons();
}

MainMenu::~MainMenu() = default;

void MainMenu::CreateButtons() {
    // 标题
    titleLabel_ = std::make_shared<UILabel>("Title", font_);
    titleLabel_->SetPosition(660, 200);
    titleLabel_->SetText("🚴 Bike Shop Tycoon");
    titleLabel_->SetColor({255, 255, 255, 255});
    AddChild(titleLabel_);
    
    // 副标题
    subtitleLabel_ = std::make_shared<UILabel>("Subtitle", font_);
    subtitleLabel_->SetPosition(830, 280);
    subtitleLabel_->SetText("自行车店大亨");
    subtitleLabel_->SetColor({180, 180, 190, 255});
    AddChild(subtitleLabel_);
    
    // 新游戏按钮
    newGameBtn_ = std::make_shared<UIButton>("NewGameBtn", font_);
    newGameBtn_->SetPosition(860, 400);
    newGameBtn_->SetSize(200, 50);
    newGameBtn_->SetText("新游戏");
    newGameBtn_->SetOnClick([this]() {
        if (onMenuAction_) onMenuAction_("newgame");
    });
    AddChild(newGameBtn_);
    
    // 继续游戏按钮
    continueBtn_ = std::make_shared<UIButton>("ContinueBtn", font_);
    continueBtn_->SetPosition(860, 480);
    continueBtn_->SetSize(200, 50);
    continueBtn_->SetText("继续游戏");
    continueBtn_->SetOnClick([this]() {
        if (onMenuAction_) onMenuAction_("continue");
    });
    AddChild(continueBtn_);
    
    // 退出按钮
    quitBtn_ = std::make_shared<UIButton>("QuitBtn", font_);
    quitBtn_->SetPosition(860, 560);
    quitBtn_->SetSize(200, 50);
    quitBtn_->SetText("退出");
    quitBtn_->SetOnClick([this]() {
        if (onMenuAction_) onMenuAction_("quit");
    });
    AddChild(quitBtn_);
}

void MainMenu::Render(SDL_Renderer* renderer) {
    if (!IsVisible()) return;
    
    // 绘制背景
    SDL_SetRenderDrawColor(renderer, 25, 25, 30, 255);
    SDL_RenderFillRect(renderer, &rect_);
    
    // 渲染子元素
    for (auto& child : children_) {
        if (child->IsVisible()) {
            child->Render(renderer);
        }
    }
    
    // 显示存档状态
    SDL_Color gray = {100, 100, 110, 255};
    
    bool hasSave = SaveSystem::Instance().HasSave();
    const char* statusText = hasSave ? "检测到存档" : "暂无存档";
    
    SDL_Surface* surface = TTF_RenderText_Blended(font_, statusText, gray);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {870, 640, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 版本号
    surface = TTF_RenderText_Blended(font_, "v0.1 - C++ SDL2 Version", gray);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = {20, 1040, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

bool MainMenu::HandleEvent(const SDL_Event& event) {
    if (!IsVisible()) return false;
    return UIPanel::HandleEvent(event);
}

} // namespace BikeShopTycoon