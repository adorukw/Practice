#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
    double x, y;
};

double DistSq(const Point& a, const Point& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

double Dist(const Point& a, const Point& b) { return sqrt(DistSq(a, b)); }

bool CmpX(const Point& a, const Point& b) { return a.x < b.x; }

bool CmpY(const Point& a, const Point& b) { return a.y < b.y; }

const double INF = 1e18;
const double EPS = 1e-9;

double ClosestPair(vector<Point>& points, int left, int right) {
    if (left == right) return INF;
    if (right - left == 1) return Dist(points[left], points[right]);

    int mid = (left + right) / 2;
    double leftMin = ClosestPair(points, left, mid);
    double rightMin = ClosestPair(points, mid + 1, right);
    double minDist = min(leftMin, rightMin);

    vector<Point> candidates;
    for (int i = left; i <= right; i++) {
        if (fabs(points[i].x - points[mid].x) < minDist)
            candidates.push_back(points[i]);
    }
    sort(candidates.begin(), candidates.end(), CmpY);

    int n = candidates.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1;
             j < n && (candidates[j].y - candidates[i].y) < minDist; j++) {
            double currDist = Dist(candidates[i], candidates[j]);
            if (currDist < minDist) minDist = currDist;
        }
    }

    return minDist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    while (cin >> N) {
        if (N == 0) break;
        vector<Point> points(N);
        for (int i = 0; i < N; i++) cin >> points[i].x >> points[i].y;
        sort(points.begin(), points.end(), CmpX);
        double mindDist = ClosestPair(points, 0, N - 1);
        cout << fixed << setprecision(2) << mindDist / 2 << '\n';
    }
}