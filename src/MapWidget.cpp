#include "MapWidget.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsPathItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include <QtMath>

MapWidget::MapWidget(QWidget *parent)
    : QGraphicsView(parent)
    , m_minLon(0)
    , m_minLat(0)
    , m_maxLon(0)
    , m_maxLat(0)
    , m_isPanning(false)
    , m_scaleFactor(1.0)
{
    setupScene();
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

void MapWidget::setupScene()
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    
    // 设置场景背景色
    m_scene->setBackgroundBrush(QBrush(QColor(240, 248, 255)));
}

bool MapWidget::loadGeoJson(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开GeoJSON文件:" << filePath;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "GeoJSON解析错误:" << error.errorString();
        return false;
    }
    
    m_geoJsonDoc = doc;
    parseGeoJson(doc);
    
    emit mapLoaded();
    return true;
}

void MapWidget::parseGeoJson(const QJsonDocument &doc)
{
    // 清除旧的地图要素
    for (QGraphicsItem *item : m_mapFeatures) {
        m_scene->removeItem(item);
        delete item;
    }
    m_mapFeatures.clear();
    
    QJsonObject root = doc.object();
    QString type = root["type"].toString();
    
    if (type == "FeatureCollection") {
        QJsonArray features = root["features"].toArray();
        
        double minLon = 180, maxLon = -180;
        double minLat = 90, maxLat = -90;
        
        for (const QJsonValue &featureValue : features) {
            QJsonObject feature = featureValue.toObject();
            QJsonObject geometry = feature["geometry"].toObject();
            QString geomType = geometry["type"].toString();
            QJsonArray coordinates = geometry["coordinates"].toArray();
            
            if (geomType == "Polygon" || geomType == "MultiPolygon") {
                QPainterPath path;
                
                if (geomType == "Polygon") {
                    QJsonArray rings = coordinates;
                    if (!rings.isEmpty()) {
                        QJsonArray ring = rings[0].toArray();
                        bool first = true;
                        for (const QJsonValue &coordValue : ring) {
                            QJsonArray coord = coordValue.toArray();
                            double lon = coord[0].toDouble();
                            double lat = coord[1].toDouble();
                            
                            minLon = qMin(minLon, lon);
                            maxLon = qMax(maxLon, lon);
                            minLat = qMin(minLat, lat);
                            maxLat = qMax(maxLat, lat);
                            
                            QPointF point = lonLatToScene(lon, lat);
                            if (first) {
                                path.moveTo(point);
                                first = false;
                            } else {
                                path.lineTo(point);
                            }
                        }
                        path.closeSubpath();
                    }
                } else if (geomType == "MultiPolygon") {
                    // 处理MultiPolygon
                    for (const QJsonValue &polygonValue : coordinates) {
                        QJsonArray rings = polygonValue.toArray();
                        if (!rings.isEmpty()) {
                            QJsonArray ring = rings[0].toArray();
                            bool first = true;
                            for (const QJsonValue &coordValue : ring) {
                                QJsonArray coord = coordValue.toArray();
                                double lon = coord[0].toDouble();
                                double lat = coord[1].toDouble();
                                
                                minLon = qMin(minLon, lon);
                                maxLon = qMax(maxLon, lon);
                                minLat = qMin(minLat, lat);
                                maxLat = qMax(maxLat, lat);
                                
                                QPointF point = lonLatToScene(lon, lat);
                                if (first) {
                                    path.moveTo(point);
                                    first = false;
                                } else {
                                    path.lineTo(point);
                                }
                            }
                            path.closeSubpath();
                        }
                    }
                }
                
                QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
                pathItem->setPen(QPen(QColor(100, 150, 200), 1));
                pathItem->setBrush(QBrush(QColor(200, 220, 240, 100)));
                m_scene->addItem(pathItem);
                m_mapFeatures.append(pathItem);
            } else if (geomType == "LineString" || geomType == "MultiLineString") {
                QPainterPath path;
                
                if (geomType == "LineString") {
                    bool first = true;
                    for (const QJsonValue &coordValue : coordinates) {
                        QJsonArray coord = coordValue.toArray();
                        double lon = coord[0].toDouble();
                        double lat = coord[1].toDouble();
                        
                        minLon = qMin(minLon, lon);
                        maxLon = qMax(maxLon, lon);
                        minLat = qMin(minLat, lat);
                        maxLat = qMax(maxLat, lat);
                        
                        QPointF point = lonLatToScene(lon, lat);
                        if (first) {
                            path.moveTo(point);
                            first = false;
                        } else {
                            path.lineTo(point);
                        }
                    }
                }
                
                QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
                pathItem->setPen(QPen(QColor(150, 150, 150), 2));
                m_scene->addItem(pathItem);
                m_mapFeatures.append(pathItem);
            }
        }
        
        // 更新地图边界
        if (minLon < maxLon && minLat < maxLat) {
            m_minLon = minLon;
            m_maxLon = maxLon;
            m_minLat = minLat;
            m_maxLat = maxLat;
        }
    }
    
    // 调整视图以适应场景
    if (m_scene->items().size() > 0) {
        fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

QPointF MapWidget::lonLatToScene(double lon, double lat) const
{
    // 简单的墨卡托投影
    if (m_maxLon == m_minLon || m_maxLat == m_minLat) {
        return QPointF(lon * 1000, -lat * 1000);
    }
    
    double x = (lon - m_minLon) / (m_maxLon - m_minLon) * 2000;
    double y = (m_maxLat - lat) / (m_maxLat - m_minLat) * 2000;
    
    return QPointF(x, y);
}

void MapWidget::setMapBounds(double minLon, double minLat, double maxLon, double maxLat)
{
    m_minLon = minLon;
    m_minLat = minLat;
    m_maxLon = maxLon;
    m_maxLat = maxLat;
    
    // 重新解析GeoJSON以更新坐标转换
    if (!m_geoJsonDoc.isNull()) {
        parseGeoJson(m_geoJsonDoc);
    }
}

void MapWidget::updateUnits(const QList<Unit> &units)
{
    // 清除旧的单位标记
    clearUnits();
    
    // 添加新的单位标记
    for (const Unit &unit : units) {
        QPointF pos = lonLatToScene(unit.longitude, unit.latitude);
        
        // 创建单位标记（圆形）
        QGraphicsEllipseItem *marker = new QGraphicsEllipseItem(
            pos.x() - 5, pos.y() - 5, 10, 10);
        marker->setBrush(QBrush(getUnitColor(unit)));
        marker->setPen(QPen(Qt::black, 1));
        marker->setZValue(100);
        
        m_scene->addItem(marker);
        m_unitMarkers.append(marker);
        
        // 添加单位名称标签
        QGraphicsTextItem *label = new QGraphicsTextItem(unit.name);
        label->setPos(pos.x() + 8, pos.y() - 8);
        label->setDefaultTextColor(Qt::black);
        label->setFont(QFont("Arial", 8));
        label->setZValue(101);
        
        m_scene->addItem(label);
        m_unitMarkers.append(label);
    }
}

void MapWidget::clearUnits()
{
    for (QGraphicsItem *item : m_unitMarkers) {
        m_scene->removeItem(item);
        delete item;
    }
    m_unitMarkers.clear();
}

QColor MapWidget::getUnitColor(const Unit &unit) const
{
    if (unit.status == "正常") {
        return QColor(0, 255, 0, 200);
    } else if (unit.status == "受损") {
        return QColor(255, 255, 0, 200);
    } else if (unit.status == "被歼") {
        return QColor(255, 0, 0, 200);
    }
    return QColor(128, 128, 128, 200);
}

QString MapWidget::getUnitSymbol(const Unit &unit) const
{
    if (unit.type == "步兵") {
        return "I";
    } else if (unit.type == "装甲") {
        return "A";
    } else if (unit.type == "炮兵") {
        return "A";
    }
    return "U";
}

void MapWidget::wheelEvent(QWheelEvent *event)
{
    // 缩放
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
        m_scaleFactor *= scaleFactor;
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        m_scaleFactor /= scaleFactor;
    }
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton || 
        (event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier)) {
        m_isPanning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPanning) {
        QPoint delta = event->pos() - m_lastPanPoint;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        m_lastPanPoint = event->pos();
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isPanning) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void MapWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
}

