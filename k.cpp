#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

struct point {
    long long x;
    long long y;
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

int orientation(const segment& s, const point& p) {
    long long o = pseudovec(vec(s), vec({s.p2, p}));
    if (o < 0) {
        return -1;
    } else if (o > 0) {
        return 1;
    }
    return 0;
}

point minPoint = {0, numeric_limits<long long>::max()};

long long sqr_len(const point& a) {
    return a.x * a.x + a.y * a.y;
}

struct simple_comp {
    bool operator()(const point& a, const point& b) const {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};

struct points_comp {
    bool operator ()(const point& a, const point& b) const {
        point left = {a.x - minPoint.x, a.y - minPoint.y};
        point right = {b.x - minPoint.x, b.y - minPoint.y};
        long long angle = pseudovec(left, right);
        if (angle == 0) {
            return sqr_len(left) < sqr_len(right);
        }
        return angle > 0;
    }
};

unsigned long long sqr(long long x) {
    return (unsigned long long)abs(x) * (unsigned long long)abs(x);
}

unsigned long long point_dist(const point& a, const point& b) {
    return sqr(a.x - b.x) + sqr(a.y - b.y);
}

long double ld_dist(const point& a, const point& b) {
    return hypotl((long double)(a.x - b.x), (long double)(a.y - b.y));
}

int comp_by_angle(const segment& s1, const segment& s2) {
    point a = vec(s1);
    point b = vec(s2);
    long long angle = pseudovec(a, b);
    if (angle == 0) {
        return 0;
    } else if (angle > 0) {
        return 1;
    }
    return -1;
}

long double eps = 1e-9 / 2.0;

long double sqrt_bin_search(long double val) {
    long double val_ld = val;
    long double l = 0;
    long double r = val_ld;
    while (r - l > eps) {
        long double m = (r + l) / 2;
        if (m * m < val_ld) {
            l = m;
        } else {
            r = m;
        }
    }
    return r;
}

int main() {
    int n;
    cin >> n;
    set<point, simple_comp> points;
    for (int i = 0; i < n; i++) {
        long long x, y;
        cin >> x >> y;
        points.insert({x, y});
        if (y < minPoint.y) {
            minPoint = {x, y};
        }
    }

    for (auto& p : points) {
        if (p.y == minPoint.y && p.x > minPoint.x) {
            minPoint = p;
        }
    }
    points.erase(minPoint);
    vector<point> st;
    unsigned long sz = points.size() + 1;
    st.reserve(sz);
    st.push_back(minPoint);
    for (const point& p : points) {
        st.push_back(p);
    }
    sort(st.begin() + 1, st.end(), points_comp());
    int lastIndex = 1;
    for (int i = 2; i < sz; i++) {
        while (orientation({st[lastIndex - 1], st[lastIndex]}, st[i]) <= 0 && lastIndex > 0) {
            lastIndex--;
        }
        swap(st[i], st[lastIndex + 1]);
        lastIndex++;
    }
    sz = lastIndex + 1;
    reverse(st.begin(), st.begin() + sz);
    int max_index = 0;
    if (sz == 1 || sz == 0) {
        long double res = 0;
        cout << fixed << setprecision(100) << (long double) 0.0 << endl;
        return 0;
    }
    if (sz == 2) {
        cout << fixed << setprecision(100) << sqrt_bin_search(point_dist(st[0], st[1])) << endl;
        return 0;
    }
    int first = 0;
    int second = 1;
    int cur = 0;
    while(true) {
        point cur_point_f = st[first];
        point next_point_f = st[(first + 1) % sz];
        point cur_point_s = st[second % sz];
        point next_point_s = st[(second + 1) % sz];
        if (comp_by_angle({cur_point_f, next_point_f}, {cur_point_s, next_point_s}) < 0) {
            second++;
        } else {
            break;
        }
    }
    cur = first;

    unsigned long long max_dist = point_dist(st[first], st[second % sz]);
    long double max_dist_ld = max_dist;

    while (second <= sz) {
        point cur_point_f = st[(first ) % sz];
        point cur_point_s = st[(second ) % sz];
        point next_point_f = st[(first + 1) % sz];
        point next_point_s = st[(second + 1) % sz];
        if (comp_by_angle({cur_point_f, next_point_f}, {cur_point_s, next_point_s}) <= 0) {
            second++;
            cur = second;
        } else {
            first++;
            cur = first;
        }
        cur_point_f = st[first % sz];
        cur_point_s = st[second % sz];
        unsigned long long cur_dist = point_dist(cur_point_f, cur_point_s);
        if (cur_dist > max_dist) {
            max_dist = cur_dist;
        }
        next_point_f = st[(first + 1) % sz];
        next_point_s = st[(second + 1) % sz];
        if (comp_by_angle({cur_point_f, next_point_f}, {cur_point_s, next_point_s}) == 0) {
            cur_dist = max(point_dist(cur_point_f, next_point_s), point_dist(next_point_f, cur_point_s));
            cur_dist = max(cur_dist, point_dist(next_point_f, next_point_s));
            if (cur_dist > max_dist) {
                max_dist = cur_dist;
            }
            if (first == cur) {
                second++;
            } else {
                first++;
            }
        }
    }
    max_dist_ld = max_dist;

    cout << setprecision(100) << sqrt_bin_search(max_dist_ld) << endl;
    return 0;
}