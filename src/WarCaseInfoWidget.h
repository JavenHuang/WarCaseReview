#ifndef WARCASEINFOWIDGET_H
#define WARCASEINFOWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QScrollArea>
#include "WarCaseData.h"

class WarCaseInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WarCaseInfoWidget(QWidget *parent = nullptr);
    
    // 更新战例信息
    void updateInfo(const WarCase &warCase);
    void updateCurrentSituation(const QDateTime &time, 
                                const QList<Tactics> &tactics,
                                const Casualty &casualty);

private:
    void setupUI();
    
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    
    // 基本信息
    QLabel *m_nameLabel;
    QLabel *m_typeLabel;
    QLabel *m_timeLabel;
    
    // 详细信息
    QTextEdit *m_descriptionEdit;
    QTextEdit *m_environmentEdit;
    QTextEdit *m_initialSituationEdit;
    
    // 当前态势
    QLabel *m_currentTimeLabel;
    QTextEdit *m_currentTacticsEdit;
    QTextEdit *m_currentCasualtyEdit;
    
    // 最终情况
    QTextEdit *m_finalSituationEdit;
    
    WarCase m_currentCase;
};

#endif // WARCASEINFOWIDGET_H

