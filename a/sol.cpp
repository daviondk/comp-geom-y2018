#include <gmpxx.h>
#include "seg_intersection_tests.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct point {
    long double x;
    long double y;
};

struct segment {
    point p1;
    point p2;
};

int orientation(const segment& s, const point& p) {
    long double o = (p.x - s.p1.x) * (s.p2.y - s.p1.y) - (p.y - s.p1.y) * (s.p2.x - s.p1.x);
    if (o < -1e-9) {
        return -1;
    } else if (o > 1e-9) {
        return 1;
    } else {
        mpq_class px = (double) p.x;
        mpq_class sp1x = (double) s.p1.x;
        mpq_class sp2x = (double) s.p2.x;
        mpq_class sp2y = (double) s.p2.y;
        mpq_class sp1y = (double) s.p1.y;
        mpq_class py = (double) p.y;
        mpq_class o_mpq = (px - sp1x) * (sp2y - sp1y) - (py - sp1y) * (sp2x - sp1x);
        return mpq_sgn(o_mpq.get_mpq_t());
    }
}

bool between(const segment& s, const point& p) {
    const long double& lx = min(s.p1.x, s.p2.x);
    const long double& rx = max(s.p1.x, s.p2.x);
    const long double& ly = min(s.p1.y, s.p2.y);
    const long double& ry = max(s.p1.y, s.p2.y);
    return p.x >= lx && p.x <= rx && p.y >= ly && p.y <= ry;
}

int main() {
    int t;
    cin >> t;
    auto inp = genTest(t);
    for (uint i = 0; i < inp.size(); i += 8) {
        point p1 = {inp[i], inp[i + 1]};
        point p2 = {inp[i + 2], inp[i + 3]};
        point p3 = {inp[i + 4], inp[i + 5]};
        point p4 = {inp[i + 6], inp[i + 7]};
        segment seg1 = { p1, p2 };
        segment seg2 = { p3, p4 };
        int o1 = orientation(seg1, p3);
        int o2 = orientation(seg1, p4);
        int o3 = orientation(seg2, p1);
        int o4 = orientation(seg2, p2);
        bool pr1 = o1 != o2;
        bool pr2 = o3 != o4;

        if (pr1 & pr2) {
            cout << "Y";
        } else if (!pr1 && o1 != 0 || !pr2 && o3 != 0) {
            cout << "N";
        } else {
            if (between(seg1, p3) || between(seg1, p4) || between(seg2, p1) || between(seg2, p2)) {
                cout << "Y";
            } else {
                cout << "N";
            }
        }
    }
    cout << "\n";
}