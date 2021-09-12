#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

struct point {
    long long x;
    long long y;

    point operator+(const point & a) const
    {
        return {x + a.x, y + a.y};
    }
};

struct segment {
    point p1;
    point p2;
};

long long pseudovec(const point& p1, const point& p2) {
    return p1.x * p2.y - p2.x * p1.y;
}

point vec(const segment& s) {
    return { s.p2.x - s.p1.x, s.p2.y - s.p1.y };
}

long double point_dist(const point& a, const point& b) {
    return sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0));
}

long double len(const segment& s) {
    return point_dist(s.p1, s.p2);
}

long double point_line_dist(const point& p, const segment& s) {
    point v1 = vec({p , s.p1});
    point v2 = vec({p, s.p2});
    long double s_len = len(s);
    return abs(pseudovec(v1, v2)) / s_len;
}

long double dot(const point& p1, const point& p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

long double point_seg_dist(const point& p, const segment& s) {

    long double dot1 = dot(vec(s), vec({s.p1, p}));
    long double dot2 = dot(vec({s.p2, s.p1}), vec({s.p2, p}));
    if (dot1 >= 0 && dot2 >= 0) {
        return point_line_dist(p, s);
    } else {
        return min(point_dist(p, s.p1), point_dist(p, s.p2));
    }
}

long double seg_seg_dist(const segment& s1, const segment& s2) {
    return min(min(point_seg_dist(s1.p1, s2), point_seg_dist(s1.p2, s2)), min(point_seg_dist(s2.p1, s1), point_seg_dist(s2.p2, s1)));
}

bool comp_by_angle(const segment& s1, const segment& s2) {
    point a = vec(s1);
    point b = vec(s2);
    long long angle = pseudovec(a, b);
    if (angle == 0) {
        return false;
    }
    return angle > 0;
}

const long double pi = atan(1.0) * 4.0;
long double calcAngleTan(const segment& s1) {
    point p = vec(s1);
    if(p.x == 0 && p.y == 0) {
        return 1e9;
    }
    long double a1 = atan2(p.y, p.x);
    if (a1 < 0) {
        return a1 + 2.0 * pi;
    }
    return a1;
}

bool comp_by_angle_tan(const segment& s1, const segment& s2) {
    long double a1 = calcAngleTan(s1);
    long double a2 = calcAngleTan(s2);
    return a1 < a2;
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

bool intersect(const point& p1, const point& p2, const point& p3, const point& p4) {
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

bool in_polygon(const vector<point>& a, const point& p) {
    int l = 1;
    int r = a.size();
    if (orientation({a[0], a[1]}, p) < 0
        || orientation({a[0], a.back()}, p) > 0) {
        return false;
    }
    while (l < r - 1) {
        int m = (l + r) / 2;
        if (orientation({a[0], a[m]}, p) < 0) {
            r = m;
        } else {
            l = m;
        }
    }
    return orientation({a[l], a[r]}, p) == 0 || !intersect(a[0], p, a[l], a[r]);
}

long double min_dist(point &p, vector<point>& poly) {
    long double min_dist = numeric_limits<long double>::max();
    if (poly.size() == 1) {
        return point_dist(p, poly[0]);
    }

    if (in_polygon(poly, p)) {
        return 0;
    }

    for (int i = 0; i < poly.size(); i++) {
        long double cur_dist = point_seg_dist(p, {poly[i], poly[(i + 1) % poly.size()]});
        if (cur_dist < min_dist) {
            min_dist = cur_dist;
        }
    }
    return min_dist;
}

long double min_seg_distance(const segment& s, vector<point>& poly) {
    long double min_dist = numeric_limits<long double>::max();

    if (in_polygon(poly, s.p1) && in_polygon(poly, s.p2)) {
        return 0;
    }

    for (int i = 0; i < poly.size(); i++) {
        long double cur_dist = seg_seg_dist(s, {poly[i], poly[(i + 1) % poly.size()]});
        if (cur_dist < min_dist) {
            min_dist = cur_dist;
        }
    }
    return min_dist;
}

int main() {
    int n;
    cin >> n;
    vector<point> a(n);
    int min_index_a = 0;
    for (int i = 0; i < n; i++) {
        long long x, y;
        cin >> x >> y;
        a[i] = {x, y};
    }
    reverse(a.begin(), a.end());
    for (int i = 0; i < n; i++) {
        if (a[i].x < a[min_index_a].x) {
            min_index_a = i;
        }
    }
    for (int i = 0; i < n; i++) {
        point p = a[i];
        if (p.x == a[min_index_a].x && p.y < a[min_index_a].y) {
            min_index_a = i;
        }
    }
    reverse(a.begin(), a.begin() + min_index_a);
    reverse(a.begin() + min_index_a, a.end());
    reverse(a.begin(), a.end());
    int m;
    cin >> m;
    vector<point> b(m);
    int min_index_b = 0;
    for (int i = 0; i < m; i++) {
        long long x, y;
        cin >> x >> y;
        b[i] = {x, y};
    }
    if (n == 1) {
        cout << fixed << setprecision(20) << min_dist(a[0], b);
        return 0;
    }
    if (m == 1) {
        cout << fixed << setprecision(20) << min_dist(b[0], a);
        return 0;
    }
    if (n == 2) {
        cout << fixed << setprecision(20) << min_seg_distance({a[0], a[1]}, b);
        return 0;
    }
    if (m == 2) {
        cout << fixed << setprecision(20) << min_seg_distance({b[0], b[1]}, a);
        return 0;
    }
    for (int i = 0; i < m; i++) {
        b[i] = {-b[i].x, -b[i].y};
    }
    reverse(b.begin(), b.end());
    for (int i = 0; i < m; i++) {
        if (b[i].x < b[min_index_b].x) {
            min_index_b = i;
        }
    }
    for (int i = 0; i < m; i++) {
        point p = b[i];
        if (p.x == b[min_index_b].x && p.y < b[min_index_b].y) {
            min_index_b = i;
        }
    }
    reverse(b.begin(), b.begin() + min_index_b);
    reverse(b.begin() + min_index_b, b.end());
    reverse(b.begin(), b.end());

    vector<point> sum;
    point start = {0, 0};
    int i = 0;
    int j = 0;
    while (i < n || j < m) {
        int i_cur = i % n;
        int j_cur = j % m;
        sum.push_back(a[i_cur] + b[j_cur]);
        int i_next = (i + 1) % n;
        int j_next = (j + 1) % m;
        if (comp_by_angle({a[i_cur], a[i_next]}, {b[j_cur], b[j_next]})) {
            i++;
        } else if (comp_by_angle({b[j_cur], b[j_next]}, {a[i_cur], a[i_next]})) {
            j++;
        } else {
            i++;
            j++;
        }
    }
    cout << fixed << setprecision(20) << min_dist(start, sum);
    return 0;
}