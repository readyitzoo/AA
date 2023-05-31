#include <iostream>

using namespace std;

int main() {
    int l, k, m;
    string crom;
    cin >> l >> k >> crom;
    for (int i = 0; i < k; i++)
    {
        cin >> m;
        if (crom[m] == '0')
            crom[m] = '1';
        else
            crom[m] = '0';
    }
    cout << crom;
    return 0;
}
