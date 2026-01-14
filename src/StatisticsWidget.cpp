#include "StatisticsWidget.h"
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QGridLayout>

StatisticsWidget::StatisticsWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void StatisticsWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(10);
    
    // 文字统计标签
    m_personnelLabel = new QLabel("人员：0", this);
    m_personnelLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    m_equipmentLabel = new QLabel("装备：0", this);
    m_equipmentLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    m_casualtyRatioLabel = new QLabel("战损比：0%", this);
    m_casualtyRatioLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: red;");
    
    m_mainLayout->addWidget(m_personnelLabel);
    m_mainLayout->addWidget(m_equipmentLabel);
    m_mainLayout->addWidget(m_casualtyRatioLabel);
    
    // 人员战损饼图
    m_personnelChart = new QChart();
    m_personnelChart->setTitle("人员战损情况");
    m_personnelChart->legend()->setVisible(true);
    m_personnelChart->legend()->setAlignment(Qt::AlignBottom);
    
    m_personnelSeries = new QPieSeries();
    m_personnelChart->addSeries(m_personnelSeries);
    
    m_personnelChartView = new QChartView(m_personnelChart, this);
    m_personnelChartView->setRenderHint(QPainter::Antialiasing);
    m_personnelChartView->setMinimumHeight(150);
    m_mainLayout->addWidget(m_personnelChartView);
    
    // 装备战损柱状图
    m_equipmentChart = new QChart();
    m_equipmentChart->setTitle("装备战损情况");
    m_equipmentChart->legend()->setVisible(true);
    m_equipmentChart->legend()->setAlignment(Qt::AlignBottom);
    
    m_equipmentSeries = new QBarSeries();
    m_equipmentChart->addSeries(m_equipmentSeries);
    
    QBarCategoryAxis *equipmentAxisX = new QBarCategoryAxis();
    m_equipmentChart->addAxis(equipmentAxisX, Qt::AlignBottom);
    m_equipmentSeries->attachAxis(equipmentAxisX);
    
    QValueAxis *equipmentAxisY = new QValueAxis();
    m_equipmentChart->addAxis(equipmentAxisY, Qt::AlignLeft);
    m_equipmentSeries->attachAxis(equipmentAxisY);
    
    m_equipmentChartView = new QChartView(m_equipmentChart, this);
    m_equipmentChartView->setRenderHint(QPainter::Antialiasing);
    m_equipmentChartView->setMinimumHeight(150);
    m_mainLayout->addWidget(m_equipmentChartView);
    
    // 战损比趋势图
    m_casualtyRatioChart = new QChart();
    m_casualtyRatioChart->setTitle("战损比趋势");
    m_casualtyRatioChart->legend()->setVisible(true);
    m_casualtyRatioChart->legend()->setAlignment(Qt::AlignBottom);
    
    m_casualtyRatioSeries = new QBarSeries();
    m_casualtyRatioChart->addSeries(m_casualtyRatioSeries);
    
    QBarCategoryAxis *ratioAxisX = new QBarCategoryAxis();
    m_casualtyRatioChart->addAxis(ratioAxisX, Qt::AlignBottom);
    m_casualtyRatioSeries->attachAxis(ratioAxisX);
    
    QValueAxis *ratioAxisY = new QValueAxis();
    ratioAxisY->setRange(0, 100);
    ratioAxisY->setTitleText("百分比 (%)");
    m_casualtyRatioChart->addAxis(ratioAxisY, Qt::AlignLeft);
    m_casualtyRatioSeries->attachAxis(ratioAxisY);
    
    m_casualtyRatioChartView = new QChartView(m_casualtyRatioChart, this);
    m_casualtyRatioChartView->setRenderHint(QPainter::Antialiasing);
    m_casualtyRatioChartView->setMinimumHeight(150);
    m_mainLayout->addWidget(m_casualtyRatioChartView);
    
    m_mainLayout->addStretch();
}

void StatisticsWidget::updateStatistics(const Casualty &casualty, const QList<Unit> &units)
{
    m_currentCasualty = casualty;
    m_currentUnits = units;
    
    // 更新文字统计
    m_personnelLabel->setText(QString("人员战损：%1 人").arg(casualty.personnel));
    m_equipmentLabel->setText(QString("装备战损：%1 件").arg(casualty.equipment));
    m_casualtyRatioLabel->setText(QString("战损比：%1%").arg(casualty.casualtyRatio * 100, 0, 'f', 2));
    
    updateCharts();
}

void StatisticsWidget::updateCharts()
{
    // 更新人员战损饼图
    m_personnelSeries->clear();
    
    int totalPersonnel = 0;
    for (const Unit &unit : m_currentUnits) {
        totalPersonnel += unit.personnel;
    }
    
    if (totalPersonnel > 0) {
        int remaining = totalPersonnel - m_currentCasualty.personnel;
        if (remaining > 0) {
            m_personnelSeries->append("剩余人员", remaining);
        }
        if (m_currentCasualty.personnel > 0) {
            m_personnelSeries->append("战损人员", m_currentCasualty.personnel);
        }
    }
    
    // 更新装备战损柱状图
    m_equipmentSeries->clear();
    
    QBarSet *equipmentSet = new QBarSet("装备");
    int totalEquipment = 0;
    for (const Unit &unit : m_currentUnits) {
        totalEquipment += unit.equipment;
    }
    
    if (totalEquipment > 0) {
        int remaining = totalEquipment - m_currentCasualty.equipment;
        equipmentSet->append(remaining);
        equipmentSet->append(m_currentCasualty.equipment);
        m_equipmentSeries->append(equipmentSet);
        
        QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis*>(m_equipmentChart->axes(Qt::Horizontal).first());
        if (axisX) {
            axisX->clear();
            axisX->append("剩余装备");
            axisX->append("战损装备");
        }
    }
    
    // 更新战损比柱状图
    m_casualtyRatioSeries->clear();
    
    QBarSet *ratioSet = new QBarSet("战损比");
    ratioSet->append(m_currentCasualty.casualtyRatio * 100);
    m_casualtyRatioSeries->append(ratioSet);
    
    QBarCategoryAxis *ratioAxisX = qobject_cast<QBarCategoryAxis*>(m_casualtyRatioChart->axes(Qt::Horizontal).first());
    if (ratioAxisX) {
        ratioAxisX->clear();
        ratioAxisX->append("当前战损比");
    }
}

