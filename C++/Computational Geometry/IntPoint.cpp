#include <iostream>

using namespace std;

struct IntPoint {
    int x, y;
    // constructor
    IntPoint(int nx = 0, int ny = 0) : x(nx), y(ny) {}
    IntPoint(const IntPoint &p) : x(p.x), y(p.y) {}
    // dot product
    int operator * (const IntPoint &p) const {
        return x * p.x + y * p.y;
    }
    // override "==" operator
    bool operator == (const IntPoint &p) const {
        return x == p.x && y == p.y;
    }
    bool operator != (const IntPoint &p) const {
        return x != p.x || y != p.y;
    }
    // set zero
    void toZero() {x = y = 0;}
    // compute reversed vector
    IntPoint operator -() const {return IntPoint(-x, -y);}
    //add
    IntPoint operator + (const IntPoint &p) const {
        return IntPoint(x + p.x, y + p.y);
    }
    IntPoint &operator += (const IntPoint &p) {
        x += p.x, y += p.y;
        return *this;
    }
    // subtract
    IntPoint operator - (const IntPoint &p) const {
        return IntPoint(x - p.x, y - p.y);
    }
    IntPoint &operator -= (const IntPoint &p) {
        x -= p.x, y -= p.y;
        return *this;
    }
    // multiply with scalar
    IntPoint operator *(int a) const {
        return IntPoint(x * a, y * a);
    }
};

// distance
// return: Manhattan Distance
inline int dist(IntPoint &a, IntPoint &b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// distSq
// return: square of Euclidean Distance
inline int distSq(IntPoint &a, IntPoint &b) {
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

// cross product
inline int cross(IntPoint &a, IntPoint &b) {
    return a.x * b.y - a.y * b.x;
}

// output operator
inline ostream &operator << (ostream &os, const IntPoint &p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

// global ZERO integer vector
const IntPoint ZERO;

int main(void) {
    cout << ZERO << endl;
    IntPoint a, b, c, t;
    cout << (a = IntPoint(3, 5)) << " + "
        << (b = IntPoint (4, 6)) << " = "
        << (c = a + b) << endl;
    cout << "Manhattan Distance of a and b: " << dist(a, b) << endl;
    cout << "Square of Euclidean Distance of a and b: " << distSq(a, b) << endl;
    cout << c << " == " << (t = IntPoint(7, 11)) << "? " << (c == t ? "Yes" : "No") << endl;
    cout << c << " != " << t << "? " << (c != t ? "Yes" : "No") << endl;
    return 0;
}
