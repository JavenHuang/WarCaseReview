cmake_minimum_required(VERSION 3.16)
project(WarCaseReview VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt5组件
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts Gui)

# 启用自动处理Qt的MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 源文件
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/WarCaseData.cpp
    src/MapWidget.cpp
    src/ReplayController.cpp
    src/StatisticsWidget.cpp
    src/WarCaseInfoWidget.cpp
)

# 头文件
set(HEADERS
    src/MainWindow.h
    src/WarCaseData.h
    src/MapWidget.h
    src/ReplayController.h
    src/StatisticsWidget.h
    src/WarCaseInfoWidget.h
)

# UI文件
set(UI_FILES
    ui/MainWindow.ui
)

# 资源文件
set(RESOURCES
    resources/resources.qrc
)

# 创建可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCES}
)

# 链接Qt库
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    Qt5::Gui
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
