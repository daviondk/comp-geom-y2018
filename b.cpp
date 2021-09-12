#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

struct point {
    long double x;
    long double y;
};

struct segment {
    point p1;
    point p2;
};

bool between(const segment& s, const point& p) {
    const long double& lx = min(s.p1.x, s.p2.x);
    const long double& rx = max(s.p1.x, s.p2.x);
    const long double& ly = min(s.p1.y, s.p2.y);
    const long double& ry = max(s.p1.y, s.p2.y);
    return p.x >= lx && p.x <= rx && p.y >= ly && p.y <= ry;
}

long double point_dist(const point& a, const point& b) {
    return sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0));
}

long double pseudovec(const point& p1, const point& p2) {
    return p1.x * p2.y - p2.x * p1.y;
}

point vec(const segment& s) {
    return { s.p2.x - s.p1.x, s.p2.y - s.p1.y };
}

long double len(const segment& s) {
    return point_dist(s.p1, s.p2);
}

const long double eps = 1e-12;

long double point_line_dist(const point& p, const segment& s) {
    long double check = pseudovec(vec({s.p1, p}), vec(s));
    if (abs(check) < eps) {
        return 0;
    }
    point v1 = vec({p , s.p1});
    point v2 = vec({p, s.p2});
    long double s_len = len(s);
    return abs(pseudovec(v1, v2)) / s_len;
}

