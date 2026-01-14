#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include "ui_MainWindow.h"
#include "MapWidget.h"
#include "WarCaseInfoWidget.h"
#include "StatisticsWidget.h"
#include "ReplayController.h"
#include "WarCaseData.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCaseTypeChanged(int index);
    void onCaseSelected(int index);
    void onPlayPauseClicked();
    void onStopClicked();
    void onSpeedChanged(int value);
    void onReplayProgressChanged(int progress);
    void onReplayFinished();
    void updateReplayDisplay();

private:
    void setupUI();
    void setupMenuBar();
    void loadWarCases();
    void updateCaseInfo();
    void updateStatistics();

    Ui::MainWindow *ui;
    
    // 中间地图区域
    MapWidget *m_mapWidget;
    
    // 右侧信息面板
    WarCaseInfoWidget *m_infoWidget;
    StatisticsWidget *m_statisticsWidget;
    
    // 数据和控制
    WarCaseData *m_warCaseData;
    ReplayController *m_replayController;
    int m_currentCaseType; // 0: 战役, 1: 战术
    int m_currentCaseIndex;
    bool m_isPlaying;
};

#endif // MAINWINDOW_H

