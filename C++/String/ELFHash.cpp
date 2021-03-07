#include <iostream>

using namespace std;

inline unsigned int ELFHash(char *str) {
    unsigned int ret = 0, x = 0;
    for (; *str; str++) {
        ret = (ret << 4) + *str;
        if (x = ret & 0xf0000000) {
            ret ^= (x >> 24);
            ret &= ~x;
        }
    }
    return ret & 0x7fffffff;
}

int main(void) {

    return 0;
}
