cmake_minimum_required(VERSION 3.10)
project(BikeShopTycoon VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# SDL2 依赖
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

# 源文件
set(SOURCES
    src/main.cpp
    
    # Core
    src/core/Game.cpp
    src/core/SaveSystem.cpp
    src/core/TimeManager.cpp
    
    # Data
    src/data/ProductDatabase.cpp
    
    # Systems
    src/systems/CustomerManager.cpp
    src/systems/InventoryManager.cpp
    src/systems/RepairService.cpp
    src/systems/EventSystem.cpp
    
    # UI
    src/ui/UIManager.cpp
    src/ui/HUD.cpp
    src/ui/ShopPanel.cpp
    src/ui/BottomNav.cpp
)

# 可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

# 资源目录（如果存在）
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets 
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

# 安装
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY assets/ DESTINATION share/${PROJECT_NAME}/assets)