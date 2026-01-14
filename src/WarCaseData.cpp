#include "WarCaseData.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

WarCaseData::WarCaseData(QObject *parent)
    : QObject(parent)
{
    initializeDefaultCases();
}

bool WarCaseData::loadWarCases()
{
    // 初始化默认战例数据
    initializeDefaultCases();
    emit dataLoaded();
    return true;
}

void WarCaseData::initializeDefaultCases()
{
    m_campaignCases.clear();
    m_tacticalCases.clear();
    
    // 创建10个经典战例（5个战役，5个战术）
    
    // 战役战例1：上甘岭战役
    WarCase case1;
    case1.id = "campaign_001";
    case1.name = "上甘岭战役";
    case1.type = "战役";
    case1.description = "1952年10月14日至11月25日，中国人民志愿军在朝鲜上甘岭地区与联合国军进行的防御作战。";
    case1.startTime = QDateTime(QDate(1952, 10, 14), QTime(4, 0));
    case1.endTime = QDateTime(QDate(1952, 11, 25), QTime(20, 0));
    case1.battlefieldEnvironment = "山地地形，海拔1000-1500米，地形复杂，易守难攻。";
    case1.initialSituation = "志愿军第15军第45师防守上甘岭地区，联合国军集中优势兵力发起进攻。";
    case1.precisionLevel = UnitLevel::Battalion;
    
    Phase phase1_1;
    phase1_1.name = "防御阶段";
    phase1_1.description = "初期防御作战";
    phase1_1.startTime = case1.startTime;
    phase1_1.endTime = QDateTime(QDate(1952, 10, 20), QTime(12, 0));
    
    Tactics tactics1_1;
    tactics1_1.name = "坑道防御";
    tactics1_1.description = "利用坑道工事进行顽强防御";
    tactics1_1.startTime = case1.startTime;
    tactics1_1.endTime = case1.endTime;
    tactics1_1.effect = "有效保存有生力量";
    phase1_1.tactics.append(tactics1_1);
    
    case1.phases.append(phase1_1);
    
    Unit unit1_1;
    unit1_1.id = "unit_001";
    unit1_1.name = "第45师第135团";
    unit1_1.level = UnitLevel::Regiment;
    unit1_1.type = "步兵";
    unit1_1.longitude = 127.5;
    unit1_1.latitude = 38.3;
    unit1_1.personnel = 3000;
    unit1_1.equipment = 500;
    unit1_1.status = "正常";
    case1.units.append(unit1_1);
    
    Casualty casualty1;
    casualty1.personnel = 11500;
    casualty1.equipment = 2000;
    casualty1.casualtyRatio = 0.15;
    casualty1.timestamp = case1.endTime;
    case1.casualties.append(casualty1);
    
    case1.finalSituation = "志愿军成功守住上甘岭阵地，联合国军伤亡惨重，被迫停止进攻。";
    m_campaignCases.append(case1);
    
    // 战役战例2：平型关大捷
    WarCase case2;
    case2.id = "campaign_002";
    case2.name = "平型关大捷";
    case2.type = "战役";
    case2.description = "1937年9月25日，八路军第115师在平型关伏击日军第5师团第21旅团。";
    case2.startTime = QDateTime(QDate(1937, 9, 25), QTime(7, 0));
    case2.endTime = QDateTime(QDate(1937, 9, 25), QTime(13, 0));
    case2.battlefieldEnvironment = "山地峡谷地形，适合伏击。";
    case2.initialSituation = "日军第21旅团沿公路行进，八路军115师设伏。";
    case2.precisionLevel = UnitLevel::Regiment;
    
    Unit unit2_1;
    unit2_1.id = "unit_002";
    unit2_1.name = "第115师第343旅";
    unit2_1.level = UnitLevel::Brigade;
    unit2_1.type = "步兵";
    unit2_1.longitude = 113.6;
    unit2_1.latitude = 39.2;
    unit2_1.personnel = 5000;
    unit2_1.equipment = 3000;
    unit2_1.status = "正常";
    case2.units.append(unit2_1);
    
    Casualty casualty2;
    casualty2.personnel = 1000;
    casualty2.equipment = 100;
    casualty2.casualtyRatio = 0.20;
    casualty2.timestamp = case2.endTime;
    case2.casualties.append(casualty2);
    
    case2.finalSituation = "八路军全歼日军辎重队，击毙日军1000余人，缴获大量武器装备。";
    m_campaignCases.append(case2);
    
    // 添加更多战役战例（简化版）
    for (int i = 3; i <= 5; i++) {
        WarCase case_i;
        case_i.id = QString("campaign_%1").arg(i, 3, 10, QChar('0'));
        case_i.name = QString("战役战例%1").arg(i);
        case_i.type = "战役";
        case_i.description = QString("经典战役战例%1的详细描述").arg(i);
        case_i.startTime = QDateTime(QDate(1950 + i, 1, 1), QTime(8, 0));
        case_i.endTime = QDateTime(QDate(1950 + i, 1, 5), QTime(18, 0));
        case_i.battlefieldEnvironment = "复杂地形环境";
        case_i.initialSituation = "初始作战态势";
        case_i.precisionLevel = UnitLevel::Battalion;
        
        Unit unit_i;
        unit_i.id = QString("unit_%1").arg(i, 3, 10, QChar('0'));
        unit_i.name = QString("第%1师").arg(i);
        unit_i.level = UnitLevel::Regiment;
        unit_i.type = "步兵";
        unit_i.longitude = 116.0 + i * 0.1;
        unit_i.latitude = 39.0 + i * 0.1;
        unit_i.personnel = 2000 + i * 100;
        unit_i.equipment = 300 + i * 50;
        unit_i.status = "正常";
        case_i.units.append(unit_i);
        
        Casualty casualty_i;
        casualty_i.personnel = 500 + i * 50;
        casualty_i.equipment = 100 + i * 10;
        casualty_i.casualtyRatio = 0.15 + i * 0.01;
        casualty_i.timestamp = case_i.endTime;
        case_i.casualties.append(casualty_i);
        
        case_i.finalSituation = QString("战役%1最终情况说明").arg(i);
        m_campaignCases.append(case_i);
    }
    
    // 战术战例1：夜袭阳明堡
    WarCase tactical1;
    tactical1.id = "tactical_001";
    tactical1.name = "夜袭阳明堡";
    tactical1.type = "战术";
    tactical1.description = "1937年10月19日夜，八路军第129师第769团夜袭阳明堡日军机场。";
    tactical1.startTime = QDateTime(QDate(1937, 10, 19), QTime(19, 0));
    tactical1.endTime = QDateTime(QDate(1937, 10, 20), QTime(1, 0));
    tactical1.battlefieldEnvironment = "机场周边，夜间作战。";
    tactical1.initialSituation = "日军机场防守薄弱，八路军组织夜袭。";
    tactical1.precisionLevel = UnitLevel::Platoon;
    
    Unit unit_t1;
    unit_t1.id = "unit_t001";
    unit_t1.name = "第769团第3营";
    unit_t1.level = UnitLevel::Battalion;
    unit_t1.type = "步兵";
    unit_t1.longitude = 112.8;
    unit_t1.latitude = 38.5;
    unit_t1.personnel = 500;
    unit_t1.equipment = 200;
    unit_t1.status = "正常";
    tactical1.units.append(unit_t1);
    
    Casualty casualty_t1;
    casualty_t1.personnel = 30;
    casualty_t1.equipment = 10;
    casualty_t1.casualtyRatio = 0.06;
    casualty_t1.timestamp = tactical1.endTime;
    tactical1.casualties.append(casualty_t1);
    
    tactical1.finalSituation = "成功摧毁日军飞机24架，毙伤日军100余人。";
    m_tacticalCases.append(tactical1);
    
    // 添加更多战术战例（简化版）
    for (int i = 2; i <= 5; i++) {
        WarCase tactical_i;
        tactical_i.id = QString("tactical_%1").arg(i, 3, 10, QChar('0'));
        tactical_i.name = QString("战术战例%1").arg(i);
        tactical_i.type = "战术";
        tactical_i.description = QString("经典战术战例%1的详细描述").arg(i);
        tactical_i.startTime = QDateTime(QDate(1940 + i, 1, 1), QTime(6, 0));
        tactical_i.endTime = QDateTime(QDate(1940 + i, 1, 1), QTime(12, 0));
        tactical_i.battlefieldEnvironment = "战术环境";
        tactical_i.initialSituation = "初始战术态势";
        tactical_i.precisionLevel = UnitLevel::Squad;
        
        Unit unit_ti;
        unit_ti.id = QString("unit_t%1").arg(i, 3, 10, QChar('0'));
        unit_ti.name = QString("第%1排").arg(i);
        unit_ti.level = UnitLevel::Platoon;
        unit_ti.type = "步兵";
        unit_ti.longitude = 115.0 + i * 0.1;
        unit_ti.latitude = 38.0 + i * 0.1;
        unit_ti.personnel = 30 + i * 5;
        unit_ti.equipment = 20 + i * 3;
        unit_ti.status = "正常";
        tactical_i.units.append(unit_ti);
        
        Casualty casualty_ti;
        casualty_ti.personnel = 5 + i;
        casualty_ti.equipment = 2 + i;
        casualty_ti.casualtyRatio = 0.10 + i * 0.02;
        casualty_ti.timestamp = tactical_i.endTime;
        tactical_i.casualties.append(casualty_ti);
        
        tactical_i.finalSituation = QString("战术%1最终情况说明").arg(i);
        m_tacticalCases.append(tactical_i);
    }
}

