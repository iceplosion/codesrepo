#include <bits/stdc++.h>
#define lo(i, n, m) for (int i = n; i < m; i++)
#define loe(i, n, m) for (int i = n; i <= m; i++)
using namespace std;
const int LIM = 1e5;
int nxt[LIM], f[LIM], ed[LIM], cnt[LIM], que[LIM], hd, tl, accur, ans[LIM];
char str[LIM], strid = 0;

typedef pair<int, int> P;
map<P, int> mp;
int idcur = 0;
inline int getId(int crt, int sig) {
    if (!mp.count(make_pair(crt, sig))) {
        mp[make_pair(crt, sig)] = ++idcur;
    }
    return mp[make_pair(crt, sig)];
}

inline void insAC(char *str) {
    int cur = 0;
    for (int i = 0; str[i]; i++) {
        int idx = getId(cur, str[i]);
        if (!nxt[idx]) nxt[idx] = ++accur;
        cur = nxt[idx];
    }
    ed[cur] = ++strid;
}

inline void build() {
    hd = tl = 0;
    for (auto x : mp) {
        if (x.first.first) break;
        que[tl++] = nxt[x.second];
    }
    while (hd < tl) {
        int u = que[hd++];
        auto ite = mp.lower_bound(make_pair(u, 0));
        while (ite != mp.end()) {
            if (ite->first.first != u) break;
            for (int tp = f[u]; true; tp = f[tp]) {
                if (!tp || mp.count(make_pair(tp, ite->first.second))) {
                    f[ite->second] = nxt[getId(tp, ite->first.second)];
                    break;
                }
            }
            que[tl++] = ite->second;
            ite++;
        }
    }
}

int main(void) {
    int n;
    scanf("%d", &n);
    lo(i, 0, n) {
        scanf("%s", str);
        insAC(str);
    }
    build();
    scanf("%s", str);
    int cur = 0;
    for (int i = 0; str[i]; i++) {
        ++cnt[cur];
        int tp;
        for (tp = cur; true; tp = f[tp]) {
            if (!tp || mp.count(make_pair(tp, str[i]))) {
                tp = nxt[getId(tp, str[i])];
                break;
            }
        }
        cur = tp;
    }
    ++cnt[cur];
    for (int i = tl - 1; i >= 0; i--) cnt[f[que[i]]] += cnt[que[i]];
    for (int i = 1; i <= accur; i++) if (ed[i]) ans[ed[i]] += cnt[i];
    for (int i = 1; i <= n; i++) printf("%d\n", ans[i]);
    return 0;
}

/*
5
abc
deabced
eed
abced
deab
abcccdeedeeeddeabced
*/
