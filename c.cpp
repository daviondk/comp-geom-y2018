#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

struct point {
    long long x;
    long long y;
};

long double min_len = numeric_limits<long double>::max();
unsigned long long min_len_sqr = numeric_limits<unsigned long long>::max();
point p1, p2;

struct points_comp {
    bool operator ()(const point& a, const point& b) {
        return a.x < b.x;
    }
};

struct points_comp_y {
    bool operator ()(const point& a, const point& b) {
        return a.y < b.y;
    }
};

inline unsigned long long sqr(long long x) {
    return x * x;
}
points_comp_y comp_y;
void rec(point points[], unsigned int l, unsigned int r) {
    if (r - l <= 4) {
        for (unsigned int i = l; i < r; i++) {
            for (unsigned int j = i + 1; j < r; j++) {
                unsigned long long len = sqr(points[i].x - points[j].x) + sqr(points[i].y - points[j].y);
                if (len < min_len_sqr) {
                    p1 = points[i];
                    p2 = points[j];
                    min_len_sqr = len;
                }
            }
        }
        min_len = sqrt(min_len_sqr);
        sort(points + l, points + r, comp_y);
        return;
    }
    unsigned int m = (l + r) / 2;
    long long m_x = points[m].x;
    rec(points, l, m);
    rec(points, m, r);
    unsigned int sz = r - l;
    point res[sz];
    unsigned int i = l;
    unsigned int j = m;
    for (int k = 0; i < m || j < r; k++) {
        if (i != m && (j == r || points[i].y <= points[j].y)) {
            if (m_x - points[i].x < min_len) {
//                unsigned int bottom = m;
//                while (bottom < r && points[i].y - points[bottom].y >= min_len) {
//                    bottom++;
//                }
                unsigned int l1 = m - 1;
                unsigned int r1 = r;
                while (l1 < r1 - 1) {
                    unsigned int m1 = (l1 + r1) / 2;
                    if (points[i].y - points[m1].y >= min_len) {
                        l1 = m1;
                    } else {
                        r1 = m1;
                    }
                }
                unsigned int bottom = r1;
//                unsigned int top = bottom;
//                while (top < r && points[top].y - points[i].y < min_len) {
//                    top++;
//                }

                l1 = bottom - 1;
                r1 = r;
                while (l1 < r1 - 1) {
                    unsigned int m1 = (l1 + r1) / 2;
                    if (points[m1].y - points[i].y < min_len) {
                        l1 = m1;
                    } else {
                        r1 = m1;
                    }
                }
                unsigned int top = r1;
                for (unsigned int i1 = bottom; i1 < top; i1++) {
                    unsigned long long len = sqr(points[i].x - points[i1].x) + sqr(points[i].y - points[i1].y);
                    if (len < min_len_sqr) {
                        p1 = points[i];
                        p2 = points[i1];
                        min_len_sqr = len;
                    }
                }
                min_len = sqrt(min_len_sqr);
            }
            res[k] = points[i];
            i++;
        } else {
            res[k] = points[j];
            j++;
        }
    }
    copy(res, res + sz, points + l);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    point points[n];
    for (int i = 0; i < n; i++) {
        long long x, y;
        cin >> x >> y;
        points[i] = {x, y};
    }
    points_comp points_x_comp;
    sort(points, points + n, points_x_comp);
    rec(points, 0, n);
    cout << p1.x << " " << p1.y << '\n';
    cout << p2.x << " " << p2.y << '\n';
}