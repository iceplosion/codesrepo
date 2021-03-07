#include <bits/stdc++.h>
#define pb push_back
#define mk make_pair
#define lo(i, n, m) for (int i = n; i < m; ++i)
#define loe(i, n, m) for (int i = n; i <= m; ++i)
#define rlo(i, n, m) for (int i = n - 1; i >= m; --i)
#define rloe(i, n, m) for (int i = n; i >= m; --i)
using namespace std;
const int buflim = 20000;
char buf[buflim], *bcur = buf;
// 0: key, 1: identifier, 2: literal, 3: operator
map<string, int> ides, keys, opes;
int idid = 5;
int pri[40] = {0, 114, 114, 114, 114, 2, 2, 4, 2, 4,
    3, 3, 3, 5, 5, 5, 5, 6, 6, 7, 8, 9, 10, 11, 11};
int rgt[40] = {0, 0, 0, 0, 0, 1, 1, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
struct Tk {
    int tp, val;
    Tk() {}
    Tk(int tt, int vv) : tp(tt), val(vv) {}
} tks[10000], *tkcur = tks;
vector<int> ins;
int icur = 0;
inline bool isbk() {
    return *bcur == ' ' || *bcur == '\n' || *bcur == '\t';
}
#define isdg() isdigit(*bcur)
#define isal() isalpha(*bcur)
int Ri() {
    int r = 0, f = 1;
    while (isbk()) ++bcur;
    if (*bcur == '-') f = -1, ++bcur;
    while (isdg()) r = (r << 3) + (r << 1) + *(bcur++) - '0';
    return r * f;
}
char wdb[20000], *wdcur = wdb;
void Rw() {
    while (isbk()) ++bcur;
    wdcur = wdb;
    while (isal() || isdg() || *bcur == '_') *wdcur++ = *bcur++;
    *wdcur = '\0';
}
bool eq(const Tk &a, const Tk &b) {
    return a.tp == b.tp && a.val == b.val;
}
void Itk(int tt, int vv) {
    *tkcur = Tk(tt, vv);
    if ((eq(*tkcur, Tk(3, 6)) || eq(*tkcur, Tk(3, 8)))
        && (tkcur[-1].tp != 3 || eq(tkcur[-1], Tk(3, 2)) || eq(tkcur[-1], Tk(3, 4))))
        ++(tkcur->val);
    ++tkcur;
}
void tknize() {
    int n = Ri();
    lo(i, 0, n) ins.pb(Ri());
    while (*bcur != ';') ++bcur;
    ++bcur;
    string wdm;
    while (*bcur != EOF) {
        if (*bcur == ' ' || *bcur == '\n' || *bcur == '\t') {
            ++bcur;
            continue;
        } else if (isal() || *bcur == '_') {
            Rw();
            wdm = string(wdb);
            if (keys.count(wdm)) Itk(0, keys[wdm]);
            else {
                if (!ides.count(wdm)) ides[wdm] = idid++;
                Itk(1, ides[wdm]);
            }
        } else if (isdg()) {
            int x = Ri();
            Itk(2, x);
        } else {
            wdb[0] = *bcur++;
            wdb[1] = *bcur;
            wdb[2] = 0;
            wdm = string(wdb);
            if (opes.count(wdm)) {
                ++bcur;
                Itk(3, opes[wdm]);
            } else {
                wdb[1] = 0;
                Itk(3, opes[string(wdb)]);
            }
        }
    }
}
struct Var {
    int id, *v;
    int sz;
    vector<int> dmsz;
    Var() : sz(1) {}
    Var(int ii) : id(ii), sz(1) {}
    void ad(int x) {
        dmsz.pb(x);
        sz *= x;
    }
};
typedef vector<Var> Vl;
struct Node {
    int tp, val;
    Vl ls;
    vector<Node> subs;
    Node &Cn() {
        subs.pb(Node());
        return subs.back();
    }
} func[10000];
Vl gbl;
Tk *vcur = tks;
void Pvls(Vl &vl) {
    static const Tk cma = Tk(3, 25);
    static const Tk it = Tk(0, 5);
    static const Tk bk = Tk(3, 3);
    while (eq(*vcur, it) || eq(*vcur, cma) || vcur->tp == 1) {
        if (vcur->tp == 1) {
            vl.pb(Var((vcur++)->val));
            while (eq(*vcur, bk)) {
                vl.back().ad(vcur[1].val);
                vcur += 3;
            }
        } else ++vcur;
    }
    ++vcur;
}
void Pfunc() {
    static int pos;
    func[pos = vcur[1].val].tp = 1;
    vcur += 3;
    Pvls(func[pos].ls);
    void Pblock(Node &);
    Pblock(func[pos].Cn());
}
void Pdec(Node &nod) {
    nod.tp = 2;
    nod.val = -1;
    Pvls(nod.ls);
}
void Pst(Node &nod) {
    if (vcur->tp == 0) {
        if (vcur->val == 1) {
            void Pif(Node &);
            Pif(nod.Cn());
        } else if (vcur->val == 3) {
            void Pwhile(Node &);
            Pwhile(nod.Cn());
        } else if (vcur->val == 4) {
            void Pfor(Node &);
            Pfor(nod.Cn());
        } else if (vcur->val == 5) {
            Pdec(nod.Cn());
            for (auto tmp : nod.subs.back().ls) nod.ls.pb(tmp);
        } else if (vcur->val == 6) {
            void Pret(Node &);
            Pret(nod.Cn());
        }
    } else if (eq(*vcur, Tk(3, 27))) {
        void Pblock(Node &);
        Pblock(nod.Cn());
    } else {
        void Pexp(Node &);
        Pexp(nod.Cn());
    }
}
void Pblock(Node &nod) {
    // if, while, for, int, {, expr, return
    nod.tp = 2;
    ++vcur;
    while (!eq(*vcur, Tk(3, 28))) Pst(nod);
    ++vcur;
}
void Plit(Node &nod) {
    nod.tp = 4;
    nod.val = vcur->val;
    ++vcur;
}
void Pide(Node &nod) {
    nod.tp = 5;
    nod.val = vcur->val;
    ++vcur;
}
void Pope(Node &nod, int val) {
    nod.tp = 6;
    nod.val = val;
}
void Pexp(Node &nod) {
    nod.tp = 3;
    stack<int> ops;
    ops.push(1);
    while (true) {
        if (vcur->tp == 1) {
            // variable, function
            if (eq(vcur[1], Tk(3, 1))) {void Pcall(Node &); Pcall(nod.Cn());}
            else Pide(nod.Cn());
        } else if (vcur->tp == 2) {
            Plit(nod.Cn());
        } else {
            if (vcur->val == 1 || vcur->val == 3) {
                ops.push(vcur->val);
            } else if (vcur->val == 2) {
                while (ops.top() != 1) {
                    Pope(nod.Cn(), ops.top());
                    ops.pop();
                }
                ops.pop();
                if (ops.empty()) break;
            } else if (vcur->val == 4) {
                while (ops.top() != 3) {
                    Pope(nod.Cn(), ops.top());
                    ops.pop();
                }
                Pope(nod.Cn(), 3);
                ops.pop();
            } else if (pri[vcur->val]) {
                while (pri[vcur->val] >= pri[ops.top()] + rgt[ops.top()]) {
                    Pope(nod.Cn(), ops.top());
                    ops.pop();
                }
                ops.push(vcur->val);
            } else {
                break;
            }
            ++vcur;
        }
    }
    while (ops.size() && ops.top() > 1) {
        Pope(nod.Cn(), ops.top());
        ops.pop();
    }
    ++vcur;
}
void Pif(Node &nod) {
    nod.tp = 7;
    vcur += 2;
    Pexp(nod.Cn());
    // {, if, while, for, int, expr, return
    Pst(nod);
    if (eq(*vcur, Tk(0, 2))) {
        ++vcur;
        // {, if, while, for, int, expr, return
        Pst(nod);
    }
}
void Pwhile(Node &nod) {
    nod.tp = 8;
    vcur += 2;
    Pexp(nod.Cn());
    Pst(nod);
}
void Pfor(Node &nod) {
    nod.tp = 9;
    vcur += 2;
    Pexp(nod.Cn());
    Pexp(nod.Cn());
    Pexp(nod.Cn());
    Pst(nod);
}
void Pret(Node &nod) {
    nod.tp = 10;
    ++vcur;
    Pexp(nod.Cn());
}
void Pcall(Node &nod) {
    nod.tp = 11;
    nod.val = vcur->val;
    vcur+= 2;
    if (!eq(vcur[0], Tk(3, 2))) {
        do {
            Pexp(nod.Cn());
        } while (eq(vcur[-1], Tk(3, 25)));
    } else ++vcur;
}
void parse() {
    while (vcur != tkcur) {
        if (eq(vcur[2], Tk(3, 1))) Pfunc();
        else Pvls(gbl);
    }
}
stack<Var> vs[10000];
int sp[30000000], *scur = sp;
#define alloc(a, b) a=scur;scur+=b;memset(a, 0, b*sizeof(int))
void freeup(Node &nod, int cc) {
    lo(i, 0, cc) {
        scur -= nod.ls[i].sz;
        vs[nod.ls[i].id].pop();
    }
}
void freeup(Node &nod) {freeup(nod, nod.ls.size());}
void allocall(Node &nod) {
    lo(i, 0, nod.ls.size()) {
        vs[nod.ls[i].id].push(nod.ls[i]);
        alloc(vs[nod.ls[i].id].top().v, nod.ls[i].sz);
    }
}
int (*exec[50])(Node &);
#define run(x) exec[x.tp](x)
vector<int> msg;
bool isret = false;
int Rfunc(Node &nod) {
    if (nod.val == -1) {
        int ret = putchar(msg.back());
        msg.pop_back();
        return ret;
    }
    allocall(nod);
    lo(i, 0, nod.ls.size()) {
        *vs[nod.ls[i].id].top().v = msg.back();
        msg.pop_back();
    }
    int ret = run(nod.subs[0]);
    if (!isret) ret = 0;
    freeup(nod);
    isret = false;
    return ret;
}
int Rblock(Node &nod) {
    if (nod.val == -1) {allocall(nod); return nod.ls.size();}
    int ret = 0, cc = 0;
    lo(i, 0, nod.subs.size()) {
        ret = run(nod.subs[i]);
        if (nod.subs[i].val == -1) cc += ret;
        if (isret) break;
    }
    freeup(nod, cc);
    return ret;
}
int Rexp(Node &nod) {
    if (!nod.subs.size()) return 1;
    lo(i, 0, nod.subs.size()) run(nod.subs[i]);
    int ret = scur[-3] ? vs[scur[-3]].top().v[scur[-2]] : scur[-2];
    scur -= 3;
    return ret;
}
int Rlit(Node &nod) {
    *scur++ = 0;
    *scur++ = nod.val;
    *scur++ = 0;
    return 0;
}
int Ride(Node &nod) {
    *scur++ = nod.val;
    *scur++ = 0;
    *scur++ = 0;
    return 0;
}
int pop() {
    int ret = scur[-3] ? vs[scur[-3]].top().v[scur[-2]] : scur[-2];
    scur -= 3;
    return ret;
}
void push(int x) {
    *scur++ = 0;
    *scur++ = x;
    *scur++ = 0;
}
int Rif(Node &nod) {
    int ret = 0;
    if (run(nod.subs[0])) ret = run(nod.subs[1]);
    else if (nod.subs.size() > 2) ret = run(nod.subs[2]);
    return ret;
}
int Rwhile(Node &nod) {
    int ret = 0;
    while (run(nod.subs[0])) {
        ret = run(nod.subs[1]);
        if (isret) break;
    }
    return ret;
}
int Rfor(Node &nod) {
    int ret = 0;
    for (run(nod.subs[0]); run(nod.subs[1]); run(nod.subs[2])) {
        ret = run(nod.subs[3]);
        if (isret) break;
    }
    return ret;
}
int Rret(Node &nod) {
    int ret = run(nod.subs[0]);
    isret = true;
    return ret;
}
int Rcall(Node &nod) {
    int tmp;
    rlo(i, nod.subs.size(), 0) msg.pb(tmp = run(nod.subs[i]));
    int res = run(func[nod.val]);
    push(res);
    return 0;
}

#define runop(x) _op[x]()
void (*_op[40])();
int Rope(Node &nod) {
    runop(nod.val);
    return 0;
}

#define dop(x, y) \
void x() { \
    int b = pop(); \
    int a = pop(); \
    push(y); \
}
dop(_add, a+b) dop(_sub, a-b) dop(_mul, a*b)
dop(_div, a/b) dop(_mod, a%b) dop(_le, a<=b)
dop(_ge, a>=b) dop(_l, a<b) dop(_g, a>b)
dop(_eq, a==b) dop(_neq, a!=b) dop(_xor, (!a)+(!b)==1)
dop(_and, a&&b) dop(_or, a||b)
void _not() {push(!pop());}
void _pos() {push(pop());}
void _neg() {push(-pop());}
void _ass() {
    int y = pop();
    vs[scur[-3]].top().v[scur[-2]] = y;
}
void _shl() {
    if (scur[-3] == 3) {
        pop();
        putchar('\n');
    } else printf("%d", pop());
}
void _shr() {
    vs[scur[-3]].top().v[scur[-2]] = ins[icur++];
    pop();
}
void _idx() {
    int y = pop();
    scur[-2] = scur[-2] * vs[scur[-3]].top().dmsz[scur[-1]++] + y;
}

int main(void) {
    exec[1] = Rfunc;
    exec[2] = Rblock;
    exec[3] = Rexp;
    exec[4] = Rlit;
    exec[5] = Ride;
    exec[6] = Rope;
    exec[7] = Rif;
    exec[8] = Rwhile;
    exec[9] = Rfor;
    exec[10] = Rret;
    exec[11] = Rcall;
    ides["cin"] = 1;
    ides["cout"] = 2;
    ides["endl"] = 3;
    ides["putchar"] = 4;
    keys["if"] = 1;
    keys["else"] = 2;
    keys["while"] = 3;
    keys["for"] = 4;
    keys["int"] = 5;
    keys["return"] = 6;
    opes["("] = 1;
    opes[")"] = 2;
    opes["["] = 3;
    _op[3] = _idx;
    opes["]"] = 4;
    opes["!"] = 5;
    _op[5] = _not;
    opes["+"] = 6;  // pos
    _op[6] = _pos;
    // plus is 7
    _op[7] = _add;
    opes["-"] = 8;  // neg
    _op[8] = _neg;
    // minus is 9
    _op[9] = _sub;
    opes["*"] = 10;
    _op[10] = _mul;
    opes["/"] = 11;
    _op[11] = _div;
    opes["%"] = 12;
    _op[12] = _mod;
    opes["<="] = 13;
    _op[13] = _le;
    opes[">="] = 14;
    _op[14] = _ge;
    opes["<"] = 15;
    _op[15] = _l;
    opes[">"] = 16;
    _op[16] = _g;
    opes["=="] = 17;
    _op[17] = _eq;
    opes["!="] = 18;
    _op[18] = _neq;
    opes["^"] = 19;
    _op[19] = _xor;
    opes["&&"] = 20;
    _op[20] = _and;
    opes["||"] = 21;
    _op[21] = _or;
    opes["="] = 22;
    _op[22] = _ass;
    opes["<<"] = 23;
    _op[23] = _shl;
    opes[">>"] = 24;
    _op[24] = _shr;
    opes[","] = 25;
    opes[";"] = 26;
    opes["{"] = 27;
    opes["}"] = 28;

    fread(buf, 1, buflim, stdin)[buf] = EOF;
    tknize();
    parse();
    loe(i, 1, 3) {
        vs[i].push(Var(i));
        alloc(vs[i].top().v, 1);
    }
    lo(i, 0, gbl.size()) {
        vs[gbl[i].id].push(gbl[i]);
        alloc(vs[gbl[i].id].top().v, gbl[i].sz);
    }
    func[4].val = -1;
    func[4].tp = 1;
    run(func[ides["main"]]);
    return 0;
}
