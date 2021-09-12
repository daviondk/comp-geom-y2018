#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cassert>

using namespace std;

struct point {
    long long x;
    long long y;
    point* prev;
    point* next;
    int n;

    void setPrev(point* prev ) {
        this->prev = prev;
        prev->next = this;
    }

    void setNext(point* next ) {
        this->next = next;
        next->prev = this;
    }

    point operator+(const point & a) const
    {
        return {x + a.x, y + a.y};
    }
};

long long pseudovec(const point& p1, const point& p2) {
    return p1.x * p2.y - p2.x * p1.y;
}

point vec(const point& p1, const point& p2) {
    return { p2.x - p1.x, p2.y - p1.y };
}

int orientation(const point& p1, const point& p2, const point& p) {
    long long o = pseudovec(vec(p1 , p2), vec(p2, p));
    if (o < 0) {
        return -1;
    } else if (o > 0) {
        return 1;
    }
    return 0;
}

struct triangle {
    point* a, *b, *c;
    bool inside(point *p, int clockwise) {
        int o1 = orientation(*a, *b, *p);
        int o2 = orientation(*b, *c, *p);
        int o3 = orientation(*c, *a, *p);
        return (o1 * clockwise <= 0 && o2 * clockwise <= 0 && o3 * clockwise <= 0);
    }
};


void clear(point* head) {
    head->prev->next = nullptr;
    while (head != nullptr) {
        point* temp = head;
        head = head->next;
        delete temp;
    }
}

vector<pair<int, int>> getTriangulation(point* head, int n, int clockwise) {
    vector<pair<int, int>> result;

    int count_diag = 0;
    int count_bad = 0;
    while (count_diag < n - 3) {
        if (count_bad >= n) {
            result.clear();
            result.emplace_back(-1, -1);
            break;
        }
        bool canAddDiag = false;
        if (orientation(*head->prev, *head->next, *head) * clockwise > 0) {
            canAddDiag = true;
            triangle cur_triangle = {head->prev, head, head->next};
            point* cur_head = head->next->next;
            while (cur_head != head->prev) {
                if (cur_triangle.inside(cur_head, clockwise)) {
                    canAddDiag = false;
                    break;
                }
                cur_head = cur_head->next;
            }
            if (canAddDiag) {
                result.emplace_back(head->prev->n, head->next->n);
//                cout << head->prev->n << " " << head->next->n << endl;
                head->prev->setNext(head->next);
                point* temp = head;
                head = head->next;
                delete temp;
                count_diag++;
            }
        }
        if (!canAddDiag) {
            head = head->next;
            count_bad++;
        } else {
            count_bad = 0;
        }
    }
    clear(head);
    return result;
}

int main() {
    int n;
    cin >> n;
    point* head = new point;

    cin >> head->x >> head->y;
    head->n = 0;
    point* prev = head;

    point* min = head;

    for (int i = 1; i < n; i++) {
        point* cur = new point;
        cin >> cur->x >> cur->y;
        cur->n = i;
        prev->setNext(cur);
        prev = cur;

        if (cur->y < min->y) {
            min = cur;
        }
    }
    prev->setNext(head);

    for (int i = 0; i < n; i++) {
        if (head->y == min->y && head->x > min->x) {
            min = head;
        }
        head = head->next;
    }

    int clockwise = orientation(*min->prev, *min->next, *min);

    vector<pair<int, int>> res;

    res = getTriangulation(head, n, clockwise);
    assert(res.empty() || res[0].first != -1);
    for (int i = 0; i < n - 3; i++) {
        cout << res[i].first << " " << res[i].second << endl;
    }

    return 0;
}