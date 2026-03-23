# Windows Visual Studio 编译指南

> 最后更新: 2026-03-24

## 环境要求

| 工具 | 版本 | 说明 |
|------|------|------|
| Visual Studio | 2022 | 需要包含"使用 C++ 的桌面开发"工作负载 |
| CMake | 3.10+ | VS 2022 内置 |
| vcpkg | 最新 | C++ 包管理器 |
| Git | 最新 | 用于克隆项目 |

---

## 第一步：安装 Visual Studio 2022

1. 下载 [Visual Studio 2022 Community](https://visualstudio.microsoft.com/downloads/)（免费）
2. 安装时选择工作负载：**"使用 C++ 的桌面开发"**
3. 确保勾选：
   - MSVC v143 生成工具
   - Windows 10/11 SDK
   - CMake 工具

---

## 第二步：安装 vcpkg

打开 **PowerShell（管理员模式）**：

```powershell
# 克隆 vcpkg 到 C 盘
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# 运行引导脚本
.\bootstrap-vcpkg.bat

# 集成到系统（重要！）
.\vcpkg integrate install
```

---

## 第三步：安装 SDL2 依赖

```powershell
# 在 vcpkg 目录下运行（C:\vcpkg）
.\vcpkg install sdl2:x64-windows
.\vcpkg install sdl2-image:x64-windows
.\vcpkg install sdl2-ttf:x64-windows
.\vcpkg install sdl2-mixer:x64-windows
```

⏳ 等待安装完成（首次约 10-15 分钟）

---

## 第四步：克隆项目

```powershell
# 进入你的开发目录
cd C:\dev
# 或者 D:\projects 等

# 克隆项目
git clone https://github.com/N5ssstil/bike-shop-tycoon-cpp.git
cd bike-shop-tycoon-cpp
```

---

## 第五步：编译项目

### 方法 A：命令行编译（推荐）

```powershell
# 在项目根目录下
mkdir build
cd build

# 生成项目（注意替换你的 vcpkg 路径）
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake

# 编译
cmake --build . --config Release
```

### 方法 B：Visual Studio GUI

1. 打开 Visual Studio 2022
2. 选择 **"打开本地文件夹"**
3. 选择 `bike-shop-tycoon-cpp` 目录
4. 等待 CMake 自动配置（底部会显示进度）
5. 菜单：**生成 → 全部生成** (或按 Ctrl+Shift+B)

---

## 第六步：运行游戏

### 从命令行运行

```powershell
cd build\Release
.\BikeShopTycoon.exe
```

### 从 Visual Studio 运行

1. 确保 `BikeShopTycoon.exe` 是启动项目
2. 按 **F5** 或点击绿色播放按钮 ▶️

---

## 资源文件说明

项目已经包含必要的资源文件：

```
assets/
├── fonts/
│   ├── arial.ttf        # ✅ 已包含
│   └── arial_bold.ttf   # ✅ 已包含
├── data/
│   ├── products.json    # ✅ 商品数据
│   └── events.json      # ✅ 事件数据
└── images/              # 📁 待添加图片
```

---

## 快速启动脚本

在项目根目录创建 `build.bat`，双击即可编译：

```batch
@echo off
echo === Bike Shop Tycoon Build Script ===
echo.

if not exist build mkdir build
cd build

echo [1/2] Configuring CMake...
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo [2/2] Building Release...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build successful!
echo Executable: build\Release\BikeShopTycoon.exe
echo ========================================
pause
```

---

## 常见问题

### Q: CMake 找不到 SDL2

```powershell
# 确保重新集成 vcpkg
cd C:\vcpkg
.\vcpkg integrate install

# 或者设置环境变量
$env:VCPKG_ROOT = "C:\vcpkg"
```

### Q: 链接错误 LNK2019

确保安装的是 **x64** 版本：
```powershell
.\vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows sdl2-mixer:x64-windows
```

### Q: 运行时找不到 DLL

两种解决方案：

1. **复制 DLL 到 exe 目录**：
```powershell
# 编译完成后，vcpkg 会自动复制 DLL
# 如果没有，手动从 vcpkg 复制
copy C:\vcpkg\installed\x64-windows\bin\*.dll build\Release\
```

2. **添加 vcpkg bin 到 PATH**：
```powershell
$env:PATH += ";C:\vcpkg\installed\x64-windows\bin"
```

### Q: 字体加载失败

项目已包含字体文件。如果仍有问题：
- 检查 `assets/fonts/arial.ttf` 是否存在
- 或使用绝对路径测试

---

## 项目结构

```
bike-shop-tycoon-cpp/
├── CMakeLists.txt      # CMake 配置
├── build.bat           # 编译脚本（自己创建）
├── src/
│   ├── main.cpp        # 程序入口
│   ├── core/           # 核心系统 (Game, TimeManager, SaveSystem)
│   ├── data/           # 数据层 (Types, ProductDatabase)
│   ├── systems/        # 游戏系统 (Customer, Inventory, Repair)
│   └── ui/             # UI 层 (HUD, Panels)
├── assets/
│   ├── fonts/          # ✅ 字体文件
│   ├── images/         # 📁 图片资源
│   └── data/           # ✅ 数据文件
└── docs/               # 文档
```

---

## 下一步

编译成功后，你可以：

1. **运行游戏** - 查看当前界面
2. **添加图片资源** - 放入 `assets/images/`
3. **继续开发** - 完善顾客系统、库存系统等

有问题随时问我！🐱🚲