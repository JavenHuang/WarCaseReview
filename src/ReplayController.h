#ifndef REPLAYCONTROLLER_H
#define REPLAYCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QElapsedTimer>
#include "WarCaseData.h"

class ReplayController : public QObject
{
    Q_OBJECT

public:
    explicit ReplayController(QObject *parent = nullptr);
    
    // 设置当前战例
    void setCurrentCase(int type, int index);
    
    // 播放控制
    void play();
    void pause();
    void stop();
    void seek(int progress); // 0-100
    
    // 设置播放速度（加速比）
    void setSpeed(int speedRatio); // 2, 4, 8, 16, 32
    
    // 获取当前状态
    bool isPlaying() const { return m_isPlaying; }
    QDateTime getCurrentTime() const { return m_currentTime; }
    int getProgress() const; // 0-100
    int getSpeedRatio() const { return m_speedRatio; }

signals:
    void timeChanged(const QDateTime &time);
    void progressChanged(int progress);
    void finished();
    void unitsUpdated(const QList<Unit> &units);
    void casualtiesUpdated(const Casualty &casualty);
    void tacticsUpdated(const QList<Tactics> &tactics);

private slots:
    void onTimerTimeout();

private:
    void updateCurrentState();
    
    WarCaseData *m_warCaseData;
    WarCase m_currentCase;
    QDateTime m_startTime;
    QDateTime m_endTime;
    QDateTime m_currentTime;
    
    QTimer *m_timer;
    QElapsedTimer m_elapsedTimer;
    qint64 m_lastElapsedMs;
    
    bool m_isPlaying;
    int m_speedRatio; // 加速比: 2, 4, 8, 16, 32
    int m_currentType;
    int m_currentIndex;
};

#endif // REPLAYCONTROLLER_H

