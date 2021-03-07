#include <iostream>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

// 字符集的大小
const int CHARSIZE = 256;
// 自动机状态数上限
const int STATELIM = 200;

struct Pattern {
    int id, pos;
    Pattern(int i, int p) : id(i), pos(p) {}
};

// 模式串
string pat[] = {"I", "try", "my", "first"};

// encoding and decoding function
// 根据需要实现字符与编码的映射关系
// 编码应小于CHARSIZE
inline int charToCode(char ch) {
    return (int)ch;
}
inline char codeToChar(int code) {
    return (char)code;
}
// get the size of pattern whose index is idx
// 用在search(string)函数中，以计算出现pattern的起始位置
inline int patSize(int idx) {
    return pat[idx].size();
}

class ACauto {
private:
    const int NIL = -1;
    // trie
    int go[STATELIM][CHARSIZE];
    int gblcnt;
    int patcnt;
    // failure index
    int f[STATELIM];
    // end of word flag
    int ef[STATELIM];
    // valid flag: the ac automata has been constructed
    bool vf;
public:
    ACauto() {clear();}
    void clear();
    // insert word to trie
    void insert(const string &);
    // build ac automata
    void buildAC();
    // get next state
    int nextState(int, char);
    // search all pattern in the text
    vector<Pattern> search(const string &);
};

void ACauto::clear() {
    vf = false;
    gblcnt = 1;
    patcnt = 0;
    memset(go, NIL, sizeof go);
    memset(ef, 0, sizeof ef);
}

void ACauto::insert(const string &pat) {
    if (vf)
        for (int i = 0; i < CHARSIZE; i++)
            if (!go[0][i]) go[0][i] = NIL;
    vf = false;
    int crt = 0;
    int pl = pat.size();
    for (int i = 0; i < pl; i++) {
        int key = charToCode(pat[i]);
        if (go[crt][key] == NIL)
            go[crt][key] = gblcnt++;
        crt = go[crt][key];
    }
    ef[crt] |= 1 << (patcnt++);
}

void ACauto::buildAC() {
    if (vf) return;
    vf = true;
    memset(f, NIL, sizeof f);
    queue<int> q;
    for (int i = 0; i < CHARSIZE; i++) {
        if (go[0][i] == NIL) go[0][i] = 0;
        else {
            f[go[0][i]] = 0;
            q.push(go[0][i]);
        }
    }
    int crt;
    while (!q.empty()) {
        crt = q.front();
        q.pop();
        for (int i = 0; i < CHARSIZE; i++) {
            if (go[crt][i] != NIL) {
                int fail = f[crt];
                while (go[fail][i] == NIL)
                    fail = f[fail];
                fail = go[fail][i];
                f[go[crt][i]] = fail;
                ef[go[crt][i]] |= ef[fail];
                q.push(go[crt][i]);
            }
        }
    }
}

int ACauto::nextState(int crt, char key) {
    if (!vf) buildAC();
    int code = charToCode(key);
    while (go[crt][code] == NIL)
        crt = f[crt];
    return go[crt][code];
}

vector<Pattern> ACauto::search(const string &txt) {
    if (!vf) buildAC();
    vector<Pattern> ret;
    int crt = 0, pl = txt.size();
    for (int i = 0; i < pl; i++) {
        crt = nextState(crt, txt[i]);
        if (!ef[crt]) continue;
        for (int j = 0; j < patcnt; j++) {
            if (ef[crt] & (1 << j))
                ret.push_back(Pattern(j, i - patSize(j) + 1));
        }
    }
    return ret;
}

// 测试程序
int main(void) {
    string text = "I try to build a AC automata, and this is my first time I try to do this.";
    ACauto ac;
    for (int i = 0; i < 4; i++)
        ac.insert(pat[i]);
    vector<Pattern> ans = ac.search(text);
    for (auto x : ans) {
        cout << pat[x.id] << " appears from "
            << x.pos << " to " << x.pos + patSize(x.id) - 1 << "\n";
    }
    cout << endl;
    return 0;
}
