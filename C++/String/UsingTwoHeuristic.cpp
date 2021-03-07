#include <iostream>
#include <string>
#include <cstring>

using namespace std;

string txt, pat;
const int LIM = 100;
const int SIZE = 256;
int lastAppear[SIZE];
int shift[LIM];
int border[LIM];

inline int maxi(int a, int b) {return a > b ? a : b;}
inline int mini(int a, int b) {return a < b ? a : b;}

void badHeuristic(int pl) {
    memset(lastAppear, 0, sizeof lastAppear);
    for (int i = 0; i < pl; i++) {
        lastAppear[pat[i]] = i;
    }
}

void buildBorder(int pl) {
    int j = pl + 1, i = pl;
    border[i] = j;
    while (i > 0) {
        while (j <= pl && pat[i - 1] != pat[j - 1]) {
            if (!shift[j]) {
                shift[j] = j - i;
            }
            j = border[j];
        }
        i--, j--;
        border[i] = j;
    }
}

void goodHeuristic(int pl) {
    int j = border[0];
    for (int i = 0; i <= pl; i++) {
        if (!shift[i]) shift[i] = j;
        if (i == j) j = border[j];
    }
}

void BoyerMoore() {
    int pl = pat.size(), tl = txt.size();
    badHeuristic(pl);
    buildBorder(pl);
    goodHeuristic(pl);
    int rear = pl - 1;
    while (rear < tl) {
        int i = pl - 1;
        while (i >= 0 && pat[i] == txt[rear - pl + i + 1]) i--;
        if (i < 0) {
            /*
            * Find the pattern
            * cout << "find pattern at " << rear - pl + 1<< endl;
            */
            if (rear + 1 >= tl) rear += shift[0];
            else rear += maxi(shift[0], pl - lastAppear[txt[rear + 1]]);
        } else {
            rear += maxi(shift[i + 1], maxi(1, i - lastAppear[txt[rear - pl + i + 1]]));
        }
    }
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> txt >> pat;
    BoyerMoore();
    return 0;
}
