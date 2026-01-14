QT += core widgets charts gui

CONFIG += c++17

TARGET = WarCaseReview
TEMPLATE = app

# 源文件
SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/WarCaseData.cpp \
    src/MapWidget.cpp \
    src/ReplayController.cpp \
    src/StatisticsWidget.cpp \
    src/WarCaseInfoWidget.cpp

# 头文件
HEADERS += \
    src/MainWindow.h \
    src/WarCaseData.h \
    src/MapWidget.h \
    src/ReplayController.h \
    src/StatisticsWidget.h \
    src/WarCaseInfoWidget.h

# UI文件
FORMS += \
    ui/MainWindow.ui

# 资源文件
RESOURCES += \
    resources/resources.qrc

# 输出目录
DESTDIR = $$PWD/bin
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
RCC_DIR = $$PWD/build/rcc
UI_DIR = $$PWD/build/ui