WarCase WarCaseData::getCase(int type, int index) const
{
    if (type == 0) {
        if (index >= 0 && index < m_campaignCases.size()) {
            return m_campaignCases[index];
        }
    } else {
        if (index >= 0 && index < m_tacticalCases.size()) {
            return m_tacticalCases[index];
        }
    }
    return WarCase();
}

QList<Unit> WarCaseData::getUnitsAtTime(int type, int index, const QDateTime &time) const
{
    WarCase warCase = getCase(type, index);
    if (warCase.id.isEmpty()) {
        return QList<Unit>();
    }
    
    // 简化实现：返回初始单位，实际应该根据时间插值
    return interpolateUnits(warCase.units, time);
}

Casualty WarCaseData::getCasualtiesAtTime(int type, int index, const QDateTime &time) const
{
    WarCase warCase = getCase(type, index);
    if (warCase.id.isEmpty() || warCase.casualties.isEmpty()) {
        return Casualty();
    }
    
    // 根据时间查找对应的战损数据
    qint64 totalDuration = warCase.startTime.msecsTo(warCase.endTime);
    qint64 currentDuration = warCase.startTime.msecsTo(time);
    
    if (totalDuration <= 0) {
        return warCase.casualties.first();
    }
    
    double progress = qBound(0.0, (double)currentDuration / totalDuration, 1.0);
    
    if (warCase.casualties.size() == 1) {
        Casualty result = warCase.casualties.first();
        result.personnel = (int)(result.personnel * progress);
        result.equipment = (int)(result.equipment * progress);
        result.casualtyRatio = result.casualtyRatio * progress;
        return result;
    }
    
    // 如果有多个时间点的战损数据，进行插值
    return warCase.casualties.first();
}

QList<Tactics> WarCaseData::getTacticsAtTime(int type, int index, const QDateTime &time) const
{
    WarCase warCase = getCase(type, index);
    if (warCase.id.isEmpty()) {
        return QList<Tactics>();
    }
    
    QList<Tactics> result;
    for (const Phase &phase : warCase.phases) {
        if (time >= phase.startTime && time <= phase.endTime) {
            for (const Tactics &tactic : phase.tactics) {
                if (time >= tactic.startTime && time <= tactic.endTime) {
                    result.append(tactic);
                }
            }
        }
    }
    
    return result;
}

QList<Unit> WarCaseData::interpolateUnits(const QList<Unit> &units, const QDateTime &time) const
{
    // 简化实现：直接返回单位列表
    // 实际应该根据时间插值计算单位位置和状态
    return units;
}

