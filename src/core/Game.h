#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
#include "data/Types.h"

namespace BikeShopTycoon {

// 前向声明
class UIManager;
class HUD;
class CustomerManager;
class InventoryManager;
class TimeManager;

class Game {
public:
    static Game& Instance();
    
    bool Init(const std::string& title, int width, int height);
    void Run();
    void Quit();
    
    // 渲染器访问
    SDL_Renderer* GetRenderer() const { return renderer_; }
    SDL_Window* GetWindow() const { return window_; }
    TTF_Font* GetFont() const { return font_; }
    TTF_Font* GetFontLarge() const { return fontLarge_; }
    
    // 游戏状态
    bool IsRunning() const { return running_; }
    void SetRunning(bool running) { running_ = running; }
    
    // 窗口尺寸
    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }
    
    // 玩家数据
    PlayerData& GetPlayerData() { return playerData_; }
    const PlayerData& GetPlayerData() const { return playerData_; }
    
    // 系统访问
    CustomerManager& GetCustomerManager() { return *customerManager_; }
    
private:
    Game() = default;
    ~Game();
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    
    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    
    void CleanUp();
    bool InitSystems();
    void RenderShopScene();
    void RenderCustomers();
    
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    TTF_Font* font_ = nullptr;
    TTF_Font* fontLarge_ = nullptr;
    
    bool running_ = false;
    int width_ = 1920;
    int height_ = 1080;
    
    // 游戏数据
    PlayerData playerData_;
    
    // 系统指针
    std::shared_ptr<HUD> hud_;
    std::unique_ptr<CustomerManager> customerManager_;
};

} // namespace BikeShopTycoon