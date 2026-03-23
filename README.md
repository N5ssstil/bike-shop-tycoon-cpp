# Bike Shop Tycoon - C++ 重构计划

## 项目概述

从 Unity + C# 重构为 **C++ + SDL2**，保留游戏设计，重新实现核心功能。

---

## 技术选型

| 组件 | 选择 | 原因 |
|------|------|------|
| 语言 | C++17 | 现代特性，跨平台 |
| 窗口/渲染 | SDL2 | 轻量、跨平台、2D友好 |
| 图像加载 | SDL2_image | PNG/JPG支持 |
| 字体渲染 | SDL2_ttf | TrueType字体 |
| 音频 | SDL2_mixer | 音效/音乐 |
| 构建系统 | CMake | 跨平台构建 |
| JSON解析 | nlohmann/json | 头文件库，易用 |

---

## 项目结构

```
bike-shop-tycoon-cpp/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── Game.cpp
│   │   ├── Game.h
│   │   ├── SaveSystem.cpp
│   │   ├── SaveSystem.h
│   │   ├── TimeManager.cpp
│   │   ├── TimeManager.h
│   │   └── Settings.h
│   ├── data/
│   │   ├── PlayerData.h
│   │   ├── ItemData.h
│   │   ├── CustomerData.h
│   │   ├── InventoryData.h
│   │   └── ProductDatabase.cpp
│   ├── systems/
│   │   ├── CustomerManager.cpp
│   │   ├── CustomerManager.h
│   │   ├── InventoryManager.cpp
│   │   ├── InventoryManager.h
│   │   ├── RepairService.cpp
│   │   ├── RepairService.h
│   │   ├── EventSystem.cpp
│   │   ├── EventSystem.h
│   │   └── AchievementSystem.h
│   ├── ui/
│   │   ├── UIManager.cpp
│   │   ├── UIManager.h
│   │   ├── HUD.cpp
│   │   ├── HUD.h
│   │   ├── ShopPanel.cpp
│   │   ├── ShopPanel.h
│   │   ├── InventoryPanel.cpp
│   │   └── InventoryPanel.h
│   └── utils/
│       ├── JsonHelper.h
│       └── Random.h
├── assets/
│   ├── fonts/
│   ├── images/
│   └── data/
│       ├── products.json
│       └── events.json
├── external/
│   └── nlohmann/
└── README.md
```

---

## 开发阶段

### Phase 1: 基础框架 (当前)
- [ ] 项目结构搭建
- [ ] SDL2 初始化
- [ ] 游戏主循环
- [ ] 基础渲染

### Phase 2: 数据层
- [ ] 数据结构定义
- [ ] JSON 加载/保存
- [ ] 商品数据库

### Phase 3: 核心系统
- [ ] 顾客管理系统
- [ ] 库存管理系统
- [ ] 维修服务系统

### Phase 4: UI 层
- [ ] HUD 显示
- [ ] 店铺界面
- [ ] 交互响应

### Phase 5: 游戏循环
- [ ] 时间推进
- [ ] 存档系统
- [ ] 成就系统

---

## 依赖安装

### Linux (Alibaba Cloud)
```bash
sudo yum install -y SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel
```

### 编译
```bash
mkdir build && cd build
cmake ..
make
./bike_shop_tycoon
```

---

*创建时间: 2026-03-23*