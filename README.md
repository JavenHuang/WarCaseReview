# 战例复盘分析系统

基于Qt5.12.0开发的战例复盘分析系统，支持战役和战术战例的态势复盘分析。

## 功能特性

- **战例选择**：支持选择10个经典战例（5个战役战例，5个战术战例）
- **态势复盘**：实现2、4、8、16、32五种推进加速比
- **精度级别**：
  - 战役复盘精确到旅/团（营）
  - 战术复盘精确到排（班）
- **复盘内容**：
  - 初始态势
  - 战场环境
  - 作战过程
  - 阶段划分
  - 战损战果
  - 战法运用
  - 装备使用
- **地图显示**：
  - 矢量地图支持
  - 加载GeoJSON格式地图数据
  - 无损缩放和平移
- **数据可视化**：
  - 文字形式说明战例事件最终情况
  - 图表形式展示作战结果
  - 人员、装备战损情况及战损比

## 项目结构

```
WarCaseReview/
├── CMakeLists.txt          # CMake构建文件
├── WarCaseReview.pro        # Qt项目文件
├── src/                    # 源代码目录
│   ├── main.cpp            # 程序入口
│   ├── MainWindow.h/cpp    # 主窗口
│   ├── WarCaseData.h/cpp   # 战例数据管理
│   ├── MapWidget.h/cpp     # 地图显示组件
│   ├── ReplayController.h/cpp  # 复盘回放控制
│   ├── StatisticsWidget.h/cpp  # 统计图表组件
│   └── WarCaseInfoWidget.h/cpp # 战例信息展示组件
├── ui/                     # UI文件目录
│   └── MainWindow.ui       # 主窗口UI设计文件
├── resources/              # 资源文件目录
│   └── resources.qrc       # 资源文件
└── data/                   # 数据文件目录
    ├── 中国_省.geojson     # 省级地图数据
    ├── 中国_市.geojson     # 市级地图数据
    └── 中国_县.geojson     # 县级地图数据
```

## 构建要求

- Qt 5.12.0 或更高版本
- CMake 3.16 或更高版本（使用CMake构建时）
- C++17 编译器
- Qt模块：
  - Qt5::Core
  - Qt5::Widgets
  - Qt5::Charts
  - Qt5::Gui

## 构建方法

### 使用CMake构建

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 使用qmake构建

```bash
qmake WarCaseReview.pro
make  # 或 nmake (Windows) 或 mingw32-make (MinGW)
```

## 运行

构建完成后，可执行文件位于 `bin/` 目录（CMake）或项目根目录（qmake）。

## 使用说明

1. **选择战例类型**：在左侧面板选择"战役战例"或"战术战例"
2. **选择具体战例**：从下拉列表中选择要分析的战例
3. **控制回放**：
   - 点击"播放"开始复盘回放
   - 使用速度滑块调整播放速度（2、4、8、16、32倍速）
   - 使用进度滑块跳转到指定时间点
   - 点击"停止"停止回放
4. **查看信息**：
   - 地图区域显示单位位置和移动轨迹
   - 右侧信息面板显示战例详细信息和当前态势
   - 统计面板显示战损战果图表

## 地图数据

系统支持加载GeoJSON格式的矢量地图数据。默认加载 `data/中国_省.geojson` 文件。

地图操作：
- **缩放**：鼠标滚轮
- **平移**：Ctrl+鼠标左键拖拽 或 中键拖拽

## 开发说明

- UI界面使用Qt Designer设计（.ui文件格式）
- 地图渲染使用QGraphicsView/QGraphicsScene
- 图表展示使用Qt Charts模块
- 战例数据存储在内存中，可扩展为从文件或数据库加载

## 许可证

本项目仅供学习和研究使用。

