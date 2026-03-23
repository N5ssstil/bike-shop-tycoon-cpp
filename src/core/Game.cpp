#include "Game.h"
#include "ui/UIManager.h"
#include "ui/HUD.h"
#include "systems/CustomerManager.h"
#include "systems/InventoryManager.h"
#include "core/TimeManager.h"
#include <iostream>
#include <chrono>

namespace BikeShopTycoon {

Game& Game::Instance() {
    static Game instance;
    return instance;
}

Game::~Game() {
    CleanUp();
}

bool Game::Init(const std::string& title, int width, int height) {
    width_ = width;
    height_ = height;
    
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL 初始化失败: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // 设置窗口
    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_, height_,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    
    if (!window_) {
        std::cerr << "窗口创建失败: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // 创建渲染器
    renderer_ = SDL_CreateRenderer(window_, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!renderer_) {
        std::cerr << "渲染器创建失败: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // 初始化 SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image 初始化失败: " << IMG_GetError() << std::endl;
        return false;
    }
    
    // 初始化 SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf 初始化失败: " << TTF_GetError() << std::endl;
        return false;
    }
    
    // 加载字体
    font_ = TTF_OpenFont("assets/fonts/arial.ttf", 24);
    fontLarge_ = TTF_OpenFont("assets/fonts/arial_bold.ttf", 36);
    
    if (!font_) {
        std::cerr << "警告: 字体加载失败: " << TTF_GetError() << std::endl;
        std::cerr << "尝试使用系统字体..." << std::endl;
        font_ = TTF_OpenFont("/usr/share/fonts/dejavu/DejaVuSans.ttf", 24);
        fontLarge_ = TTF_OpenFont("/usr/share/fonts/dejavu/DejaVuSans-Bold.ttf", 36);
    }
    
    std::cout << "=== Bike Shop Tycoon ===" << std::endl;
    std::cout << "窗口: " << width_ << "x" << height_ << std::endl;
    std::cout << "SDL 初始化成功!" << std::endl;
    
    // 初始化游戏系统
    if (!InitSystems()) {
        std::cerr << "游戏系统初始化失败!" << std::endl;
        return false;
    }
    
    return true;
}

bool Game::InitSystems() {
    // 初始化 UI 管理器
    if (!UIManager::Instance().Init(renderer_, font_)) {
        std::cerr << "UI 管理器初始化失败!" << std::endl;
        return false;
    }
    
    // 创建 HUD
    hud_ = std::make_shared<HUD>(font_);
    hud_->Update(playerData_);
    UIManager::Instance().AddPanel(hud_);
    
    std::cout << "游戏系统初始化完成!" << std::endl;
    return true;
}

void Game::Run() {
    running_ = true;
    
    auto lastTime = std::chrono::high_resolution_clock::now();
    float accumulator = 0.0f;
    const float fixedDeltaTime = 1.0f / 60.0f; // 60 FPS
    
    while (running_) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // 限制 deltaTime 避免大跳跃
        if (deltaTime > 0.1f) deltaTime = 0.1f;
        
        HandleEvents();
        
        // 固定时间步更新
        accumulator += deltaTime;
        while (accumulator >= fixedDeltaTime) {
            Update(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }
        
        Render();
    }
}

void Game::Quit() {
    running_ = false;
}

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running_ = false;
                break;
                
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running_ = false;
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
                // 传递给 UI 系统处理
                UIManager::Instance().HandleEvent(event);
                break;
        }
    }
}

void Game::Update(float deltaTime) {
    // 更新时间管理器
    TimeManager::Instance().Update(deltaTime);
    
    // 检查是否需要推进到新的一天
    if (TimeManager::Instance().IsNewDay()) {
        playerData_.day++;
        std::cout << "进入第 " << playerData_.day << " 天" << std::endl;
    }
    
    // 更新 HUD
    hud_->Update(playerData_);
}

void Game::Render() {
    // 清屏 - 深色背景
    SDL_SetRenderDrawColor(renderer_, 26, 26, 30, 255);
    SDL_RenderClear(renderer_);
    
    // 渲染主内容区域（店铺场景）
    RenderShopScene();
    
    // 渲染 UI
    UIManager::Instance().Render();
    
    SDL_RenderPresent(renderer_);
}

void Game::RenderShopScene() {
    // 渲染店铺背景
    SDL_SetRenderDrawColor(renderer_, 35, 35, 40, 255);
    SDL_Rect shopArea = {0, 60, width_, height_ - 60};
    SDL_RenderFillRect(renderer_, &shopArea);
    
    // 渲染店铺标题
    if (fontLarge_) {
        SDL_Color titleColor = {200, 200, 210, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(fontLarge_, 
            "🚴 狗狗自行车店 🚴", titleColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
            SDL_Rect dest = {(width_ - surface->w) / 2, 120, surface->w, surface->h};
            SDL_RenderCopy(renderer_, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
    
    // 渲染欢迎文字
    if (font_) {
        SDL_Color textColor = {150, 150, 160, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font_, 
            "欢迎光临！顾客会陆续进店...", textColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
            SDL_Rect dest = {(width_ - surface->w) / 2, 180, surface->w, surface->h};
            SDL_RenderCopy(renderer_, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
    
    // 渲染底部提示
    if (font_) {
        SDL_Color hintColor = {100, 100, 110, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font_, 
            "按 ESC 退出游戏", hintColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
            SDL_Rect dest = {20, height_ - 40, surface->w, surface->h};
            SDL_RenderCopy(renderer_, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
}

void Game::CleanUp() {
    // 清理 UI
    UIManager::Instance().Shutdown();
    
    if (fontLarge_) {
        TTF_CloseFont(fontLarge_);
        fontLarge_ = nullptr;
    }
    
    if (font_) {
        TTF_CloseFont(font_);
        font_ = nullptr;
    }
    
    TTF_Quit();
    IMG_Quit();
    
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    
    SDL_Quit();
    std::cout << "游戏已退出" << std::endl;
}

} // namespace BikeShopTycoon