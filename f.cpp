#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip>

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
    bool operator()(const point& a, const point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};

struct points_comp {
    bool operator ()(const point& a, const point& b) {
        point left = {a.x - minPoint.x, a.y - minPoint.y};
        point right = {b.x - minPoint.x, b.y - minPoint.y};
        long long angle = pseudovec(left, right);
        if (angle == 0) {
            return sqr_len(left) < sqr_len(right);
        }
        return angle > 0;
    }
};

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
    cout << sz << endl;
    for (int i = 0; i < sz; i++) {
        cout << st[i].x << " " << st[i].y << endl;
    }
    long double s = 0;
    long long sum = 0;
    for (int i = 0; i < sz; i++) {
        unsigned int next = (i + 1) % sz;
        sum += (st[i].x + st[next].x) * (st[i].y - st[next].y);
    }
    s = (long double) abs(sum) * 0.5;
    cout << setprecision(20) << s << endl;
    return 0;
}