long double dot(const point& p1, const point& p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

long double point_seg_dist(const point& p, const segment& s) {

    long double check = pseudovec(vec({s.p1, p}), vec(s));
    if (abs(check) < eps) {
        if (between(s, p)) {
            return 0;
        } else {
            return min(point_dist(p, s.p1), point_dist(p, s.p2));
        }
    }

    long double dot1 = dot(vec(s), vec({s.p1, p}));
    long double dot2 = dot(vec({s.p2, s.p1}), vec({s.p2, p}));
    if (dot1 >= 0 && dot2 >= 0) {
        return point_line_dist(p, s);
    } else {
        return min(point_dist(p, s.p1), point_dist(p, s.p2));
    }
}

bool is_point_on_ray(const point& p, const segment& s) {
    long double check = pseudovec(vec({s.p1, p}), vec(s));
    bool onRay = true;
    if (abs(check) < eps) {
        if (s.p1.x < s.p2.x) {
            if (p.x < s.p1.x) {
                onRay = false;
            }
        } else {
            if (p.x > s.p1.x) {
                onRay = false;
            }
        }

        if (s.p1.y < s.p2.y) {
            if (p.y < s.p1.y) {
                onRay = false;
            }
        } else {
            if (p.y > s.p1.y) {
                onRay = false;
            }
        }
    }
    return onRay && (abs(check) < eps);
}

long double point_ray_dist(const point& p, const segment& s) {

    long double check = pseudovec(vec({s.p1, p}), vec(s));
    if (abs(check) < eps) {
        bool onRay = true;
        if (s.p1.x < s.p2.x) {
            if (p.x < s.p1.x) {
                onRay = false;
            }
        } else {
            if (p.x > s.p1.x) {
                onRay = false;
            }
        }

        if (s.p1.y < s.p2.y) {
            if (p.y < s.p1.y) {
                onRay = false;
            }
        } else {
            if (p.y > s.p1.y) {
                onRay = false;
            }
        }

        if (onRay) {
            return 0;
        } else {
            return point_dist(p, s.p1);
        }
    }

    long double dot1 = dot(vec(s), vec({s.p1, p}));
    if (dot1 >= 0) {
        return point_line_dist(p, s);
    } else {
        return point_dist(p, s.p1);
    }
}

point intersect_lines(const segment& s1, const segment& s2) {
//    long double ps1 = pseudovec(vec({s2.p1, s1.p1}), vec(s2));
//    long double ps2 = pseudovec(vec({s2.p1, s1.p2}), vec(s2));
    long double ps1 = pseudovec(vec(s1), vec(s2));
    if (abs(ps1) < eps ) {
        return {INT_MAX, INT_MAX};
    }
    long double mult = pseudovec(vec(s2), vec({s2.p1, s1.p1})) / pseudovec(vec(s1), vec(s2));
    point p1p2 = vec(s1);
    point p1m = {mult * p1p2.x, mult * p1p2.y};
    return {p1m.x + s1.p1.x, p1m.y + s1.p1.y};
}

long double seg_ray_dist(const segment& s1, const segment& s2) {
//    long double ps1 = pseudovec(vec({s2.p1, s1.p1}), vec(s2));
//    long double ps2 = pseudovec(vec({s2.p1, s1.p2}), vec(s2));
//    if (ps1 * ps2 <= 0) {
//        return 0;
//    }
    point m = intersect_lines(s1, s2);

    long double xmax1 = max(s1.p1.x, s1.p2.x);
    long double xmin1 = min(s1.p1.x, s1.p2.x);
    long double ymax1 = max(s1.p1.y, s1.p2.y);
    long double ymin1 = min(s1.p1.y, s1.p2.y);

    if (m.x != INT_MAX && m.x >= xmin1 && m.x <= xmax1 && m.y >= ymin1 && m.y <= ymax1) {
        if ((m.x > s2.p1.x) == (s2.p2.x > s2.p1.x) && (m.y > s2.p1.y) == (s2.p2.y > s2.p1.y)) {
            return 0;
        }
    }
    return min(min(point_ray_dist(s1.p1, s2), point_ray_dist(s1.p2, s2)), point_seg_dist(s2.p1, s1));
}

long double seg_line_dist(const segment& s1, const segment& s2) {
//    long double ps1 = pseudovec(vec({s2.p1, s1.p1}), vec(s2));
//    long double ps2 = pseudovec(vec({s2.p1, s1.p2}), vec(s2));
//    if (ps1 * ps2 <= 0) {
//        return 0;
//    }
    point m = intersect_lines(s1, s2);

    long double xmax1 = max(s1.p1.x, s1.p2.x);
    long double xmin1 = min(s1.p1.x, s1.p2.x);
    long double ymax1 = max(s1.p1.y, s1.p2.y);
    long double ymin1 = min(s1.p1.y, s1.p2.y);

    if (m.x != INT_MAX && m.x >= xmin1 && m.x <= xmax1 && m.y >= ymin1 && m.y <= ymax1) {
        return 0;
    }
    return min(point_line_dist(s1.p1, s2), point_line_dist(s1.p2, s2));
}

long double seg_seg_dist(const segment& s1, const segment& s2) {
    point m = intersect_lines(s1, s2);

    long double xmax1 = max(s1.p1.x, s1.p2.x);
    long double xmin1 = min(s1.p1.x, s1.p2.x);
    long double ymax1 = max(s1.p1.y, s1.p2.y);
    long double ymin1 = min(s1.p1.y, s1.p2.y);

    if (m.x != INT_MAX && m.x >= xmin1 && m.x <= xmax1 && m.y >= ymin1 && m.y <= ymax1) {

        long double xmax2 = max(s2.p1.x, s2.p2.x);
        long double xmin2 = min(s2.p1.x, s2.p2.x);
        long double ymax2 = max(s2.p1.y, s2.p2.y);
        long double ymin2 = min(s2.p1.y, s2.p2.y);
        if (m.x >= xmin2 && m.x <= xmax2 && m.y >= ymin2 && m.y <= ymax2) {
            return 0;
        }
    }
    return min(min(point_seg_dist(s1.p1, s2), point_seg_dist(s1.p2, s2)), min(point_seg_dist(s2.p1, s1), point_seg_dist(s2.p2, s1)));
}

long double line_line_dist(const segment& s1, const segment& s2) {
//    long double ps1 = pseudovec(vec({s2.p1, s1.p1}), vec(s2));
//    long double ps2 = pseudovec(vec({s2.p1, s1.p2}), vec(s2));
//    if (ps1 * ps2 <= 0) {
//        return 0;
//    }
    point m = intersect_lines(s1, s2);
    if (m.x != INT_MAX) {
        return 0;
    }
    return point_line_dist(s1.p1, s2);
}

long double ray_line_dist(const segment& s1, const segment& s2) {
    if (is_point_on_ray(s1.p1, s2) || is_point_on_ray(s1.p1, {s2.p2, s2.p1})) {
        return 0;
    }
    point m = intersect_lines(s1, s2);
    if (m.x != INT_MAX && dot(vec(s1), vec({s1.p1, m})) >= 0) {
        return 0;
    }

    return point_line_dist(s1.p1, s2);
}

long double ray_ray_dist(const segment& s1, const segment& s2) {
//    long double ps1 = pseudovec(vec(s1), vec(s2));
//    if (abs(ps1) < eps) {
//        long double ps_line = pseudovec(vec({s1.p1, s2.p1}), vec(s1));
//        if (abs(ps_line) < eps) {
//            long double dot_rays = dot(vec(s1), vec(s2));
//            if ()
//        }
//    }
    if (is_point_on_ray(s1.p1, s2) || is_point_on_ray(s2.p1, s1)) {
        return 0;
    }
    point m = intersect_lines(s1, s2);
//    cout << m.x << " " << m.y << endl;
    if (m.x != INT_MAX && dot(vec(s1), vec({s1.p1, m})) >= 0 && dot(vec(s2), vec({s2.p1, m})) >= 0) {
        return 0;
    }

    return min(point_ray_dist(s1.p1, s2), point_ray_dist(s2.p1, s1));
}

int main() {
    point a, b, c, d;
    cin >> a.x >> a.y;
    cin >> b.x >> b.y;
    cin >> c.x >> c.y;
    cin >> d.x >> d.y;
//    cout << seg_ray_dist({a, b}, {c, d});
    cout << setprecision(20) << point_dist(a, c) << endl;
    cout << setprecision(20) << point_seg_dist(a, {c, d}) << endl;
    cout << setprecision(20) << point_ray_dist(a, {c, d}) << endl;
    cout << setprecision(20) << point_line_dist(a, {c, d}) << endl;
    cout << setprecision(20) << point_seg_dist(c, {a, b}) << endl;
    cout << setprecision(20) << seg_seg_dist({a, b}, {c, d}) << endl;
    cout << setprecision(20) << seg_ray_dist({a, b}, {c, d}) << endl;
    cout << setprecision(20) << seg_line_dist({a, b}, {c, d}) << endl;
    cout << setprecision(20) << point_ray_dist(c, {a, b}) << endl;
    cout << setprecision(20) << seg_ray_dist({c, d}, {a, b}) << endl;
    cout << setprecision(20) << ray_ray_dist({a, b}, {c, d}) << endl;
    cout << setprecision(20) << ray_line_dist({a, b}, {c, d}) << endl;
    cout << setprecision(20) << point_line_dist(c, {a, b}) << endl;
    cout << setprecision(20) << seg_line_dist({c, d}, {a, b}) << endl;
    cout << setprecision(20) << ray_line_dist({c, d}, {a, b}) << endl;
    cout << setprecision(20) << line_line_dist({a, b}, {c, d}) << endl;
}