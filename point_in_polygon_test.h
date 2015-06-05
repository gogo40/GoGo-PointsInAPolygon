/*
Winding Test

Péricles Lopes Machado, 2015

- Small changes to support doubles.

These functions implements two test to verify if a point is inside a polygon
*/

// Copyright 2000 softSurfer, 2012 Dan Sunday
// This code may be freely used, distributed and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

#ifndef WINDING_TEST_H
#define WINDING_TEST_H

#include <vector>

namespace PointInPolygonTest {


void setTolerance(double eps);

// a Point is defined by its coordinates {double x, y;}
//===================================================================

struct Point {
    double x, y;
};

typedef std::vector<Point> Points;

// isLeft(): tests if a point is Left|On|Right of an infinite line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 and P1
//            =0 for P2  on the line
//            <0 for P2  right of the line
//    See: Algorithm 1 "Area of Triangles and Polygons"
inline double isLeft(const Point& P0, const Point& P1, const Point& P2 )
{
    return ((P1.x - P0.x) * (P2.y - P0.y)
            - (P2.x -  P0.x) * (P1.y - P0.y));
}

// cn_PnPoly(): crossing number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  0 = outside, 1 = inside
// This code is patterned after [Franklin, 2000]
int cn_PnPoly(const Point& P, const Points& V);

// wn_PnPoly(): winding number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  wn = the winding number (=0 only when P is outside)
int wn_PnPoly( const Point& P, const Points& V);

}

#endif // WINDING_TEST_H

