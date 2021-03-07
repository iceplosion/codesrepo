#include <iostream>

using namespace std;

const int LIM;
int Z[LIM];
char str[LIM];
void buildZ(int n) {
    Z[0] = n;
    int j = 1, k;
    for (int i = 1; i < n; i = k) {
        if (j < i) j = i;
        while (j < n && str[j - i] == str[j]) j++;
        Z[i] = j - i, k = i + 1;
        while (Z[k - i] + k < j) {
            Z[k] = Z[k - 1];
            k++;
        }
    }
}
int main(void) {

    return 0;
}
