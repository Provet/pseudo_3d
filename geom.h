#pragma once

#include <algorithm>
#include <math.h>
#include <memory>

using namespace std;

const double EPS = 1e-9;

bool LessOrEqual(double lhs, double rhs) {
    return lhs < rhs + EPS;
}

double DegreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

class Point{
public:
    Point() {}

    Point(double x, double y)
        : x(x)
        , y(y)
    {   }
    
    double x;
    double y;
};

class Vector{
public:
    Vector() {}

    Vector(Point a, Point b) {
        x = b.x - a.x;
        y = b.y - a.y;
    }

    Vector(double x, double y)
        : x(x)
        , y(y)
    {   }

    Point Shift(const Point& p) const {
        return Point(p.x + x, p.y + y);
    }

    double Len() const {
        return sqrt(x * x + y * y);
    }

    void Rotate(double angle) {
        double newX = x * cos(angle) - y * sin(angle);
        double newY = x * sin(angle) + y * cos(angle);
        x = newX;
        y = newY;
    }

    static double DotProduct(const Vector& lhs, const Vector& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }
    
    double x;
    double y;
};

class Line{
public:
    Line() {}
    
    Line(const Point& a, const Point& b) {
        A = a.y - b.y;
        B = b.x - a.x;
        C = -b.x * a.y + b.y * a.x;
    }

    static Point Intersection(const Line& lhs, const Line& rhs) {
        Point p;
        p.x = -(lhs.C * rhs.B - rhs.C * lhs.B) / (lhs.A * rhs.B - rhs.A * lhs.B);
        p.y = -(lhs.A * rhs.C - rhs.A * lhs.C) / (lhs.A * rhs.B - rhs.A * lhs.B);
        return p;
    }

    double A;
    double B;
    double C;
};

class Segment{
public:
    Segment() {}

    Segment(const Point& a, const Point& b) 
        : p1(a)
        , p2(b)
    {   }

    Line GetLine() const {
        return Line(p1, p2);
    }
    
    static shared_ptr<Point> Intersection(const Segment& lhs, const Line& rhs) {
        Line lhsLine = lhs.GetLine();
        shared_ptr<Point> linesIntersection = make_shared<Point>(Line::Intersection(lhsLine, rhs));
        
        static auto IsPointOnSegment = [](const Segment& s, const Point& p) -> bool {
            double minx = min(s.p1.x, s.p2.x);
            double miny = min(s.p1.y, s.p2.y);
            double maxx = max(s.p1.x, s.p2.x);
            double maxy = max(s.p1.y, s.p2.y);
            return (LessOrEqual(minx, p.x) && LessOrEqual(p.x, maxx) && LessOrEqual(miny, p.y) && LessOrEqual(p.y, maxy));
        };

        if (IsPointOnSegment(lhs, *linesIntersection)) {
            return linesIntersection;
        }

        return nullptr;
    }
    
    Point p1;
    Point p2;
};

