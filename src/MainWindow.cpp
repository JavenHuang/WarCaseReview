#include "MainWindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentCaseType(0)
    , m_currentCaseIndex(0)
    , m_isPlaying(false)
{
    ui->setupUi(this);
    setupUI();
    setupMenuBar();
    
    m_warCaseData = new WarCaseData(this);
    m_replayController = new ReplayController(this);
    m_replayController->setParent(this);
    
    connect(m_replayController, &ReplayController::timeChanged, this, &MainWindow::updateReplayDisplay);
    connect(m_replayController, &ReplayController::progressChanged, this, &MainWindow::onReplayProgressChanged);
    connect(m_replayController, &ReplayController::finished, this, &MainWindow::onReplayFinished);
    connect(m_replayController, &ReplayController::unitsUpdated, m_mapWidget, &MapWidget::updateUnits);
    
    loadWarCases();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // 连接UI信号
    connect(ui->caseTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onCaseTypeChanged);
    connect(ui->caseCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onCaseSelected);
    connect(ui->playPauseBtn, &QPushButton::clicked, this, &MainWindow::onPlayPauseClicked);
    connect(ui->stopBtn, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(ui->speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedChanged);
    connect(ui->progressSlider, &QSlider::valueChanged, this, &MainWindow::onReplayProgressChanged);
    
    // 获取地图组件 - 由于是自定义widget，需要手动创建
    QWidget *mapPanel = ui->mainSplitter->widget(1);
    if (mapPanel) {
        QVBoxLayout *mapLayout = qobject_cast<QVBoxLayout*>(mapPanel->layout());
        if (!mapLayout) {
            mapLayout = new QVBoxLayout(mapPanel);
            mapLayout->setContentsMargins(0, 0, 0, 0);
        }
        m_mapWidget = new MapWidget(mapPanel);
        mapLayout->addWidget(m_mapWidget);
    } else {
        m_mapWidget = new MapWidget(this);
    }
    
    // 加载默认地图
    if (m_mapWidget) {
        m_mapWidget->loadGeoJson("data/中国_省.geojson");
    }
    
    // 获取信息组件 - 从右侧分割器中获取
    QSplitter *rightSplitter = qobject_cast<QSplitter*>(ui->mainSplitter->widget(2));
    if (rightSplitter) {
        m_infoWidget = qobject_cast<WarCaseInfoWidget*>(rightSplitter->widget(0));
        m_statisticsWidget = qobject_cast<StatisticsWidget*>(rightSplitter->widget(1));
        
        // 如果组件不存在，创建它们
        if (!m_infoWidget) {
            m_infoWidget = new WarCaseInfoWidget();
            rightSplitter->insertWidget(0, m_infoWidget);
        }
        if (!m_statisticsWidget) {
            m_statisticsWidget = new StatisticsWidget();
            rightSplitter->insertWidget(1, m_statisticsWidget);
        }
        
        // 设置右侧分割器比例
        rightSplitter->setStretchFactor(0, 1);
        rightSplitter->setStretchFactor(1, 1);
    } else {
        // 如果右侧分割器不存在，创建组件
        m_infoWidget = new WarCaseInfoWidget(this);
        m_statisticsWidget = new StatisticsWidget(this);
    }
    
    // 设置主分割器比例
    ui->mainSplitter->setStretchFactor(0, 0);
    ui->mainSplitter->setStretchFactor(1, 2);
    ui->mainSplitter->setStretchFactor(2, 1);
}

void MainWindow::setupMenuBar()
{
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "关于", 
            "战例复盘分析系统 v1.0.0\n\n"
            "支持战役和战术战例的态势复盘分析。");
    });
}

void MainWindow::loadWarCases()
{
    if (m_warCaseData->loadWarCases()) {
        onCaseTypeChanged(0);
    }
}

