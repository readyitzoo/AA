// l3p2
#include <iostream>

using namespace std;

struct Point {
    long long x, y;
};

Point pA, pB, pC, pD;

long long determinant(Point& A, Point& B, Point& C, Point& D) {
    long long x1 = B.x - A.x;
    long long x2 = B.y - A.y;
    long long y1 = C.x - A.x;
    long long y2 = C.y - A.y;
    long long z1 = D.x - A.x;
    long long z2 = D.y - A.y;
    long long x3 = B.x * B.x - A.x * A.x + B.y * B.y - A.y * A.y;
    long long y3 = C.x * C.x - A.x * A.x + C.y * C.y - A.y * A.y;
    long long z3 = D.x * D.x - A.x * A.x + D.y * D.y - A.y * A.y;

    long long sum2 = z1 * y2 * x3 + z2 * y3 * x1 + x2 * y1 * z3;
    long long sum1 = x1 * y2 * z3 + y1 * z2 * x3 + x2 * y3 * z1;

    return sum2 - sum1;
};

int main ()
{
    cin >> pA.x >> pA.y >> pB.x >> pB.y >> pC.x >> pC.y >> pD.x >> pD.y;

    // daca D este in interiorul cercului circumscris triunghiului ABC => AC e ilegala
    if (determinant(pA, pB, pC, pD) > 0) {
        cout << "AC: ILLEGAL" << '\n';
    } else {
        cout << "AC: LEGAL" << '\n';
    }

    // daca A este in exteriorul cercului circ triunghiului BCD => BD e legala
    if (determinant(pB, pC, pD, pA) > 0) {
        cout << "BD: ILLEGAL" << '\n';
    } else {
        cout << "BD: LEGAL" << '\n';
    }
}