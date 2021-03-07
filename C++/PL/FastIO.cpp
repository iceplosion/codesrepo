#include <iostream>

using namespace std;

struct FastIO {
    bool valid = true;
    inline bool isDigit(char ch) {
        return ch >= '0' && ch <= '9';
    }
    FastIO & operator >> (int & num) {
        char ch;
        num = 0;
        while (~(ch = getchar()) && !isDigit(ch));
        if (ch == EOF) {
            valid = false;
            return *this;
        }
        do {
            num = num * 10 + (ch & 15);
        } while (ch = getchar(), isDigit(ch));
        if (ch == EOF) valid = false;
        return *this;
    }
    operator bool() {
        return valid;
    }
} in;

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    while (in >> n)
        cout << n << endl;
    return 0;
}
