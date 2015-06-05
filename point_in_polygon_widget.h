/*
Point in Polygon Widget

A small widget to test if a point is inside a polygon

(using a GPS and GPS points)

(c) 2015, Péricles Lopes Machado
*/
#ifndef POINTINPOLYGON_H
#define POINTINPOLYGON_H

#include <QWidget>

#include <map>

#include <QGeoCoordinate>
#include <QGeoSatelliteInfoSource>
#include <QGeoPositionInfoSource>

#include "point_in_polygon_test.h"

namespace Ui {
class PointInPolygon;
}

class PointInPolygonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointInPolygonWidget(QWidget *parent = 0);
    ~PointInPolygonWidget();

private slots:
    void on_addPointButton_clicked();
    void remove_row();

    void on_verifyPointButton_clicked();

    void on_loadFromCSVFileButton_clicked();

    void on_testPointsFromCSVButton_clicked();

    void positionUpdated(const QGeoPositionInfo &info);
    void resetGPS();

private:

    void add_new_point(const QString& mine, const QString& point, double x, double y);
    void update_points_map();
    void verify_point(const PointInPolygonTest::Point& p);

    Ui::PointInPolygon *ui;
    typedef std::tuple<QString, QString> IdMine;
    std::map<IdMine, PointInPolygonTest::Points> pts_map_;

    QGeoPositionInfoSource* position_info_;
    QGeoSatelliteInfoSource* satellite_info_;
    QGeoPositionInfo last_position_;
    double dist_acc_;
    bool is_first_distance_;
};

#endif // POINTINPOLYGON_H
