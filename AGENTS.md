# AGENTS.md — NoPlugins (Qt Widgets Project)

> 北京大学 程序设计实习 Spring 2026 — QtForAI 分组项目
> 当前项目：**CarrotDefense - 塔防游戏**（CarrotDefense.exe）

## 构建系统（Qt6 + CMake + MSVC）

- **构建工具**: CMake 3.30 + NMake
- **编译器**: MSVC 19.50（VS 2022 Community 路径 `D:\Program Files\Microsoft Visual Studio\18\Community\`）
- **Qt 版本**: Qt 6.11.0（路径 `D:\Qt\6.11.0\msvc2022_64\`）
- **CMake 路径**: `D:\Qt\Tools\CMake_64\bin\cmake.exe`
- **C++ 标准**: C++17（`CMAKE_CXX_STANDARD 17`）
- **windeployqt**: `D:\Qt\6.11.0\msvc2022_64\bin\windeployqt.exe`

### 一键构建（必须严格遵守）

```powershell
$sandbox = "D:\Study\2026_Spring\程序设计实习\QtForAI\NoPlugins\sandbox_for_agent"
$vsPath = "D:\Program Files\Microsoft Visual Studio\18\Community"

# 创建 batch 文件（ASCII 编码）
@"
@echo off
call "$vsPath\VC\Auxiliary\Build\vcvars64.bat" > nul 2>&1
set PATH=D:\Qt\Tools\CMake_64\bin;%PATH%
rmdir /s /q build 2>nul
mkdir build
cd build
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
nmake
"@ | Set-Content "$sandbox\build_cmake.bat" -Encoding ASCII

# 执行
Set-Location $sandbox
cmd /c "build_cmake.bat"
```

**关键注意事项**：
- 必须设置 `-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY` 绕过 CMake 编译器链接测试失败
- 不要在包含中文的路径中执行 `cd /d`（cmd 编码问题），改用 PowerShell `Set-Location`
- windeployqt 已通过 CMake `POST_BUILD` 自动调用，构建后自动打包所有 DLL

### 重新编译

```powershell
Set-Location "$sandbox\build"
cmd /c "rebuild.bat"  # 内容：vcvars → set PATH → nmake
```

## CarrotDefense 项目结构

```
sandbox_for_agent/
├── CMakeLists.txt              # Qt6 CMake 构建（含 windeployqt POST_BUILD）
├── main.cpp                    # 入口：QApplication + MainWindow
├── mainwindow.h / .cpp         # QMainWindow：工具栏控制 + 状态栏
├── gamescene.h / .cpp          # 游戏核心：QPainter 渲染 + 60fps 游戏循环
├── enemy.h / .cpp              # 敌人：3 种类型，沿路点移动，血条
├── tower.h / .cpp              # 防御塔：3 种类型，寻敌 + 攻击
├── projectile.h / .cpp         # 弹射物：直线飞向目标，溅射/减速效果
├── wave.h / .cpp               # 波次管理：8 波递增难度
├── node.h                      # A* 节点结构体
├── pathfinder.h / .cpp         # A* 寻路（复用自 Pathfinding Visualizer）
├── AGENTS.md
└── build/
    ├── CarrotDefense.exe       # 最终产物
    ├── Qt6Core.dll + Qt6Gui.dll + Qt6Widgets.dll + ...
    └── *.dll（windeployqt 自动打包）
```

### 游戏功能

| 功能 | 说明 |
|---|---|
| 地图 | 15×12 网格，左边起点(S)，右边终点(E) |
| 塔防机制 | 在空格放置防御塔 → 阻挡敌人路径 → A* 自动重算路径 |
| 塔类型 | Arrow(50g/快/低伤), Cannon(100g/慢/溅射), Ice(75g/中/减速) |
| 敌人类型 | Normal(红), Fast(橙), Tank(紫) — 不同血量/速度/奖励 |
| 波次 | 8 波递增难度，每波完成后奖励金币 |
| 经济 | 起始 200g，杀敌+金币，放置塔-金币 |
| 生命 | 20 条命，敌人到达终点扣命 |
| 操作 | 点选塔 → 点空格放置；工具栏 Start/Pause |

### Qt6 模块依赖

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
```

### Qt5 兼容

本项目的 A* 寻路核心（node.h, pathfinder.h/cpp）为纯 C++，无 Qt 依赖，可直接在任何 Qt 版本或标准 C++ 项目中使用。
