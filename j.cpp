#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <cmath>
#include <list>

using namespace std;

long double eps = 1e-10;

struct point {
    long double x;
    long double y;
    point(long double x, long double y) {
        this->x = x;
        this->y = y;
    }

    bool near(const point& p) {
        return abs(p.x - x) < eps && abs(p.y - y) < eps;
    }

    point operator+(const point & a) const
    {
        return {x + a.x, y + a.y};
    }
};

struct segment {
    point p1;
    point p2;
};

long double pseudovec(const point& p1, const point& p2) {
    return p1.x * p2.y - p2.x * p1.y;
}

point vec(const segment& s) {
    return { s.p2.x - s.p1.x, s.p2.y - s.p1.y };
}

point intersect_lines(const segment& s1, const segment& s2) {
    long double ps1 = pseudovec(vec(s1), vec(s2));
    if (abs(ps1) < eps ) {
        return {numeric_limits<long double>::infinity(), numeric_limits<long double>::infinity()};
    }
    long double mult = pseudovec(vec(s2), vec({s2.p1, s1.p1})) / pseudovec(vec(s1), vec(s2));
    point p1p2 = vec(s1);
    point p1m = {mult * p1p2.x, mult * p1p2.y};
    return {p1m.x + s1.p1.x, p1m.y + s1.p1.y};
}

point intersect(const segment& s1, const segment& s2) {
    point m = intersect_lines(s1, s2);

    long double xmax1 = max(s1.p1.x, s1.p2.x);
    long double xmin1 = min(s1.p1.x, s1.p2.x);
    long double ymax1 = max(s1.p1.y, s1.p2.y);
    long double ymin1 = min(s1.p1.y, s1.p2.y);

    if (m.x < numeric_limits<long double>::infinity() && m.x >= xmin1 - eps && m.x <= xmax1 + eps && m.y >= ymin1 - eps && m.y <= ymax1 + eps) {
        return m;
    }
    return {numeric_limits<long double>::infinity(), numeric_limits<long double>::infinity()};
}

struct facet {
    vector<point> points;
    facet(const vector<point>& points) {
        this->points = points;
    }
    pair<facet, facet> split(point p1, point p2) {
        int n = points.size();
        int first_index = -1;
        int second_index = -1;
        vector<point> first, second;
        vector<point> m_v;
        int intersect_point = 0;
        for (int i = 0; i < n; i++) {
            point f = points[i];
            point s = points[(i + 1) % n];
            segment seg = {f, s};
            point m = intersect(seg, {p1, p2});
            if (first_index != -1 && second_index == -1) {
                first.push_back(f);
            }
            if (m.x < numeric_limits<long double>::infinity()) {
                bool isPointViewed = false;
                for (auto& p : m_v) {
                    if (m.near(p)) {
                        isPointViewed = true;
                        break;
                    }
                }
                if (isPointViewed) {
                    continue;
                }
                m_v.push_back(m);
                if (first_index == -1) {
                    if (!m.near(s)) {
                        first.push_back(m);
                    } else {
                        intersect_point++;
                    }
                    if (!m.near(f)) {
                        second.push_back(m);
                    } else {
                        intersect_point++;
                    }
                    first_index = i;
                } else {
                    if (!m.near(f)) {
                        first.push_back(m);
                    } else {
                        intersect_point++;
                    }
                    if (!m.near(s)) {
                        second.push_back(m);
                    } else {
                        intersect_point++;
                    }
                    second_index = i;
                }
            }
        }
        if (first_index != -1 && second_index != -1) {
            for (int i = (second_index + 1) % n; i != (first_index + 1) % n; i = (i + 1) % n) {
                second.push_back(points[i]);
            }
            return make_pair(first, second);
        }
        return make_pair(facet({}), facet({}));
    }
    bool empty() {
        return points.empty();
    }
};

long double max_val = 1e7;

point left_bottom(-max_val, -max_val);
point right_bottom(max_val, -max_val);
point right_top(max_val, max_val);
point left_top(-max_val, max_val);

long double s_eps = 1e-8;

long double calc_s(const vector<point>& points) {
    long double sum = 0;
    int sz = points.size();
    for (int i = 0; i < sz; i++) {
        point p = points[i];
        if (abs(abs(p.x) - max_val) < eps || abs(abs(p.y) - max_val) < eps) {
            return numeric_limits<long double>::infinity();
        }
        unsigned int next = (i + 1) % sz;
        sum += (p.x + points[next].x) * (p.y - points[next].y);
    }
    long double s = abs(sum) * 0.5;
    if (s < s_eps) {
        return numeric_limits<long double>::infinity();
    }
    return s;
}

int main() {
    int n;
    facet main_facet({left_bottom, right_bottom, right_top, left_top});
    cin >> n;
    list<facet> facets;
    facets.push_back(main_facet);
    for (int i = 0; i < n; i++) {
        long double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        auto it = facets.begin();
        while (it != facets.end()) {
            auto next_it = next(it);
            pair<facet, facet> split = it->split({x1, y1}, {x2, y2});
            if (!split.first.empty() && !split.second.empty()) {
                facets.erase(it);
                facets.push_front(split.first);
                facets.push_front(split.second);
            }
            it = next_it;
        }
    }
    vector<long double> areas;
    for (auto& f : facets) {
        long double s = calc_s(f.points);
//        for (auto& p : f.points) {
//            cout << "{" << p.x << " " << p.y << "} ";
//        }
//        cout << endl;
        if (s < numeric_limits<long double>::infinity()) {
            areas.push_back(s);
        }
    }
    printf("%d\n", (int) areas.size());
    sort(areas.begin(), areas.end());
    for (auto &s : areas) {
        printf("%.10Lf\n", s);
    }
    return 0;
}