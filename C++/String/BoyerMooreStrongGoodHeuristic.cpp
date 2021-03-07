#include <iostream>
#include <string>
#include <cstring>

using namespace std;

const int LIM = 1e4 + 10;    /* The maximum size of pattern string */
string txt, pat;
// The position of border
int border[LIM];
int shift[LIM];

void strongGoodHeuristic(int pl) {
    int i = pl, j = pl + 1;
    // set the value gaurd (to avoid the endless loop)
    border[i] = j;
    while (i > 0) {
        while (j <= pl && pat[i - 1] != pat[j - 1]) {
            if (shift[j] == 0) {
                shift[j] = j - i;
            }
            j = border[j];
        }
        i--, j--;
        border[i] = j;
    }
}

void preprocessing(int pl) {
    int j = border[0];
    for (int i = 0; i <= pl; i++) {
        if (!shift[i]) shift[i] = j;
        if (i == j) j = border[j];
    }
}

// 假定模式串比文本串短
void BoyerMoore() {
    int pl = pat.size();
    int tl = txt.size();
    memset(shift, 0, sizeof shift);
    strongGoodHeuristic(pl);
    preprocessing(pl);
    int offset = 0;
    int i;
    while (offset <= tl - pl) {
        i = tl - 1;
        while (i >= 0 && pat[i] == txt[i + offset])
            i--;
        if (i < 0) {
            /* Find the pattern */
            /* Do something here */
            offset += shift[0];
        } else {
            offset += shift[i + 1];
        }
    }
}

int main(void) {

    return 0;
}
