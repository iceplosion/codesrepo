#include <iostream>
#include <cmath>

using namespace std;

const double EPS = 1e-8;

struct Point {
    double x, y;
    // constructor
    Point(double nx = 0, double ny = 0) : x(nx), y(ny) {}
    Point(const Point &p) : x(p.x), y(p.y) {}
    // dot product
    double operator* (const Point &p) const {return x * p.x + y * p.y;}
    // Override "==" operator
    bool operator == (const Point &p) const {return fabs(x - p.x) <= EPS && fabs(y - p.y) <= EPS;}
    // Override "!=" operator
    bool operator != (const Point &p) const {return fabs(x - p.x) > EPS || fabs(y - p.y) > EPS;}
    // set this point to 0
    void toZero() {x = y = 0;}
    // get reversed vector
    Point operator - () const {return Point(-x, -y);}
    // add
    Point operator + (const Point &p) const {return Point(x + p.x, y + p.y);}
    Point &operator += (const Point &p) {
        x += p.x, y += p.y;
        return *this;
    }
    // subtract
    Point operator - (const Point &p) const {return Point(x - p.x, y - p.y);}
    Point &operator -= (const Point &p) {
        x -= p.x, y -= p.y;
        return *this;
    }
    // multiply with scalar
    Point operator * (double a) const {return Point(x * a, y * a);}
    Point &operator *= (double a) {
        x *= a, y *= a;
        return *this;
    }
    // divide by scalar
    Point operator / (double a) const {
        double reva = 1. / a;
        return Point(x * reva, y * reva);
    }
    Point &operator /= (double a) {
        double reva = 1. / a;
        x *= reva, y *= reva;
        return *this;
    }
    // normalize vector
    void normalize() {
        double sq = x * x + y * y;
        // Note: zero vector can't be normalize by this way
        if (sq > 0.) {
            double rev = 1. / sqrt(sq);
            x *= rev, y *= rev;
        }
    }
    // rotation
    // param: degree in radian
    // | cos(a) -sin(a) ||x|  =  |x'|
    // | sin(a)  cos(a) ||y|     |y'|
    Point rotate(double deg) {
        double cosa = cos(deg), sina = sin(deg);
        return Point(cosa * x - sina * y, sina * x + cosa * y);
    }
};
// compute the norm(范数) of vector
inline double norm(const Point &p) {
    return sqrt(p.x * p.x + p.y * p.y);
}
inline double normSq(const Point &p) {
    return p.x * p.x + p.y * p.y;
}

// compute the cross product of vector a and b
// Note: the direction of cross product is not in this plane
inline double cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

// left multiply scalar
inline Point operator * (double a, const Point &p) {
    return Point(a * p.x, a * p.y);
}

// distSq
// return: Euclidean Distance
inline double distSq(const Point &a, const Point &b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}
inline double dist(const Point &a, const Point &b) {
    return sqrt(distSq(a, b));
}

inline ostream &operator << (ostream &os, const Point &p) {
    os << '(' << p.x << ", " << p.y << ')';
    return os;
}

// global const zero vector
const Point ZERO;

int main(void) {
    cout << "ZERO: " << ZERO << endl;
    Point p = Point(1, 0);
    cout << p << " -> " << (p = p.rotate(3.14159265 / 2)) << endl;
    cout << Point(0, 1) << " == " << p << "? "
        << (Point(0, 1) == p ? "Yes" : "No") << endl;
    return 0;
}
