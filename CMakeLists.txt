cmake_minimum_required(VERSION 3.10)
project(BikeShopTycoon VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 编译选项
if(MSVC)
    # Windows MSVC 编译选项
    add_compile_options(/W3 /utf-8)
else()
    # Linux/GCC 编译选项
    add_compile_options(-Wall -Wextra)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# 查找 SDL2 依赖
# 在 Windows 上使用 vcpkg，在 Linux 上使用 pkg-config
if(WIN32)
    # Windows: 使用 vcpkg 提供的 CMake 配置
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    
    # 设置 SDL2 库变量（vcpkg 方式）
    set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)
    set(SDL2_IMAGE_LIBRARIES SDL2_image::SDL2_image)
    set(SDL2_TTF_LIBRARIES SDL2_ttf::SDL2_ttf)
    set(SDL2_MIXER_LIBRARIES SDL2_mixer::SDL2_mixer)
else()
    # Linux: 使用 pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
endif()

# 源文件
set(SOURCES
    src/main.cpp
    
    # Core
    src/core/Game.cpp
    src/core/SaveSystem.cpp
    src/core/TimeManager.cpp
    src/core/AudioManager.cpp
    
    # Data
    src/data/ProductDatabase.cpp
    
    # Systems
    src/systems/CustomerManager.cpp
    src/systems/InventoryManager.cpp
    src/systems/RepairService.cpp
    src/systems/EventSystem.cpp
    src/systems/AchievementSystem.cpp
    
    # UI
    src/ui/UIManager.cpp
    src/ui/HUD.cpp
    src/ui/ShopPanel.cpp
    src/ui/BottomNav.cpp
    src/ui/InventoryPanel.cpp
    src/ui/RepairPanel.cpp
    src/ui/MainMenu.cpp
    src/ui/CustomerDetailPanel.cpp
)

# 可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

# Windows 特定设置
if(WIN32)
    # 设置为 Windows 应用程序（有控制台窗口）
    # 如果想隐藏控制台窗口，使用 WIN32：
    # add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
    
    # 复制 DLL 文件到输出目录
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    )
endif()

# 资源目录（如果存在）
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets 
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

# 安装
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY assets/ DESTINATION share/${PROJECT_NAME}/assets)