#include <bits/stdc++.h>
#define lo(i, n, m) for (int i = n; i < m; ++i)
#define loe(i, n, m) for (int i = n; i <= m; ++i)
#define rlo(i, n, m) for (int i = n - 1; i >= m; --i)
#define rloe(i, n, m) for (int i = n; i >= m; --i)
using namespace std;
const int lim = 1e5 + 5;
const int sz = lim * 128;
char _buf[sz], *bcur = _buf;
#define _ac(des, tp, sz) tp *des = (tp*)bcur; bcur += sizeof(tp)*(sz + 5)
#define LT 0
#define ST 1

int lbd[lim], rbd[lim];
inline void is(int len, int sigma, int *s, int *sa, bool *tp, int *cnt) {
    loe(i, 1, sigma) lbd[i] = cnt[i - 1], rbd[i] = cnt[i] - 1;
    lo(i, 0, len) if (sa[i] > 0 && !tp[sa[i] - 1]) sa[lbd[s[sa[i] - 1]]++] = sa[i] - 1;
    rlo(i, len, 0) if (sa[i] > 0 && tp[sa[i] - 1]) sa[rbd[s[sa[i] - 1]]--] = sa[i] - 1;
}
inline bool cmp(int *s1, int *s2, int len) {
    while (len--) if (*(s1++) != *(s2++)) return false;
    return true;
}
void sais(int len, int sigma, int *s, int *sa) {
    // 四个数组，len长度的tp，ceil(len / 2)长度的ps，sigma长度的bd和cnt
    _ac(tp, bool, len);
    _ac(cnt, int, sigma);
    _ac(ps, int, len);
    _ac(bd, int, sigma);
    // 添加虚拟字符'\0'
    tp[len] = ST;
    s[len++] = '\0';
    // 染色L、S，桶大小计数、初始化sa
    rlo(i, len, 1) {
        if (s[i - 1] == s[i]) tp[i - 1] = tp[i];
        else tp[i - 1] = s[i - 1] > s[i] ? LT : ST;
        ++cnt[s[i]];
        sa[i] = -1;
    }
    ++cnt[s[0]];
    sa[0] = -1;
    // 设置桶边界
    loe(i, 1, sigma) {
        cnt[i] += cnt[i - 1];
        bd[i] = cnt[i] - 1;
    }
    // 存储lms位置，并放入桶
    int cc = 0;
    lo(i, 1, len) if (!tp[i - 1] && tp[i]) sa[bd[s[ps[cc++] = i]]--] = i;
    // 诱导排序
    is(len, sigma, s, sa, tp, cnt);
    // 开启lms长度（len）、lms编码（len）、新sa（lms数量）、新子串数组(lms数量)
    _ac(slen, int, len);
    _ac(sa1, int, cc);
    _ac(s1, int, cc);
    _ac(code, int, len);
    // 设置lms长度
    rlo(i, cc - 1, 0) slen[ps[i]] = ps[i + 1] - ps[i] + 1;
    slen[ps[cc - 1]] = 1;
    // 初始化编码数组
    int idx = 0, cur = -1, pre = -1, ssz = 0;
    lo(i, 0, len) code[i] = -1;
    // 设置编码
    lo(i, 0, len) if ((cur = sa[i]) > 0 && !tp[sa[i] - 1] && tp[sa[i]]) {
        if (pre != -1 && slen[cur] == slen[pre] && cmp(s + pre, s + cur, slen[cur])) code[cur] = idx;
        else code[cur] = ++idx;
        pre = cur;
    }
    lo(i, 0, len) if (~code[i]) s1[ssz++] = code[i];
    // 判断编码数与lms数的关系，决定是否能直接解决子问题
    if (idx == cc) lo(i, 0, cc) sa1[s1[i] - 1] = i;
    else sais(cc, idx, s1, sa1);
    bd[0] = 0;
    loe(i, 1, sigma) bd[i] = cnt[i] - 1;
    lo(i, 0, len) sa[i] = -1;
    rlo(i, cc, 0) sa[bd[s[ps[sa1[i]]]]--] = ps[sa1[i]];
    is(len, sigma, s, sa, tp, cnt);
    --len;
    lo(i, 0, len) sa[i] = sa[i + 1];
}
char _ob[lim * 16], *ocur = _ob;
inline void _pt(int x) {
    if (x) {
        static char bb[16];
        int bbcur = 0;
        while (x) bb[bbcur++] = x % 10, x /= 10;
        while (bbcur--) *(ocur++) = bb[bbcur] + '0';
    } else {
        *(ocur++) = '0';
    }
}
inline void _pt(char x) {
    *(ocur++) = x;
}
inline void _fl() {
    fwrite(_ob, 1, ocur - _ob, stdout);
}
char str[lim];
int sa[lim];
int main(void) {
    int n = 0;
    fread(str, 1, lim, stdin);
    _ac(s, int, lim);
    for (int i = 0; str[i] >= 'a' && str[i] <= 'z'; ++i) s[n++] = str[i];
    sais(n, 300, s, sa);
    _ac(rk, int, n);
    _ac(ht, int, n);
    lo(i, 0, n) {
        if (i) _pt(' ');
        _pt(sa[i] + 1);
        rk[sa[i]] = i;
    }
    _pt('\n');
    for (int i = 0, k = 0; i < n; i++) {
        if (k) --k;
        if (rk[i] > 0) while (str[i + k] == str[sa[rk[i] - 1] + k]) ++k;
        else k = 0;
        ht[rk[i]] = k;
    }
    lo(i, 1, n) {
        if (i > 1) _pt(' ');
        _pt(ht[i]);
    }
    _fl();
    return 0;
}
