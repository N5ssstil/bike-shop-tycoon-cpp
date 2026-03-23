# Windows Visual Studio 编译指南

## 环境要求

| 工具 | 版本 | 说明 |
|------|------|------|
| Visual Studio | 2022 | 需要包含"使用 C++ 的桌面开发"工作负载 |
| CMake | 3.10+ | VS 2022 内置 |
| vcpkg | 最新 | C++ 包管理器 |

---

## 第一步：安装 Visual Studio 2022

1. 下载 [Visual Studio 2022 Community](https://visualstudio.microsoft.com/downloads/)（免费）
2. 安装时选择工作负载：**"使用 C++ 的桌面开发"**
3. 确保勾选：
   - MSVC v143 生成工具
   - Windows 10 SDK
   - CMake 工具

---

## 第二步：安装 vcpkg

```powershell
# 打开 PowerShell（管理员模式）

# 克隆 vcpkg
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# 运行引导脚本
.\bootstrap-vcpkg.bat

# 集成到系统
.\vcpkg integrate install
```

---

## 第三步：安装 SDL2 依赖

```powershell
# 在 vcpkg 目录下运行
.\vcpkg install sdl2:x64-windows
.\vcpkg install sdl2-image:x64-windows
.\vcpkg install sdl2-ttf:x64-windows
.\vcpkg install sdl2-mixer:x64-windows
```

等待安装完成（约 5-10 分钟）

---

## 第四步：克隆项目

```powershell
# 进入你的开发目录
cd C:\dev

# 克隆项目
git clone https://github.com/N5ssstil/bike-shop-tycoon-cpp.git
cd bike-shop-tycoon-cpp
```

---

## 第五步：使用 CMake 生成项目

### 方法 A：使用命令行

```powershell
# 创建 build 目录
mkdir build
cd build

# 生成 Visual Studio 项目
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake

# 编译
cmake --build . --config Release
```

### 方法 B：使用 Visual Studio GUI

1. 打开 Visual Studio 2022
2. 选择 **"打开本地文件夹"**
3. 选择 `bike-shop-tycoon-cpp` 目录
4. 等待 CMake 自动配置
5. 顶部菜单：**生成 → 全部生成**

---

## 第六步：配置 CMake 设置（GUI方式）

如果 CMake 配置失败，手动创建 `CMakePresets.json`：

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "windows-default",
      "generator": "Visual Studio 17 2022",
      "binaryDir": "${sourceDir}/build",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ]
}
```

---

## 第七步：运行项目

### 从 Visual Studio 运行

1. 在解决方案资源管理器中找到 `BikeShopTycoon.exe`
2. 右键 → **设为启动项目**
3. 按 **F5** 或点击绿色播放按钮

### 从命令行运行

```powershell
cd build\Release
.\BikeShopTycoon.exe
```

---

## 常见问题

### Q: 找不到 SDL2 库

确保 vcpkg 集成成功：
```powershell
.\vcpkg integrate install
```

### Q: 字体显示问题

项目需要一个字体文件。在 `assets/fonts/` 目录放置一个 `.ttf` 字体文件：
```
assets/fonts/arial.ttf
```

或使用系统字体：
```
C:\Windows\Fonts\arial.ttf
```

### Q: CMake 找不到 vcpkg

设置环境变量：
```powershell
$env:VCPKG_ROOT = "C:\vcpkg"
```

或在 CMake 命令中指定完整路径：
```powershell
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
```

### Q: 链接错误

确保安装了 x64 版本的库：
```powershell
.\vcpkg install sdl2:x64-windows
```

---

## 项目结构

```
bike-shop-tycoon-cpp/
├── CMakeLists.txt      # CMake 配置
├── src/
│   ├── main.cpp        # 入口
│   ├── core/           # 核心系统
│   ├── data/           # 数据层
│   ├── systems/        # 游戏系统
│   └── ui/             # UI 层
├── assets/             # 资源文件
│   ├── fonts/          # 字体（需要手动添加）
│   ├── images/         # 图片
│   └── data/           # 数据文件
└── build/              # 编译输出
```

---

## 快速启动脚本

创建 `build.bat`：

```batch
@echo off
mkdir build 2>nul
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
echo.
echo Build complete! Executable: build\Release\BikeShopTycoon.exe
pause
```

双击即可编译。

---

## 下一步

- 添加字体文件到 `assets/fonts/`
- 添加图片资源
- 开始游戏！

---

有问题随时问。