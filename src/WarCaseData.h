#ifndef WARCASEDATA_H
#define WARCASEDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

// 单位级别枚举
enum class UnitLevel {
    Brigade,    // 旅
    Regiment,   // 团
    Battalion,  // 营
    Company,    // 连
    Platoon,    // 排
    Squad       // 班
};

// 单位信息
struct Unit {
    QString id;
    QString name;
    UnitLevel level;
    QString type; // 步兵、装甲、炮兵等
    double longitude;
    double latitude;
    int personnel;
    int equipment;
    QString status; // 正常、受损、被歼等
};

// 战损信息
struct Casualty {
    int personnel;
    int equipment;
    double casualtyRatio;
    QDateTime timestamp;
};

// 战法应用
struct Tactics {
    QString name;
    QString description;
    QDateTime startTime;
    QDateTime endTime;
    QString effect;
};

// 阶段信息
struct Phase {
    QString name;
    QString description;
    QDateTime startTime;
    QDateTime endTime;
    QList<Tactics> tactics;
};

// 战例数据
struct WarCase {
    QString id;
    QString name;
    QString type; // "战役" 或 "战术"
    QString description;
    QDateTime startTime;
    QDateTime endTime;
    QString battlefieldEnvironment;
    QString initialSituation;
    QList<Phase> phases;
    QList<Unit> units;
    QList<Casualty> casualties;
    QString finalSituation;
    UnitLevel precisionLevel; // 复盘精度级别
};

class WarCaseData : public QObject
{
    Q_OBJECT

public:
    explicit WarCaseData(QObject *parent = nullptr);
    
    // 加载战例数据
    bool loadWarCases();
    
    // 获取战例列表
    QList<WarCase> getCampaignCases() const { return m_campaignCases; }
    QList<WarCase> getTacticalCases() const { return m_tacticalCases; }
    
    // 获取指定战例
    WarCase getCase(int type, int index) const;
    
    // 获取指定时间点的态势数据
    QList<Unit> getUnitsAtTime(int type, int index, const QDateTime &time) const;
    Casualty getCasualtiesAtTime(int type, int index, const QDateTime &time) const;
    QList<Tactics> getTacticsAtTime(int type, int index, const QDateTime &time) const;

signals:
    void dataLoaded();

private:
    void initializeDefaultCases();
    QList<Unit> interpolateUnits(const QList<Unit> &units, const QDateTime &time) const;
    
    QList<WarCase> m_campaignCases;  // 战役战例
    QList<WarCase> m_tacticalCases;  // 战术战例
};

#endif // WARCASEDATA_H

