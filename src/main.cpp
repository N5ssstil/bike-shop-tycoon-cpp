#include "core/Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << R"(
    ╔═══════════════════════════════════════════════════════╗
    ║                                                       ║
    ║         🚴  Bike Shop Tycoon  🚴                     ║
    ║                                                       ║
    ║              C++ Version                              ║
    ║                                                       ║
    ╚═══════════════════════════════════════════════════════╝
    )" << std::endl;
    
    auto& game = BikeShopTycoon::Game::Instance();
    
    if (!game.Init("Bike Shop Tycoon", 1920, 1080)) {
        std::cerr << "游戏初始化失败!" << std::endl;
        return 1;
    }
    
    game.Run();
    
    return 0;
}