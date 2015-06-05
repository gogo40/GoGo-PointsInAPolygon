/*
Point in Polygon Widget

A small widget to test if a point is inside a polygon

(using a GPS and GPS points)

(c) 2015, Péricles Lopes Machado
*/
#ifndef POINTINPOLYGON_H
#define POINTINPOLYGON_H

#include <QWidget>

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

private:
    Ui::PointInPolygon *ui;
};

#endif // POINTINPOLYGON_H
