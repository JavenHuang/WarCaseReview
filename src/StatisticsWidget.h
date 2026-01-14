#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "WarCaseData.h"

QT_CHARTS_USE_NAMESPACE

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(QWidget *parent = nullptr);
    
    // 更新统计数据
    void updateStatistics(const Casualty &casualty, const QList<Unit> &units);

private:
    void setupUI();
    void updateCharts();
    
    QVBoxLayout *m_mainLayout;
    
    // 文字统计
    QLabel *m_personnelLabel;
    QLabel *m_equipmentLabel;
    QLabel *m_casualtyRatioLabel;
    
    // 图表
    QChartView *m_personnelChartView;
    QChartView *m_equipmentChartView;
    QChartView *m_casualtyRatioChartView;
    
    QChart *m_personnelChart;
    QChart *m_equipmentChart;
    QChart *m_casualtyRatioChart;
    
    QPieSeries *m_personnelSeries;
    QBarSeries *m_equipmentSeries;
    QBarSeries *m_casualtyRatioSeries;
    
    Casualty m_currentCasualty;
    QList<Unit> m_currentUnits;
};

#endif // STATISTICSWIDGET_H

