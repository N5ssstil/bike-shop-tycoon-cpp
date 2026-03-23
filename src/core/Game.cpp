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
    
    // 加载字体 - 优先使用支持中文的字体
    // Windows: 使用微软雅黑
    // Linux: 使用 DejaVuSans（不支持中文，需要另外配置）
    
#ifdef _WIN32
    // Windows 系统字体路径
    const char* chineseFont = "C:/Windows/Fonts/msyh.ttc";  // 微软雅黑
    const char* fallbackFont = "C:/Windows/Fonts/arial.ttf";
#else
    // Linux 系统字体路径
    const char* chineseFont = "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf";
    const char* fallbackFont = "/usr/share/fonts/dejavu/DejaVuSans.ttf";
#endif
    
    // 先尝试加载中文字体
    font_ = TTF_OpenFont(chineseFont, 24);
    fontLarge_ = TTF_OpenFont(chineseFont, 36);
    
    // 如果中文字体加载失败，尝试 assets 目录的字体
    if (!font_) {
        font_ = TTF_OpenFont("assets/fonts/arial.ttf", 24);
        fontLarge_ = TTF_OpenFont("assets/fonts/arial_bold.ttf", 36);
    }
    
    // 最后尝试备用字体
    if (!font_) {
        std::cerr << "警告: 字体加载失败: " << TTF_GetError() << std::endl;
        std::cerr << "尝试使用系统字体..." << std::endl;
        font_ = TTF_OpenFont(fallbackFont, 24);
        fontLarge_ = TTF_OpenFont(fallbackFont, 36);
    }
    
    if (font_) {
        std::cout << "字体加载成功!" << std::endl;
    } else {
        std::cerr << "错误: 无法加载任何字体!" << std::endl;
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
    
    // 创建顾客管理器
    customerManager_ = std::make_unique<CustomerManager>(playerData_);
    
    // 设置顾客进店回调
    customerManager_->SetOnCustomerEnter([this](const Customer& customer) {
        std::cout << "顾客进店: " << customer.name << std::endl;
    });
    
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
    
    // 更新顾客管理器
    if (customerManager_) {
        customerManager_->Update(deltaTime);
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
    
    // 渲染顾客
    RenderCustomers();
    
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
        // 使用 UTF8 版本渲染中文
        SDL_Surface* surface = TTF_RenderUTF8_Blended(fontLarge_, 
            u8"狗狗自行车店", titleColor);
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
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, 
            u8"欢迎光临！顾客会陆续进店...", textColor);
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
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, 
            u8"按 ESC 退出游戏", hintColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
            SDL_Rect dest = {20, height_ - 40, surface->w, surface->h};
            SDL_RenderCopy(renderer_, texture, nullptr, &dest);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
}

void Game::RenderCustomers() {
    if (!customerManager_) return;
    
    const auto& customers = customerManager_->GetActiveCustomers();
    int customerIndex = 0;
    
    for (const auto& customer : customers) {
        // 计算顾客位置（横向排列）
        int customerWidth = 200;
        int customerHeight = 280;
        int startX = 100;
        int startY = 250;
        int spacing = 220;
        
        int x = startX + customerIndex * spacing;
        int y = startY;
        
        // 绘制顾客背景框
        SDL_SetRenderDrawColor(renderer_, 50, 50, 55, 255);
        SDL_Rect customerBg = {x, y, customerWidth, customerHeight};
        SDL_RenderFillRect(renderer_, &customerBg);
        
        // 绘制边框
        SDL_SetRenderDrawColor(renderer_, 80, 80, 90, 255);
        SDL_RenderDrawRect(renderer_, &customerBg);
        
        // 绘制顾客图标（简单的圆形代表头像）
        SDL_SetRenderDrawColor(renderer_, 150, 150, 160, 255);
        int centerX = x + customerWidth / 2;
        int centerY = y + 60;
        // 用矩形简化（SDL2 没有直接画圆的函数）
        SDL_Rect headRect = {centerX - 30, centerY - 30, 60, 60};
        SDL_RenderFillRect(renderer_, &headRect);
        
        // 显示顾客名字
        if (font_) {
            SDL_Color nameColor = {220, 220, 230, 255};
            SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, customer.name.c_str(), nameColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
                SDL_Rect dest = {x + (customerWidth - surface->w) / 2, y + 100, surface->w, surface->h};
                SDL_RenderCopy(renderer_, texture, nullptr, &dest);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
            }
        }
        
        // 显示顾客类型
        std::string typeStr;
        switch (customer.type) {
            case CustomerType::Student: typeStr = u8"学生"; break;
            case CustomerType::Commuter: typeStr = u8"通勤族"; break;
            case CustomerType::CyclingEnthusiast: typeStr = u8"骑行爱好者"; break;
            case CustomerType::Racer: typeStr = u8"车手"; break;
            case CustomerType::Influencer: typeStr = u8"网红"; break;
        }
        
        if (font_) {
            SDL_Color typeColor = {180, 180, 200, 255};
            SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, typeStr.c_str(), typeColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
                SDL_Rect dest = {x + (customerWidth - surface->w) / 2, y + 130, surface->w, surface->h};
                SDL_RenderCopy(renderer_, texture, nullptr, &dest);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
            }
        }
        
        // 显示预算
        if (font_) {
            SDL_Color budgetColor = {100, 200, 100, 255};
            std::string budgetStr = u8"预算: ¥" + std::to_string(customer.budget);
            SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, budgetStr.c_str(), budgetColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
                SDL_Rect dest = {x + (customerWidth - surface->w) / 2, y + 160, surface->w, surface->h};
                SDL_RenderCopy(renderer_, texture, nullptr, &dest);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
            }
        }
        
        // 显示耐心条
        int patienceBarWidth = customerWidth - 20;
        int patienceBarHeight = 10;
        int patienceBarX = x + 10;
        int patienceBarY = y + 200;
        
        // 背景
        SDL_SetRenderDrawColor(renderer_, 40, 40, 45, 255);
        SDL_Rect patienceBg = {patienceBarX, patienceBarY, patienceBarWidth, patienceBarHeight};
        SDL_RenderFillRect(renderer_, &patienceBg);
        
        // 进度
        float patiencePercent = customer.patience / 100.0f;
        SDL_Color patienceColor;
        if (patiencePercent > 0.6f) {
            patienceColor = {100, 200, 100, 255}; // 绿色
        } else if (patiencePercent > 0.3f) {
            patienceColor = {230, 180, 50, 255}; // 黄色
        } else {
            patienceColor = {200, 80, 80, 255}; // 红色
        }
        SDL_SetRenderDrawColor(renderer_, patienceColor.r, patienceColor.g, patienceColor.b, patienceColor.a);
        SDL_Rect patienceFill = {patienceBarX, patienceBarY, 
                                 static_cast<int>(patienceBarWidth * patiencePercent), patienceBarHeight};
        SDL_RenderFillRect(renderer_, &patienceFill);
        
        // 显示"点击接待"提示
        if (font_) {
            SDL_Color hintColor = {120, 120, 140, 255};
            SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, u8"点击接待", hintColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
                SDL_Rect dest = {x + (customerWidth - surface->w) / 2, y + 230, surface->w, surface->h};
                SDL_RenderCopy(renderer_, texture, nullptr, &dest);
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
            }
        }
        
        customerIndex++;
    }
}

void Game::CleanUp() {
    // 清理顾客管理器
    customerManager_.reset();
    
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