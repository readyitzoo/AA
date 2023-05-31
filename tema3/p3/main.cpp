// l3p3
#include <iostream>

using namespace std;

const int xMin = -10000000;
const int xMax = 10000000;

int n;
bool lessThan;
double limit;
int a, b, c;
double xLeft, xRight, yDown, yUp;

int main() {
    cin >> n;

    xLeft = yDown = xMin ;
    xRight = yUp = xMax ;
    for (int i = 1; i <= n; ++i) {
        lessThan = true;
        cin >> a >> b >> c;

        if (a == 0) {
            if (b < 0) {
                lessThan = false;
            }

            limit = (double)(-c) / b;

            if (lessThan) {
                yUp = min(yUp, limit);
            } else {
                yDown = max(yDown, limit);
            }
        } else { // ax + c <= 0
            if (a < 0) {
                lessThan = false;
            }

            limit = (double)(-c) / a;

            if (lessThan) {
                xRight = min(xRight, limit);
            } else {
                xLeft = max(xLeft, limit);
            }
        }
    }

    if (xLeft > xRight || yDown > yUp) {
        cout << "VOID";
    } else if (xLeft == xMin  || yDown == xMin  || xRight == xMax  || yUp == xMax ) {
        cout << "UNBOUNDED";
    } else {
        cout << "BOUNDED";
    }
}