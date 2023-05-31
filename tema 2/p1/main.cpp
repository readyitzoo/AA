#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    int x, y;

    Point(int x = 0, int y = 0): x(x), y(y) {}
};

struct Check_point: Point {
    int index;

    Check_point(int x = 0, int y = 0, int index = 0): Point(x, y), index(index) {}
};

enum class Point_position {
    Inside,
    Outside,
    Boundary
};

inline int sign(long long x) {
    if(x < 0)
        return -1;
    else if(x == 0)
        return 0;
    return 1;
}

// Returns the orientation of r with respect to segment pq: -1 -> right, 0 -> touch, 1 -> left
int orientation(Point p, Point q, Point r) {
    long long determinant =
            (1LL * q.x * r.y - 1LL * r.x * q.y) -
            (1LL * p.x * r.y - 1LL * r.x * p.y) +
            (1LL * p.x * q.y - 1LL * q.x * p.y);

    return sign(determinant);
}

int n, m;
vector<Point> polygon;
vector<Point> lower_points, upper_points; // partition of the polygon
vector<Check_point> check_points;
vector<Point_position> answers; // contains answer for each check point

void read() {
    cin >> n;
    polygon.resize(n);
    for(int i = 0; i < n; i++)
        cin >> polygon[i].x >> polygon[i].y;

    cin >> m;
    check_points.resize(m);
    for(int i = 0; i < m; i++) {
        cin >> check_points[i].x >> check_points[i].y;
        check_points[i].index = i;
    }
}

void partition_polygon() {
    // compute leftmost and rightmost points indices
    int leftmost_index = -1, rightmost_index = -1;

    for(int i = 0; i < n; i++) {
        if(leftmost_index == -1 || polygon[i].x < polygon[leftmost_index].x)
            leftmost_index = i;
        if(rightmost_index == -1 || polygon[i].x > polygon[rightmost_index].x)
            rightmost_index = i;
    }

    // partition polygon into upper and lower points
    int i;

    i = leftmost_index;
    do {
        lower_points.push_back(polygon[i]);
        i = (i + 1) % n;
    } while(i != (rightmost_index + 1) % n);

    i = leftmost_index;
    do {
        upper_points.push_back(polygon[i]);
        i = (i - 1 + n) % n;
    } while(i != (rightmost_index - 1 + n) % n);
}

bool cmp_check_points(const Check_point &p1, const Check_point &p2) {
    return p1.x < p2.x;
}

void solve() {
    partition_polygon();
    sort(check_points.begin(), check_points.end(), cmp_check_points);

    int lower_sz = lower_points.size(), upper_sz = upper_points.size();
    int min_x = lower_points[0].x, max_x = lower_points.back().x;
    int lower_index = 0, upper_index = 0; // indices for the rightmost points at the left of the current check point
    int check_index = 0;
    answers.resize(m);

    // answer points to the left of the polygon
    while(check_index < m && check_points[check_index].x < lower_points[0].x) {
        answers[check_points[check_index].index] = Point_position::Outside;
        check_index++;
    }

    // answer points with the same x as min_x
    int min_y = lower_points[0].y, max_y = lower_points[0].y;

    while(lower_index < lower_sz && lower_points[lower_index].x == min_x) {
        min_y = min(min_y, lower_points[lower_index].y);
        lower_index++;
    }
    while(upper_index < upper_sz && upper_points[upper_index].x == min_x) {
        max_y = max(max_y, upper_points[upper_index].y);
        upper_index++;
    }

    while(check_index < m && check_points[check_index].x == min_x) {
        if(check_points[check_index].y >= min_y && check_points[check_index].y <= max_y)
            answers[check_points[check_index].index] = Point_position::Boundary;
        else
            answers[check_points[check_index].index] = Point_position::Outside;
        check_index++;
    }

    // answer points between min_x and max_x
    while(check_index < m && check_points[check_index].x < max_x) {
        // update lower and upper indices
        while(lower_index < lower_sz && lower_points[lower_index].x <= check_points[check_index].x)
            lower_index++;
        lower_index--;

        while(upper_index < upper_sz && upper_points[upper_index].x <= check_points[check_index].x)
            upper_index++;
        upper_index--;

        // compute orientation of check point relative to lower and upper edge
        int lower_o = orientation(lower_points[lower_index], lower_points[lower_index + 1], check_points[check_index]);
        int upper_o = orientation(upper_points[upper_index], upper_points[upper_index + 1], check_points[check_index]);
        if(lower_o == -1 || upper_o == 1)
            answers[check_points[check_index].index] = Point_position::Outside;
        else if(lower_o == 0 || upper_o == 0)
            answers[check_points[check_index].index] = Point_position::Boundary;
        else
            answers[check_points[check_index].index] = Point_position::Inside;

        check_index++;
    }

    // answer points with the same x as max_x
    min_y = lower_points.back().y, max_y = lower_points.back().y;
    lower_index = lower_sz - 1, upper_index = upper_sz - 1;

    while(lower_index >= 0 && lower_points[lower_index].x == max_x) {
        min_y = min(min_y, lower_points[lower_index].y);
        lower_index--;
    }
    while(upper_index >= 0 && upper_points[upper_index].x == max_x) {
        max_y = max(max_y, upper_points[upper_index].y);
        upper_index--;
    }

    while(check_index < m && check_points[check_index].x == max_x) {
        if(check_points[check_index].y >= min_y && check_points[check_index].y <= max_y)
            answers[check_points[check_index].index] = Point_position::Boundary;
        else
            answers[check_points[check_index].index] = Point_position::Outside;
        check_index++;
    }

    // answer points to the right of the polygon
    while(check_index < m) {
        answers[check_points[check_index].index] = Point_position::Outside;
        check_index++;
    }
}

void print() {
    for(int i = 0; i < m; i++)
        switch (answers[i]) {
            case Point_position::Outside:
                cout << "OUTSIDE\n";
                break;
            case Point_position::Inside:
                cout << "INSIDE\n";
                break;
            case Point_position::Boundary:
                cout << "BOUNDARY\n";
                break;
            default:
                break;
        }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    print();


    return 0;
}
