#include <iostream>

using namespace std;

int main() {
    int l, i;
    string crom1, crom2;
    cin >> l;
    cin >> crom1 >> crom2;
    cin >> i;
    for (int j = i; j < l ; j++)
    {
        char aux = crom1[j];
        crom1[j] = crom2[j];
        crom2[j] = aux;
    }
    cout << crom1 << endl << crom2;
    return 0;
}
