#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPainterPath>
#include <QList>
#include "WarCaseData.h"

class MapWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);
    
    // 加载GeoJSON地图数据
    bool loadGeoJson(const QString &filePath);
    
    // 更新单位位置和状态
    void updateUnits(const QList<Unit> &units);
    
    // 设置地图范围
    void setMapBounds(double minLon, double minLat, double maxLon, double maxLat);
    
    // 清除所有单位标记
    void clearUnits();

signals:
    void mapLoaded();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void setupScene();
    void parseGeoJson(const QJsonDocument &doc);
    QPointF lonLatToScene(double lon, double lat) const;
    QColor getUnitColor(const Unit &unit) const;
    QString getUnitSymbol(const Unit &unit) const;
    
    QGraphicsScene *m_scene;
    QList<QGraphicsItem*> m_mapFeatures;
    QList<QGraphicsItem*> m_unitMarkers;
    
    // 地图边界
    double m_minLon;
    double m_minLat;
    double m_maxLon;
    double m_maxLat;
    
    // 缩放和平移
    bool m_isPanning;
    QPoint m_lastPanPoint;
    qreal m_scaleFactor;
    
    // GeoJSON数据
    QJsonDocument m_geoJsonDoc;
};

#endif // MAPWIDGET_H

