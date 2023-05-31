#include <iostream>
#include <vector>

using namespace std;

// define a struct Point to represent a point on the Cartesian plane with x and y coordinates
struct Point {
    int x, y;

    Point(int x = 0, int y = 0): x(x), y(y) {}
};

// define an enum to represent the possible positions of a point relative to a polygon
enum class Point_position {
    Inside,
    Outside,
    Boundary
};

// define an inline function to return the sign of a long long integer
inline int sign(long long x) {
    if(x < 0)
        return -1;
    else if(x == 0)
        return 0;
    return 1;
}

int orientation(Point p, Point q, Point r) {
    long long determinant =
            (1LL * q.x * r.y - 1LL * r.x * q.y) -
            (1LL * p.x * r.y - 1LL * r.x * p.y) +
            (1LL * p.x * q.y - 1LL * q.x * p.y);

    // return the sign of the determinant
    return sign(determinant);
}

int n, m;
vector<Point> polygon;

// define a function to read in the polygon from standard input
void read() {
    cin >> n;
    polygon.resize(n);
    for(int i = 0; i < n; i++)
        cin >> polygon[i].x >> polygon[i].y;
}

Point_position get_point_position(Point q) {
    int left_edges = 0, right_edges = 0;   // count the number of edges to the left and right of the point

    // loop over each edge of the polygon
    for(int i = 0; i < n; i++) {
        Point p1 = polygon[i], p2 = polygon[(i + 1) % n];  // get the endpoints of the edge
        if(p1.y > p2.y || (p1.y == p2.y && p1.x > p2.x))    // ensure that p1 is below p2 (or to the left if they are on the same horizontal line)
            swap(p1, p2);

        if(q.y < p1.y || q.y > p2.y)    // if q is above or below the edge, skip to the next edge
            continue;

        if(p1.x == p2.x) { // if the edge is vertical
            if(q.y == p1.y)   // if q lies on the edge, it is on the boundary of the polygon
                continue;
            if(q.x < p1.x)
                right_edges++;
            else if(q.x > p1.x)
                left_edges++;
            else
                return Point_position::Boundary;
        }
        else if(p1.y == p2.y) { // horizontal edge
            if(q.x >= p1.x && q.x <= p2.x)
                return Point_position::Boundary;
            else
                continue;
        }
        else {
            int o = orientation(p1, p2, q);
            if(o == 0)
                return Point_position::Boundary;
            if(q.y == p1.y)
                continue;
            if(o == -1)
                left_edges++;
            else if(o == 1)
                right_edges++;
        }
    }

    if(left_edges % 2 == 1 && right_edges % 2 == 1)
        return Point_position::Inside;
    return Point_position::Outside;
}

void solve() {
    Point q;

    cin >> m;
    for(int i = 0; i < m; i++) {
        cin >> q.x >> q.y;
        Point_position position = get_point_position(q);
        switch (position) {
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
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();

    return 0;
}