void MainWindow::onCaseTypeChanged(int index)
{
    m_currentCaseType = index;
    ui->caseCombo->clear();
    
    QList<WarCase> cases = (index == 0) ? 
        m_warCaseData->getCampaignCases() : 
        m_warCaseData->getTacticalCases();
    
    for (const auto &warCase : cases) {
        ui->caseCombo->addItem(warCase.name);
    }
    
    if (ui->caseCombo->count() > 0) {
        ui->caseCombo->setCurrentIndex(0);
        onCaseSelected(0);
    }
}

void MainWindow::onCaseSelected(int index)
{
    m_currentCaseIndex = index;
    m_replayController->setCurrentCase(m_currentCaseType, m_currentCaseIndex);
    
    WarCase warCase = m_warCaseData->getCase(m_currentCaseType, m_currentCaseIndex);
    m_infoWidget->updateInfo(warCase);
    
    // 设置地图范围（根据战例单位位置）
    if (!warCase.units.isEmpty()) {
        double minLon = warCase.units[0].longitude;
        double maxLon = warCase.units[0].longitude;
        double minLat = warCase.units[0].latitude;
        double maxLat = warCase.units[0].latitude;
        
        for (const auto &unit : warCase.units) {
            minLon = qMin(minLon, unit.longitude);
            maxLon = qMax(maxLon, unit.longitude);
            minLat = qMin(minLat, unit.latitude);
            maxLat = qMax(maxLat, unit.latitude);
        }
        
        m_mapWidget->setMapBounds(minLon, minLat, maxLon, maxLat);
    }
    
    updateCaseInfo();
}

void MainWindow::onPlayPauseClicked()
{
    if (m_isPlaying) {
        m_replayController->pause();
        ui->playPauseBtn->setText("播放");
        m_isPlaying = false;
    } else {
        m_replayController->play();
        ui->playPauseBtn->setText("暂停");
        m_isPlaying = true;
    }
}

void MainWindow::onStopClicked()
{
    m_replayController->stop();
    ui->playPauseBtn->setText("播放");
    m_isPlaying = false;
    ui->progressSlider->setValue(0);
}

void MainWindow::onSpeedChanged(int value)
{
    int speedRatios[] = {2, 4, 8, 16, 32};
    int speedRatio = speedRatios[value];
    m_replayController->setSpeed(speedRatio);
    ui->speedLabel->setText(QString("播放速度：%1倍").arg(speedRatio));
}

void MainWindow::onReplayProgressChanged(int progress)
{
    if (ui->progressSlider->value() != progress) {
        ui->progressSlider->setValue(progress);
    }
    ui->progressLabel->setText(QString("进度：%1%").arg(progress));
    
    if (sender() == ui->progressSlider) {
        m_replayController->seek(progress);
    }
}

void MainWindow::onReplayFinished()
{
    ui->playPauseBtn->setText("播放");
    m_isPlaying = false;
    ui->progressSlider->setValue(100);
}

void MainWindow::updateReplayDisplay()
{
    QDateTime currentTime = m_replayController->getCurrentTime();
    WarCase warCase = m_warCaseData->getCase(m_currentCaseType, m_currentCaseIndex);
    
    QList<Unit> units = m_warCaseData->getUnitsAtTime(m_currentCaseType, m_currentCaseIndex, currentTime);
    Casualty casualty = m_warCaseData->getCasualtiesAtTime(m_currentCaseType, m_currentCaseIndex, currentTime);
    QList<Tactics> tactics = m_warCaseData->getTacticsAtTime(m_currentCaseType, m_currentCaseIndex, currentTime);
    
    m_mapWidget->updateUnits(units);
    m_statisticsWidget->updateStatistics(casualty, units);
    m_infoWidget->updateCurrentSituation(currentTime, tactics, casualty);
}

void MainWindow::updateCaseInfo()
{
    WarCase warCase = m_warCaseData->getCase(m_currentCaseType, m_currentCaseIndex);
    m_infoWidget->updateInfo(warCase);
}

void MainWindow::updateStatistics()
{
    WarCase warCase = m_warCaseData->getCase(m_currentCaseType, m_currentCaseIndex);
    if (!warCase.casualties.isEmpty()) {
        m_statisticsWidget->updateStatistics(warCase.casualties.last(), warCase.units);
    }
}

