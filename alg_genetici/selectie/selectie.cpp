#include <iostream>

using namespace std;

int main() {
    int a, b, c, n;
    cin >> a >> b >> c;
    cin >> n;
    double crom[n], x, F = 0, f;

    for (int i = 0; i < n; i ++)
    {
        cin >> x;
        f = a * x * x + b * x + c;
        F += f;
        crom[i] = f;
    }
    f = 0;
    cout << f / F << endl;
    for (int i = 0; i < n; i++)
    {
        f += crom[i];
        cout << f / F << endl;
    }
    return 0;
}
