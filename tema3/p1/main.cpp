// l3p1
#include <iostream>
#include <vector>

using namespace std;

struct Point
{
    long long x, y;
};

vector <Point> puncte ;
Point A, B ,C;

long long determinant (Point  D) {
    long long x1=0, x2=0, y1=0, y2=0, z1=0, z2=0, z3=0, y3=0, x3=0, s1=0, s2=0;
    // calculam determinantul punctelor A,B,C si punctul D
    x1=B.x-A.x;
    x2=B.y-A.y;
    y1=C.x-A.x;
    y2=C.y-A.y;
    z1=D.x-A.x;
    z2=D.y-A.y;
    x3=B.x*B.x-A.x*A.x+B.y*B.y-A.y*A.y;
    y3=C.x*C.x-A.x*A.x+C.y*C.y-A.y*A.y;
    z3=D.x*D.x-A.x*A.x+D.y*D.y-A.y*A.y;

    s2=z1*y2*x3+z2*y3*x1+x2*y1*z3;
    s1=x1*y2*z3+y1*z2*x3+x2*y3*z1;

    return (s2-s1);

};

int main ()
{
    long long m;
    cin >> A.x >> A.y >> B.x >> B.y >> C.x >> C.y;
    cin >> m;
    for (long long i=0;i<m;i++)
    {
        Point p;
        cin >> p.x >> p.y;
        puncte.push_back(p);
        long long rez = determinant(p);
        if (rez > 0)
            cout << "INSIDE" << endl;
        else {
            if (rez < 0)
                cout << "OUTSIDE" << endl;
            else
                cout << "BOUNDARY" << endl;
        }
    }
    return 0;
}