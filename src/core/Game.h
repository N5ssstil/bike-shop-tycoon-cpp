#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>

namespace BikeShopTycoon {

class Game {
public:
    static Game& Instance();
    
    bool Init(const std::string& title, int width, int height);
    void Run();
    void Quit();
    
    // 渲染器访问
    SDL_Renderer* GetRenderer() const { return renderer_; }
    SDL_Window* GetWindow() const { return window_; }
    
    // 游戏状态
    bool IsRunning() const { return running_; }
    void SetRunning(bool running) { running_ = running; }
    
    // 窗口尺寸
    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }
    
private:
    Game() = default;
    ~Game();
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    
    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    
    void CleanUp();
    
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    TTF_Font* font_ = nullptr;
    
    bool running_ = false;
    int width_ = 1920;
    int height_ = 1080;
};

} // namespace BikeShopTycoon