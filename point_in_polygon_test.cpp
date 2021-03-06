/*
Winding Test

Péricles Lopes Machado, 2015

- Small changes to support doubles.


Based on implementation present in http://geomalgorithms.com/a03-_inclusion.html
*/

// Copyright 2000 softSurfer, 2012 Dan Sunday
// This code may be freely used, distributed and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

#include "point_in_polygon_test.h"

static double EPS = 1e-10;

// cn_PnPoly(): crossing number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  0 = outside, 1 = inside
// This code is patterned after [Franklin, 2000]
int PointInPolygonTest::cn_PnPoly(const Point& P, const Points& V)
{
    int cn = 0;    // the  crossing number counter

    // loop through all edges of the polygon
    for (std::size_t i = 0; i< V.size() - 1; ++i) {    // edge from V[i]  to V[i+1]
       if (((V[i].y <= P.y + ::EPS) && (V[i+1].y > P.y - ::EPS))     // an upward crossing
        || ((V[i].y > P.y - ::EPS) && (V[i+1].y <=  P.y + ::EPS))) { // a downward crossing
            // compute  the actual edge-ray intersect x-coordinate
            double vt = static_cast<double>((P.y  - V[i].y) / (V[i+1].y - V[i].y));
            if (P.x <  V[i].x + vt * (V[i+1].x - V[i].x) + ::EPS) // P.x < intersect
                 ++cn;   // a valid crossing of y=P.y right of P.x
        }
    }
    return cn & 1;    // 0 if even (out), and 1 if  odd (in)

}


// wn_PnPoly(): winding number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  wn = the winding number (=0 only when P is outside)
int PointInPolygonTest::wn_PnPoly( const Point& P, const Points& V)
{
    int wn = 0;    // the  winding number counter

    // loop through all edges of the polygon
    for (std::size_t i = 0; i < V.size() - 1; ++i) {   // edge from V[i] to  V[i+1]
        if (V[i].y <= P.y + ::EPS) {          // start y <= P.y
            if (V[i+1].y  > P.y - ::EPS)      // an upward crossing
                 if (isLeft( V[i], V[i+1], P) > -::EPS)  // P left of  edge
                     ++wn;            // have  a valid up intersect
        }
        else {                        // start y > P.y (no test needed)
            if (V[i+1].y  <= P.y + ::EPS)     // a downward crossing
                 if (isLeft( V[i], V[i+1], P) < ::EPS)  // P right of  edge
                     --wn;            // have  a valid down intersect
        }
    }
    return wn;
}



void PointInPolygonTest::setTolerance(double eps)
{
    ::EPS = eps;
}
