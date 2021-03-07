#include <bits/stdc++.h>

using namespace std;

const int LIM = 100;  /*size limit of string*/
const int CHAR_SIZE = 300;      /*The maximum value of characters*/
char str[LIM] = "banana";  /*string*/
int sa[LIM], _t[2][LIM * 2], _cnt[CHAR_SIZE];
inline void SAbuild(int n) {
    int i, *a = _t[0], *b = _t[1], *c = _cnt, m = CHAR_SIZE, cur;
    // 初次桶排序
    for (i = 0; i < m; i++) c[i] = 0;   // 初始化计数桶
    for (i = 0; i < n; i++) ++c[a[i] = str[i]];  // 计数
    for (i = 1; i < m; i++) c[i] += c[i - 1];   // 前缀和
    for (i = n - 1; i >= 0; i--) sa[--c[a[i]]] = i; // 存放按照首个字符排好序的后缀索引
    // 开始倍增
    for (int k = 1; k <= n; k <<= 1) {
        cur = 0;
        for (i = n - k; i < n; i++) b[cur++] = i;
        for (i = 0; i < n; i++) if (sa[i] >= k) b[cur++] = sa[i] - k;
        for (i = 0; i < m; i++) c[i] = 0;
        for (i = 0; i < n; i++) c[a[b[i]]]++;
        for (i = 1; i < m; i++) c[i] += c[i - 1];
        for (i = n - 1; i >= 0; i--) sa[--c[a[b[i]]]] = b[i];
        cur = b[sa[0]] = 0;
        // 重新编号
        for (i = 1; i < n; i++)
            b[sa[i]] = a[sa[i - 1]] == a[sa[i]] && a[sa[i - 1] + k] == a[sa[i] + k] ? cur : ++cur;
        cout << cur << endl;
        if (cur == n - 1) break;
        m = cur + 1;  // 缩小字符集的大小
        swap(a, b);
    }
}

int main(void) {
    // 此处使用字符串banana进行测试，排好序后应当得到：
    // 5 3 1 0 4 2
    // 对应后缀：
    // 5 a
    // 3 ana
    // 1 anana
    // 0 banana
    // 4 na
    // 2 nana
    int len;
    SAbuild(len = strlen(str));
    for (int i = 0; i < len; i++) {
        if (i) putchar(' ');
        printf("%d", sa[i]);
    }
    return 0;
}
