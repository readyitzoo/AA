#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

string decimalToBinary(int n, int numBits) {
    string binary;
    for (int i = numBits - 1; i >= 0; i--) {
        int bit = (n >> i) & 1;
        binary += to_string(bit);
    }
    return binary;
}

int binaryToDecimal(const string& binary) {
    int decimal = 0;
    int numBits = binary.size();
    for (int i = 0; i < numBits; i++) {
        int bit = binary[i] - '0';
        decimal = decimal * 2 + bit;
    }
    return decimal;
}

int main() {
    double a, b, d, real_crom;
    int p, m, l;
    string operation, binary_crom;

    cin >> a >> b;
    cin >> p;
    cin >> m;

    l = ceil(log2((b - a) * pow(10, p))); // number of bits needed
    d = (b - a) / pow(2, l); // discretization step

    for (int i = 0; i < m; i++) {
        cin >> operation;
        if (operation == "TO") {
            double a1 = a;
            cin >> real_crom;
            int j = static_cast<int>((real_crom - a1) / d);
            cout << decimalToBinary(j, l) << endl;
        } else {
            cin >> binary_crom;
            double j = binaryToDecimal(binary_crom);
            j = a + j * d;
            cout << j << endl;
        }
    }

    return 0;
}
