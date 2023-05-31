// l3p4
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

int n, m;
bool lessThan;
double limit, x, y;
int a, b, c;

vector < long double > leftLimits;
vector < long double > rightLimits;
vector < long double > downLimits;
vector < long double > upLimits;

// the biggest l such that l < element
int getSmallerBiggestLimit(vector < long double >& smallerLimits, double element) {
    if (smallerLimits.empty() || element < smallerLimits[0]) {
        return -1;
    }

    int left = 0, right = smallerLimits.size() - 1, last = -1;
    while (right >= left) {
        int mid = (left + right) / 2;
        if (element > smallerLimits[mid]) {
            last = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return last;
}

// the smallest l such that l > element
int findBiggerSmallestLimit(vector < long double >& greaterLimits, double element) {
    if (greaterLimits.empty() || element > greaterLimits.back()) {
        return -1;
    }

    int left = 0, right = greaterLimits.size() - 1, last = -1;
    while (right >= left) {
        int mid = (left + right) / 2;
        if (element < greaterLimits[mid]) {
            last = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return last;
}

int main() {
    cin >> n;

    for (int i = 1; i <= n; ++i) {
        lessThan = true;
        cin >> a >> b >> c;
        // by + c <= 0
        if (a == 0) {
            if (b < 0) {
                lessThan = false;
            }

            limit = (double)(-c) / b;

            if (lessThan) {
                upLimits.push_back(limit);
            } else {
                downLimits.push_back(limit);
            }
        } else {  // ax + c <= 0
            if (a < 0) {
                lessThan = false;
            }

            limit = (double)(-c) / a;

            if (lessThan) {
                rightLimits.push_back(limit);
            } else {
                leftLimits.push_back(limit);
            }
        }
    }

    cin >> m;

    sort(leftLimits.begin(), leftLimits.end());
    sort(rightLimits.begin(), rightLimits.end());
    sort(downLimits.begin(), downLimits.end());
    sort(upLimits.begin(), upLimits.end());

    for (int i = 1; i <= m; ++i) {
        cin >> x >> y;

        int leftIndex = getSmallerBiggestLimit(leftLimits, x);
        int rightIndex = findBiggerSmallestLimit(rightLimits, x);
        int downIndex = getSmallerBiggestLimit(downLimits, y);
        int upIndex = findBiggerSmallestLimit(upLimits, y);

        if (leftIndex == -1 || rightIndex == -1 || downIndex == -1 || upIndex == -1) {
            cout << "NO" << '\n';
        } else {
            long double area = (rightLimits[rightIndex] - leftLimits[leftIndex]) * (upLimits[upIndex] - downLimits[downIndex]);
            cout << "YES" << '\n';
            cout << setprecision(6) << fixed << area << '\n';
        }
    }
}