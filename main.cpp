/*
Point in Polygon

A small widget to test if a point is inside a polygon

(using a GPS and GPS points)

(c) 2015, Péricles Lopes Machado
*/

#include "point_in_polygon_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PointInPolygonWidget w;
    w.show();

    return a.exec();
}
