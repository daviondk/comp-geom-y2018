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

bool in_polygon(polygon& a, point& p) {
    int l = 1;
    int r = a.points.size();
    if (orientation({a.points[0], a.points[1]}, p) < 0
        || orientation({a.points[0], a.points.back()}, p) > 0) {
        return false;
    }
    while (l < r - 1) {
        int m = (l + r) / 2;
        if (orientation({a.points[0], a.points[m]}, p) < 0) {
            r = m;
        } else {
            l = m;
        }
    }
    return orientation({a.points[l], a.points[r]}, p) == 0 || !intersect(a.points[0], p, a.points[l], a.points[r]);
}

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    polygon poly;
    poly.points = vector<point>(n);
    vector<point> points(m);
    int minIndex = 0;
    for (int i = 0; i < n; i++) {
        long long x, y;
        cin >> x >> y;
        poly.points[i] = {x, y};
        if (x < poly.points[minIndex].x) {
            minIndex = i;
        }
    }
    reverse(poly.points.begin(), poly.points.begin() + minIndex);
    reverse(poly.points.begin() + minIndex, poly.points.end());
    reverse(poly.points.begin(), poly.points.end());
    for (int i = 0; i < m; i++) {
        long long x, y;
        cin >> x >> y;
        points[i] = { x, y };
    }
    int count = 0;
    for (int i = 0; i < m; i++) {
        count += in_polygon(poly, points[i]);
    }
    if (count < k) {
        cout << "NO" << endl;
    } else {
        cout << "YES" << endl;
    }
    return 0;
}