#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
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

struct event {
    segment s;
    int n;
};

struct full_event {
    event e;
    bool isStart;
};

long double t = -10001;

const long double eps = 1e-10;

struct event_sort_comp {
    bool operator ()(const full_event& a, const full_event& b) const {
        point f = a.isStart ? a.e.s.p1 : a.e.s.p2;
        point s = b.isStart ? b.e.s.p1 : b.e.s.p2;
        long long dt = f.x - s.x;
        if (dt == 0) {
            if (a.isStart) {
                return true;
            }
            return false;
        }
        return dt < 0;
    }
};

long double getPointYNow(const event& a) {
    if (abs(a.s.p1.x - a.s.p2.x) < eps) {
        return a.s.p1.y;
    }
    return a.s.p1.y + (a.s.p2.y - a.s.p1.y) * (t - a.s.p1.x) / (a.s.p2.x - a.s.p1.x);
}

struct event_set_comp {
    bool operator()(const event& a, const event& b) const {
        long double f_y = getPointYNow(a);
        long double s_y = getPointYNow(b);
        if (abs(f_y - s_y) < eps) {
            if (t == b.s.p2.x) {
                if (t == a.s.p2.x) {
                    return a.n < b.n;
                }
                return true;
            }
            if (t == a.s.p2.x) {
                return false;
            }
            return a.n < b.n;
        }
        return f_y < s_y - eps;
    };
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

struct point_d {
    long double x;
    long double y;
};

point_d intersect_lines(const segment& s1, const segment& s2) {
    long double ps1 = pseudovec(vec(s1), vec(s2));
    if (abs(ps1) < eps ) {
        return {numeric_limits<int>::max(), numeric_limits<int>::max()};
    }
    long double mult = pseudovec(vec(s2), vec({s2.p1, s1.p1})) / pseudovec(vec(s1), vec(s2));
    point p1p2 = vec(s1);
    point_d p1m = {mult * p1p2.x, mult * p1p2.y};
    return {p1m.x + s1.p1.x, p1m.y + s1.p1.y};
}


bool intersect_segments(const segment& s1, const segment& s2) {
    point_d m = intersect_lines(s1, s2);

    long double xmax1 = max(s1.p1.x, s1.p2.x);
    long double xmin1 = min(s1.p1.x, s1.p2.x);
    long double ymax1 = max(s1.p1.y, s1.p2.y);
    long double ymin1 = min(s1.p1.y, s1.p2.y);

    if (m.x != numeric_limits<int>::max() && m.x >= xmin1 - eps && m.x <= xmax1 + eps && m.y >= ymin1 - eps && m.y <= ymax1 + eps) {

        long double xmax2 = max(s2.p1.x, s2.p2.x);
        long double xmin2 = min(s2.p1.x, s2.p2.x);
        long double ymax2 = max(s2.p1.y, s2.p2.y);
        long double ymin2 = min(s2.p1.y, s2.p2.y);
        if (m.x >= xmin2 - eps && m.x <= xmax2 + eps && m.y >= ymin2 - eps && m.y <= ymax2 + eps) {
            return true;
        }
    }
    return min(min(point_seg_dist(s1.p1, s2), point_seg_dist(s1.p2, s2)), min(point_seg_dist(s2.p1, s1), point_seg_dist(s2.p2, s1))) < eps;
}

vector<int> exist(vector<segment>& segments) {
    int n = segments.size();
    vector<full_event> sorted;
    for (int i = 0; i < n; i++) {
        event ev = {segments[i], i + 1};
        sorted.push_back({ev, false});
        sorted.push_back({ev, true});
    }
    event_sort_comp comp;
    stable_sort(sorted.begin(), sorted.end(), comp);

    set<event, event_set_comp> line;
    vector<set<event>::iterator> pos(n);
    for (int i = 0; i < 2 * n; i++) {
        event cur = sorted[i].e;
        t = sorted[i].isStart ? cur.s.p1.x : cur.s.p2.x;
        if (sorted[i].isStart) {
            auto it = line.insert(cur).first;
            pos[cur.n - 1] = it;
            if (it != line.begin()) {
                auto prev_it = prev(it);
                if (intersect_segments(prev_it->s, cur.s)) {
                    cout << "YES" << endl;
                    cout << prev_it->n << " " << cur.n << endl;
                    int f = prev_it->n;
                    int s = cur.n;
                    vector<int> res;
                    res.push_back(f);
                    res.push_back(s);
                    line.clear();
                    return res;
                }
            }
            auto next_it = next(it);
            if (next_it != line.end()) {
                if (intersect_segments(next_it->s, cur.s)) {
                    cout << "YES" << endl;
                    cout << next_it->n << " " << cur.n << endl;
                    int f = next_it->n;
                    int s = cur.n;
                    vector<int> res;
                    res.push_back(f);
                    res.push_back(s);
                    line.clear();
                    return res;
                }
            }
        } else {
            auto it = pos[cur.n - 1];
            if (it != line.begin()) {
                auto prev_it = prev(it);
                if (it != line.end()) {
                    auto next_it = next(it);
                    if (next_it != line.end()) {
                        if (intersect_segments(prev_it->s, next_it->s)) {
                            cout << "YES" << endl;
                            cout << prev_it->n << " " << next_it->n << endl;
                            int f = prev_it->n;
                            int s = next_it->n;
                            vector<int> res;
                            res.push_back(f);
                            res.push_back(s);
                            line.clear();
                            return res;
                        }
                    }
                }
            }
            line.erase(pos[cur.n - 1]);
        }
    }
    line.clear();
    cout << "NO" << endl;
    return {};
}

int main() {
    int n;
    cin >> n;
    vector<segment> segments(n);
    for (int i = 0; i < n; i++) {
        segment s;
        cin >> s.p1.x >> s.p1.y >> s.p2.x >> s.p2.y;
        if (s.p2.x < s.p1.x) {
            swap(s.p1, s.p2);
        }
        segments[i] = s;
    }
    exist(segments);
    return 0;
}