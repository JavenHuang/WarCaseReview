#include "ReplayController.h"
#include <QDebug>

ReplayController::ReplayController(QObject *parent)
    : QObject(parent)
    , m_warCaseData(nullptr)
    , m_isPlaying(false)
    , m_speedRatio(2)
    , m_currentType(-1)
    , m_currentIndex(-1)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(100); // 100ms更新一次
    connect(m_timer, &QTimer::timeout, this, &ReplayController::onTimerTimeout);
}

void ReplayController::setCurrentCase(int type, int index)
{
    m_currentType = type;
    m_currentIndex = index;
    
    // 从父对象获取WarCaseData
    QObject *p = parent();
    while (p && !m_warCaseData) {
        m_warCaseData = qobject_cast<WarCaseData*>(p);
        p = p->parent();
    }
    
    if (!m_warCaseData) {
        qDebug() << "ReplayController需要WarCaseData作为父对象";
        return;
    }
    
    m_currentCase = m_warCaseData->getCase(type, index);
    if (m_currentCase.id.isEmpty()) {
        return;
    }
    
    m_startTime = m_currentCase.startTime;
    m_endTime = m_currentCase.endTime;
    m_currentTime = m_startTime;
    
    stop();
}

void ReplayController::play()
{
    if (m_currentCase.id.isEmpty()) {
        return;
    }
    
    m_isPlaying = true;
    m_elapsedTimer.restart();
    m_lastElapsedMs = 0;
    m_timer->start();
}

void ReplayController::pause()
{
    m_isPlaying = false;
    m_timer->stop();
}

void ReplayController::stop()
{
    m_isPlaying = false;
    m_timer->stop();
    m_currentTime = m_startTime;
    m_elapsedTimer.invalidate();
    m_lastElapsedMs = 0;
    
    updateCurrentState();
    emit progressChanged(0);
}

void ReplayController::seek(int progress)
{
    if (m_currentCase.id.isEmpty()) {
        return;
    }
    
    qint64 totalDuration = m_startTime.msecsTo(m_endTime);
    if (totalDuration <= 0) {
        return;
    }
    
    qint64 targetMs = (qint64)(totalDuration * progress / 100.0);
    m_currentTime = m_startTime.addMSecs(targetMs);
    
    if (m_currentTime > m_endTime) {
        m_currentTime = m_endTime;
    }
    
    updateCurrentState();
    emit progressChanged(progress);
}

void ReplayController::setSpeed(int speedRatio)
{
    m_speedRatio = speedRatio;
}

int ReplayController::getProgress() const
{
    if (m_currentCase.id.isEmpty()) {
        return 0;
    }
    
    qint64 totalDuration = m_startTime.msecsTo(m_endTime);
    if (totalDuration <= 0) {
        return 0;
    }
    
    qint64 currentDuration = m_startTime.msecsTo(m_currentTime);
    int progress = (int)(currentDuration * 100.0 / totalDuration);
    return qBound(0, progress, 100);
}

void ReplayController::onTimerTimeout()
{
    if (!m_isPlaying || m_currentCase.id.isEmpty()) {
        return;
    }
    
    if (!m_elapsedTimer.isValid()) {
        m_elapsedTimer.restart();
        m_lastElapsedMs = 0;
        return;
    }
    
    qint64 elapsedMs = m_elapsedTimer.elapsed();
    qint64 deltaMs = (elapsedMs - m_lastElapsedMs) * m_speedRatio;
    m_lastElapsedMs = elapsedMs;
    
    m_currentTime = m_currentTime.addMSecs(deltaMs);
    
    if (m_currentTime >= m_endTime) {
        m_currentTime = m_endTime;
        m_isPlaying = false;
        m_timer->stop();
        updateCurrentState();
        emit finished();
        return;
    }
    
    updateCurrentState();
    emit timeChanged(m_currentTime);
    emit progressChanged(getProgress());
}

void ReplayController::updateCurrentState()
{
    if (!m_warCaseData || m_currentCase.id.isEmpty()) {
        return;
    }
    
    QList<Unit> units = m_warCaseData->getUnitsAtTime(m_currentType, m_currentIndex, m_currentTime);
    Casualty casualty = m_warCaseData->getCasualtiesAtTime(m_currentType, m_currentIndex, m_currentTime);
    QList<Tactics> tactics = m_warCaseData->getTacticsAtTime(m_currentType, m_currentIndex, m_currentTime);
    
    emit unitsUpdated(units);
    emit casualtiesUpdated(casualty);
    emit tacticsUpdated(tactics);
}

