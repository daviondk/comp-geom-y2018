#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct point {
    long long x;
    long long y;
};

struct segment {
    point p1;
    point p2;
};

struct polygon {
    vector<point> points;
};

long long pseudovec(const point& p1, const point& p2) {
    return p1.x * p2.y - p2.x * p1.y;
}

point vec(const segment& s) {
    return { s.p2.x - s.p1.x, s.p2.y - s.p1.y };
}

int orientation(const segment& s, const point& p) {
    long long o = pseudovec(vec(s), vec({s.p2, p}));
    if (o < 0) {
        return -1;
    } else if (o > 0) {
        return 1;
    }
    return 0;
}

bool between(const segment& s, const point& p) {
    const long double& lx = min(s.p1.x, s.p2.x);
    const long double& rx = max(s.p1.x, s.p2.x);
    const long double& ly = min(s.p1.y, s.p2.y);
    const long double& ry = max(s.p1.y, s.p2.y);
    return p.x >= lx && p.x <= rx && p.y >= ly && p.y <= ry;
}


bool intersect(point& p1, point& p2, point& p3, point& p4) {
    segment seg1 = { p1, p2 };
    segment seg2 = { p3, p4 };
    int o1 = orientation(seg1, p3);
    int o2 = orientation(seg1, p4);
    int o3 = orientation(seg2, p1);
    int o4 = orientation(seg2, p2);
    bool pr1 = o1 != o2;
    bool pr2 = o3 != o4;

    if (pr1 & pr2) {
        return true;
    } else if (!pr1 && o1 != 0 || !pr2 && o3 != 0) {
        return false;
    } else {
        if (between(seg1, p3) || between(seg1, p4) || between(seg2, p1) || between(seg2, p2)) {
            return true;
        } else {
            return false;
        }
    }
}

bool in_polygon(polygon& a, point& p, point& m) {
    int intersectCount = 0;
    for (int i = 0; i < a.points.size(); i++) {
        point low_point = a.points[i];
        point high_point = a.points[(i + 1) % a.points.size()];
        if (low_point.y > high_point.y) {
            point temp = low_point;
            low_point = high_point;
            high_point = temp;
        }
        int o = orientation({ low_point, high_point }, p);
        if (o == 0) {
            return true;
        }
        if (intersect(low_point, high_point, p, m)) {
            if (orientation({p, m}, low_point) == 0) {
                continue;
            }
            intersectCount++;
        }
    }
    return intersectCount & 1;
}

int main() {
    int n;
    cin >> n;
    polygon poly;
    poly.points = vector<point>(n);
    point targetPoint;
    int minIndex = 0;
    long long x, y;
    cin >> x >> y;
    targetPoint = { x, y };
    point minPoint = { x, y };
    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        poly.points[i] = {x, y};
        if (x < poly.points[minIndex].x) {
            minIndex = i;
            minPoint.x = x;
        }
    }
    minPoint.x--;
    reverse(poly.points.begin(), poly.points.begin() + minIndex);
    reverse(poly.points.begin() + minIndex, poly.points.end());
    reverse(poly.points.begin(), poly.points.end());
    if (in_polygon(poly, targetPoint, minPoint)) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }
    return 0;
}