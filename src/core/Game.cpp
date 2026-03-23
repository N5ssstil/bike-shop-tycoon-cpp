#include "Game.h"
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
    
    // 加载默认字体
    font_ = TTF_OpenFont("assets/fonts/arial.ttf", 24);
    if (!font_) {
        std::cerr << "字体加载失败: " << TTF_GetError() << std::endl;
        // 继续运行，但字体功能不可用
    }
    
    std::cout << "=== Bike Shop Tycoon ===" << std::endl;
    std::cout << "窗口: " << width_ << "x" << height_ << std::endl;
    std::cout << "SDL 初始化成功!" << std::endl;
    
    return true;
}

void Game::Run() {
    running_ = true;
    
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (running_) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        HandleEvents();
        Update(deltaTime);
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
                // 处理鼠标点击
                // UIManager::Instance().HandleClick(event.button.x, event.button.y);
                break;
        }
    }
}

void Game::Update(float deltaTime) {
    // 更新游戏逻辑
    // TimeManager::Instance().Update(deltaTime);
    // CustomerManager::Instance().Update(deltaTime);
}

void Game::Render() {
    // 清屏 - 深色背景
    SDL_SetRenderDrawColor(renderer_, 26, 26, 30, 255);
    SDL_RenderClear(renderer_);
    
    // 渲染 UI
    // UIManager::Instance().Render();
    
    // 渲染测试文本
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font_, 
        "Bike Shop Tycoon - C++ Version", white);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        SDL_Rect dest = {20, 20, surface->w, surface->h};
        SDL_RenderCopy(renderer_, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    // 显示提示
    surface = TTF_RenderText_Blended(font_, 
        "Press ESC to quit", white);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        SDL_Rect dest = {20, 60, surface->w, surface->h};
        SDL_RenderCopy(renderer_, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    
    SDL_RenderPresent(renderer_);
}

void Game::CleanUp() {
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