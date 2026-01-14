#include "WarCaseInfoWidget.h"
#include <QScrollArea>
#include <QGroupBox>

WarCaseInfoWidget::WarCaseInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void WarCaseInfoWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(10);
    
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    m_contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(m_contentWidget);
    contentLayout->setSpacing(10);
    
    // 基本信息组
    QGroupBox *basicGroup = new QGroupBox("基本信息", m_contentWidget);
    QVBoxLayout *basicLayout = new QVBoxLayout(basicGroup);
    
    m_nameLabel = new QLabel("战例名称：", basicGroup);
    m_nameLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    m_typeLabel = new QLabel("战例类型：", basicGroup);
    m_timeLabel = new QLabel("时间：", basicGroup);
    
    basicLayout->addWidget(m_nameLabel);
    basicLayout->addWidget(m_typeLabel);
    basicLayout->addWidget(m_timeLabel);
    
    contentLayout->addWidget(basicGroup);
    
    // 详细描述组
    QGroupBox *descriptionGroup = new QGroupBox("战例描述", m_contentWidget);
    QVBoxLayout *descLayout = new QVBoxLayout(descriptionGroup);
    
    m_descriptionEdit = new QTextEdit(descriptionGroup);
    m_descriptionEdit->setReadOnly(true);
    m_descriptionEdit->setMaximumHeight(100);
    descLayout->addWidget(m_descriptionEdit);
    
    contentLayout->addWidget(descriptionGroup);
    
    // 战场环境组
    QGroupBox *environmentGroup = new QGroupBox("战场环境", m_contentWidget);
    QVBoxLayout *envLayout = new QVBoxLayout(environmentGroup);
    
    m_environmentEdit = new QTextEdit(environmentGroup);
    m_environmentEdit->setReadOnly(true);
    m_environmentEdit->setMaximumHeight(80);
    envLayout->addWidget(m_environmentEdit);
    
    contentLayout->addWidget(environmentGroup);
    
    // 初始态势组
    QGroupBox *initialGroup = new QGroupBox("初始态势", m_contentWidget);
    QVBoxLayout *initialLayout = new QVBoxLayout(initialGroup);
    
    m_initialSituationEdit = new QTextEdit(initialGroup);
    m_initialSituationEdit->setReadOnly(true);
    m_initialSituationEdit->setMaximumHeight(80);
    initialLayout->addWidget(m_initialSituationEdit);
    
    contentLayout->addWidget(initialGroup);
    
    // 当前态势组
    QGroupBox *currentGroup = new QGroupBox("当前态势", m_contentWidget);
    QVBoxLayout *currentLayout = new QVBoxLayout(currentGroup);
    
    m_currentTimeLabel = new QLabel("当前时间：", currentGroup);
    m_currentTacticsEdit = new QTextEdit(currentGroup);
    m_currentTacticsEdit->setReadOnly(true);
    m_currentTacticsEdit->setPlaceholderText("当前应用的战法...");
    m_currentTacticsEdit->setMaximumHeight(100);
    
    m_currentCasualtyEdit = new QTextEdit(currentGroup);
    m_currentCasualtyEdit->setReadOnly(true);
    m_currentCasualtyEdit->setPlaceholderText("当前战损情况...");
    m_currentCasualtyEdit->setMaximumHeight(80);
    
    currentLayout->addWidget(m_currentTimeLabel);
    currentLayout->addWidget(m_currentTacticsEdit);
    currentLayout->addWidget(m_currentCasualtyEdit);
    
    contentLayout->addWidget(currentGroup);
    
    // 最终情况组
    QGroupBox *finalGroup = new QGroupBox("最终情况", m_contentWidget);
    QVBoxLayout *finalLayout = new QVBoxLayout(finalGroup);
    
    m_finalSituationEdit = new QTextEdit(finalGroup);
    m_finalSituationEdit->setReadOnly(true);
    m_finalSituationEdit->setMaximumHeight(100);
    finalLayout->addWidget(m_finalSituationEdit);
    
    contentLayout->addWidget(finalGroup);
    
    contentLayout->addStretch();
    
    m_scrollArea->setWidget(m_contentWidget);
    m_mainLayout->addWidget(m_scrollArea);
}

void WarCaseInfoWidget::updateInfo(const WarCase &warCase)
{
    m_currentCase = warCase;
    
    m_nameLabel->setText(QString("战例名称：%1").arg(warCase.name));
    m_typeLabel->setText(QString("战例类型：%1").arg(warCase.type));
    m_timeLabel->setText(QString("时间：%1 至 %2")
                         .arg(warCase.startTime.toString("yyyy-MM-dd hh:mm"))
                         .arg(warCase.endTime.toString("yyyy-MM-dd hh:mm")));
    
    m_descriptionEdit->setPlainText(warCase.description);
    m_environmentEdit->setPlainText(warCase.battlefieldEnvironment);
    m_initialSituationEdit->setPlainText(warCase.initialSituation);
    m_finalSituationEdit->setPlainText(warCase.finalSituation);
    
    // 清空当前态势
    m_currentTimeLabel->setText("当前时间：--");
    m_currentTacticsEdit->clear();
    m_currentCasualtyEdit->clear();
}

void WarCaseInfoWidget::updateCurrentSituation(const QDateTime &time, 
                                                const QList<Tactics> &tactics,
                                                const Casualty &casualty)
{
    m_currentTimeLabel->setText(QString("当前时间：%1").arg(time.toString("yyyy-MM-dd hh:mm:ss")));
    
    // 更新战法应用情况
    QString tacticsText;
    if (tactics.isEmpty()) {
        tacticsText = "当前无战法应用";
    } else {
        for (const Tactics &t : tactics) {
            tacticsText += QString("【%1】\n%2\n效果：%3\n\n")
                          .arg(t.name)
                          .arg(t.description)
                          .arg(t.effect);
        }
    }
    m_currentTacticsEdit->setPlainText(tacticsText);
    
    // 更新战损情况
    QString casualtyText = QString("人员战损：%1 人\n装备战损：%2 件\n战损比：%3%")
                          .arg(casualty.personnel)
                          .arg(casualty.equipment)
                          .arg(casualty.casualtyRatio * 100, 0, 'f', 2);
    m_currentCasualtyEdit->setPlainText(casualtyText);
}

