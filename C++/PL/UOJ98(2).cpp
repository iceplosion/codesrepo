#include <bits/stdc++.h>
#define pb push_back
#define mk make_pair
// #define ENABLE_FREAD
#define lo(i, n, m) for (int i = n; i < m; ++i)
#define loe(i, n, m) for (int i = n; i <= m; ++i)
#define rlo(i, n, m) for (int i = n - 1; i >= m; --i)
#define rloe(i, n, m) for (int i = n; i >= m; --i)

using namespace std;

namespace readp {

    using namespace std;

// negative offset
#define NEG_OFFSET 1
// out of view
#define OOV 1

#define LF ((char)10)
#define CR ((char)13)
#define TAB ((char)9)
#define SPACE ((char)' ')

    const int BUFLIM = 150;
#ifdef ALL_READ_IN
    const int BUFCNT = 1;
#else
    const int BUFCNT = 2;
#endif

    const int TOKEN_LENGTH_LIM = 1 << 8;

    int crtbuf = 0;
    char buf[BUFCNT][BUFLIM];
    // versoin time stamp
    // 'verts' maintain the last version of buffer.
    int version[2], verts = 0;
    char *chptr = buf[0];
    char *eofptr;
    int bufidx_of_chptr = 0;
    bool EOFflag = false, peekEOF = false;
    // utility to fill the pos[]
    inline void _load(char *pos, int sz) {
        #ifdef ENABLE_FREAD
        int cnt;
        EOFflag = (cnt = fread(pos, 1, sz, stdin)) != sz;
        if (EOFflag) {
            pos[cnt] = EOF;
            eofptr = pos + cnt;
        }
        #else
        char ch = 0;
        // while (sz-- > 0 && ~(ch = _getchar_nolock())) *pos++ = ch;
        while (sz-- > 0 && ~(ch = getchar_unlocked())) *pos++ = ch;
        if (ch == EOF) {
            EOFflag = true, *pos = EOF;
            eofptr = pos;
        }
        #endif
    }
    inline void loadbuf() {
        _load(buf[crtbuf], BUFLIM);
        version[crtbuf] = ++verts;
        crtbuf ^= 1;
    }
    // peek the top character
    inline char peekTop() {
        if (peekEOF) return EOF;
        if (!verts) loadbuf();
        if (chptr - buf[bufidx_of_chptr] >= BUFLIM) {
            // switch to another buffer
            if (version[bufidx_of_chptr ^ 1] < version[bufidx_of_chptr])
                loadbuf();
            chptr = buf[bufidx_of_chptr ^= 1];
        }
        if (*chptr == EOF) {
            peekEOF = true;
            return EOF;
        }
        return *chptr;
    }
    // return 0x91 when try to peek a position out of view
    // cannot peek position with a negative position-offset
    inline char peekOff(int cx) {
        if (cx < 0) {
            printf("Error: atempt to peek with a negative offset.\n");
            exit(1);
        }
        if (peekEOF) return EOF;
        if (!verts) loadbuf();
        if (!cx) return peekTop();
        char *ptr = chptr;
        if (EOFflag) {
            if (cx + ptr - buf[bufidx_of_chptr] < eofptr - buf[bufidx_of_chptr]) {
                return *(cx + ptr);
            } else {
                return EOF;
            }
        } else {
            if (cx + ptr - buf[bufidx_of_chptr] < BUFLIM) {
                return *(ptr + cx);
            }
            cx -= (buf[bufidx_of_chptr] + BUFLIM) - ptr;
            ptr = buf[bufidx_of_chptr ^ 1];
            if (version[bufidx_of_chptr ^ 1] < version[bufidx_of_chptr]) {
                loadbuf();
            }
            if (EOFflag) {
                if (cx + ptr - buf[bufidx_of_chptr ^ 1] < eofptr - buf[bufidx_of_chptr ^ 1]) {
                    return *(cx + ptr);
                } else {
                    return EOF;
                }
            } else {
                if (cx + ptr - buf[bufidx_of_chptr ^ 1] < BUFLIM) {
                    return *(ptr + cx);
                } else {
                    printf("Error: peek out of view.\n");
                    exit(1);
                }
            }
        }
    }
    // read the first char remain in buffer
    inline char readTop() {
        if (peekEOF) return EOF;
        if (!verts) loadbuf();
        if (chptr - buf[bufidx_of_chptr] >= BUFLIM) {
            // switch to another buffer
            if (version[bufidx_of_chptr ^ 1] < version[bufidx_of_chptr])
                loadbuf();
            chptr = buf[bufidx_of_chptr ^= 1];
        }
        if (*chptr == EOF) {
            peekEOF = true;
            return EOF;
        }
        return *chptr++;
    }

    inline void skipChar() {
        readTop();
    }
    inline char readChar() {
        return readTop();
    }

    template <class T>
    inline bool isBlanks(T ch) {
        return (ch == SPACE || ch == LF || ch == CR || ch == TAB);
    }
    template <class T>
    inline bool isEoln(T ch) {
        return (ch == LF || ch == CR);
    }
    template <class T>
    inline bool isEof(T ch) {
        return ch == EOF;
    }
    inline bool isBlanks() {
        return isBlanks(peekTop());
    }
    inline bool isEoln() {
        return isEoln(peekTop());
    }
    inline bool isEof() {
        return isEof(peekTop());
    }
    inline bool isDigit() {
        return isdigit(peekTop());
    }
    inline bool isAlpha() {
        return isalpha(peekTop());
    }

    // return the number of blanks which were skiped
    inline int skipBlanks() {
        int ret = 0;
        while (isBlanks()) ++ret, skipChar();
        return ret;
    }
    inline int skipSpaces() {
        int ret = 0;
        while (peekTop() == ' ') ++ret, skipChar();
        return ret;
    }
    // return the index of the first non-blank character
    inline int skipBlanks(int crt) {
        while (isBlanks(peekOff(crt))) ++crt;
        return crt;
    }
    inline int skipSpaces(int crt) {
        while (peekOff(crt) == ' ') ++crt;
        return crt;
    }

    char token_buf[TOKEN_LENGTH_LIM], *token_buf_ptr = token_buf;
#define genWord() (string(token_buf))

/**
*   A macro of function to read a word. It continue to read characters till meeting
*   the first blank character or EOF.
*/
#define _read_word(function_name, accept_condition, append_statements) int function_name() {\
    token_buf_ptr = token_buf;  \
    skipBlanks();   \
    append_statements   \
    char ch;    \
    while (accept_condition && ~(ch = readTop())) {   \
        *token_buf_ptr++ = ch;   \
    }   \
    *token_buf_ptr = '\0';  \
    int ret = (token_buf_ptr - token_buf);  \
    if (ret > 0 && *(token_buf_ptr - 1) == EOF) {   \
        *(--token_buf_ptr) = '\0';  \
        --ret;  \
    }   \
    return ret;}

/**
*   A macro of function to peek a word (which do not change the 'chptr'). It continue
*   to read characters till meeting the first blank character or EOF.
*/
#define _peek_word(function_name, accept_condition, append_statements) int function_name() {    \
    token_buf_ptr = token_buf;  \
    int off = skipBlanks(0);    \
    append_statements   \
    char ch;    \
    while (accept_condition(peekOff(off)) && ~(ch = peekOff(off++))) {    \
        *token_buf_ptr++ = ch;  \
    }   \
    *token_buf_ptr = '\0';  \
    int ret = (token_buf_ptr - token_buf);  \
    if (ret > 0 && *(token_buf_ptr - 1) == EOF) {   \
        *(--token_buf_ptr) = '\0';  \
        --ret;  \
    }   \
    return ret;}

/**
*   A macro of function to peek a word which length not bigger than specified
*   length. It continue to read characters till meeting the first blank character or EOF.
*/
#define _peek_word_with_length(function_name, accept_condition, append_statements) int function_name(int length) {  \
    if (length < 0) {   \
        printf("Error: negative length.\n");    \
        exit(1);    \
    }   \
    token_buf_ptr = token_buf;  \
    int off = skipBlanks(0);    \
    append_statements   \
    char ch;    \
    while (accept_condition(peekOff(off)) && length-- > 0 && ~(ch = peekOff(off++))) {    \
        *token_buf_ptr++ = ch;  \
    }   \
    *token_buf_ptr = '\0';  \
    int ret = (token_buf_ptr - token_buf);  \
    if (ret > 0 && *(token_buf_ptr - 1) == EOF) {   \
        *(--token_buf_ptr) = '\0';  \
        --ret;  \
    }   \
    return ret;}

/**
*   A macro of function to read a word which length not bigger than specified length. It
*   continue to read characters till meeting the first blank character or EOF.
*/
#define _read_word_with_length(function_name, accept_condition, append_statements) int function_name(int length) {   \
    if (length < 0) {   \
        printf("Error: negative length.\n");    \
        exit(1);    \
    }   \
    token_buf_ptr = token_buf;  \
    skipBlanks();   \
    append_statements   \
    char ch;    \
    while (accept_condition && length-- > 0 && ~(ch = readTop())) {   \
        *token_buf_ptr++ = ch;   \
    }   \
    *token_buf_ptr = '\0';  \
    int ret = (token_buf_ptr - token_buf);  \
    if (ret > 0 && *(token_buf_ptr - 1) == EOF) {   \
        *(--token_buf_ptr) = '\0';  \
        --ret;  \
    }   \
    return ret;}

#define EMPTY_APPEND_STATEMENT ;
#define MINUS_OPERATOR_APPEND_STATEMENT_READ if (!EOFflag && peekTop() == '-') { \
    *token_buf_ptr++ = readTop();}
#define MINUS_OPERATOR_APPEND_STATEMENT_PEEK if (!EOFflag && peekOff(off) == '-') { \
    *token_buf_ptr++ = peekOff(off++);}
#define MINUS_OPERATOR_APPEND_STATEMENT_PEEK_WITH_LENGTH if (length > 0 && !EOFflag && peekOff(off) == '-') { \
    *token_buf_ptr++ = peekOff(off++); length--;}
#define MINUS_OPERATOR_APPEND_STATEMENT_READ_WITH_LENGTH if (length > 0 && !EOFflag && peekTop() == '-') {  \
    *token_buf_ptr++ = readTop(); length--;}

    // int readWord()
    _read_word(readWord, !isBlanks(), EMPTY_APPEND_STATEMENT)
    // int peekWord()
    // return the length of word
    _peek_word(peekWord, !isBlanks, EMPTY_APPEND_STATEMENT)
    // peek a word which length is not bigger than the limitation specified by 'length'(the param)
    _peek_word_with_length(peekWord, !isBlanks, EMPTY_APPEND_STATEMENT)
    // read a word which length is not bigger than the limitation specified by 'length'(the param)
    _read_word_with_length(readWord, !isBlanks(), EMPTY_APPEND_STATEMENT)

    // int peekDigitString
    _peek_word(peekDigitString, isdigit, MINUS_OPERATOR_APPEND_STATEMENT_PEEK)
    // int readDigitString()
    _read_word(readDigitString, isDigit(), MINUS_OPERATOR_APPEND_STATEMENT_READ)
    // int peekDigitString(int length)
    _peek_word_with_length(peekDigitString, isdigit, MINUS_OPERATOR_APPEND_STATEMENT_PEEK_WITH_LENGTH)
    // int readDigitString(int length)
    _read_word_with_length(readDigitString, isDigit(), MINUS_OPERATOR_APPEND_STATEMENT_READ_WITH_LENGTH)

    // bool isDigitWord()
    inline bool isDigitWord() {
        if (peekTop() == '-') {
            return isdigit(peekOff(1));
        }
        return isDigit();
    }
    // bool seekDigitWord()
    // return true if meet a digit word
    inline bool seekDigitWord() {
        while (!isDigitWord() && peekTop() != EOF) skipChar();
        return isDigitWord();
    }
    // NOTE: not safe if digit string is too long
    // int peekPosDigitWord()
    _peek_word(peekPosDigitWord, isdigit, EMPTY_APPEND_STATEMENT)
    // int readPosDigitWord()
    _read_word(readPosDigitWord, isDigit(), EMPTY_APPEND_STATEMENT)
    // int peekPosDigitWord(int length)
    _peek_word_with_length(peekPosDigitWord, isdigit, EMPTY_APPEND_STATEMENT)
    // int readPosDigitWord(int length)
    _read_word_with_length(readPosDigitWord, isDigit(), EMPTY_APPEND_STATEMENT)
    // bool isPosDigitWord()
    inline bool isPosDigitWord() {
        return isDigit();
    }
    // bool seekPosDigitWord()
    // return true if meet a digit word
    inline bool seekPosDigitWord() {
        while (!isPosDigitWord() && peekTop() != EOF) skipChar();
        return isPosDigitWord();
    }
    // stringToInt()
    inline int stringToInt(char *ptr) {
        int ret = 0, f = 1;
        if (*ptr == '-') f = -1, ++ptr;
        while (*ptr && isdigit(*ptr)) ret = (ret << 3) + (ret << 1) + *ptr++ - '0';
        return ret * f;
    }
    // stringToLongLong()
    inline long long stringToLongLong(char *ptr) {
        long long ret = 0, f = 1;
        if (*ptr == '-') f = -1, ++ptr;
        while (*ptr && isdigit(*ptr)) ret = (ret << 3) + (ret << 1) + *ptr++ - '0';
        return ret * f;
    }

    #define _query_num(function_name, datasource, convertion) inline function_name {\
        datasource; return convertion;}
    // readInt() and readInt(int length)
    _query_num(int readInt(), readDigitString(), stringToInt(token_buf))
    _query_num(int readInt(int length), readDigitString(length), stringToInt(token_buf))
    // peekInt() and peekInt(int length)
    _query_num(int peekInt(), peekDigitString(), stringToInt(token_buf))
    _query_num(int peekInt(int length), peekDigitString(length), stringToInt(token_buf))

    _query_num(long long readLongLong(), readDigitString(), stringToLongLong(token_buf))
    _query_num(long long readLongLong(int length), readDigitString(length), stringToLongLong(token_buf))
    _query_num(long long peekLongLong(), peekDigitString(), stringToLongLong(token_buf))
    _query_num(long long peekLongLong(int length), peekDigitString(length), stringToLongLong(token_buf))

    inline bool isHex(char ch) {
        return isdigit(ch) || ('A' <= ch && ch <= 'F') || ('a' <= ch && ch <= 'f');
    }
    inline bool isHex() {
        return isHex(peekTop());
    }

    _peek_word(peekHexWord, isHex, EMPTY_APPEND_STATEMENT);
    _read_word(readHexWord, isHex(), EMPTY_APPEND_STATEMENT);
    _peek_word_with_length(peekHexWord, isHex, EMPTY_APPEND_STATEMENT);
    _read_word_with_length(readHexWord, isHex(), EMPTY_APPEND_STATEMENT);

    inline int __hexDigitToDecimal(char ch) {
        if (isdigit(ch)) return ch - '0';
        else if ('A' <= ch && ch <= 'F') return 10 + ch - 'A';
        else if ('a' <= ch && ch <= 'f') return 10 + ch - 'a';
        return 0;
    }
    inline int hexToInt(char *ptr) {
        int ret = 0;
        while (*ptr) ret = (ret << 4) + __hexDigitToDecimal(*ptr++);
        return ret;
    }
    inline long long hexToLongLong(char *ptr) {
        long long ret = 0;
        while (*ptr) ret = (ret << 4) + __hexDigitToDecimal(*ptr++);
        return ret;
    }

    _query_num(int readHexToInt(), readHexWord(), hexToInt(token_buf));
    _query_num(int peekHexToInt(), peekHexWord(), hexToInt(token_buf));
    _query_num(int readHexToInt(int length), readHexWord(length), hexToInt(token_buf));
    _query_num(int peekHexToInt(int length), peekHexWord(length), hexToInt(token_buf));
    _query_num(int readHexToLongLong(), readHexWord(), hexToLongLong(token_buf));
    _query_num(int peekHexToLongLong(), peekHexWord(), hexToLongLong(token_buf));
    _query_num(int readHexToLongLong(int length), readHexWord(length), hexToLongLong(token_buf));
    _query_num(int peekHexToLongLong(int length), peekHexWord(length), hexToLongLong(token_buf));

    inline bool isBinary(char ch) {return ch == '0' || ch == '1';}
    inline bool isBinary() {return isBinary(peekTop());}

    _peek_word(peekBinaryWord, isBinary, EMPTY_APPEND_STATEMENT);
    _read_word(readBinaryWord, isBinary(), EMPTY_APPEND_STATEMENT);
    _peek_word_with_length(peekBinaryWord, isBinary, EMPTY_APPEND_STATEMENT);
    _read_word_with_length(readBinaryWord, isBinary(), EMPTY_APPEND_STATEMENT);

    inline int binaryToInt(char *ptr) {
        int ret = 0;
        while (*ptr) ret = (ret << 1) | (*ptr++ - '0');
        return ret;
    }
    inline long long binaryToLongLong(char *ptr) {
        long long ret = 0;
        while (*ptr) ret = (ret << 1) | (*ptr++ - '0');
        return ret;
    }

    _query_num(int readBinaryToInt(), readBinaryWord(), binaryToInt(token_buf));
    _query_num(int peekBinaryToInt(), peekBinaryWord(), binaryToInt(token_buf));
    _query_num(int readBinaryToInt(int length), readBinaryWord(length), binaryToInt(token_buf));
    _query_num(int peekBinaryToInt(int length), peekBinaryWord(length), binaryToInt(token_buf));
    _query_num(int readBinaryToLongLong(), readBinaryWord(), binaryToLongLong(token_buf));
    _query_num(int peekBinaryToLongLong(), peekBinaryWord(), binaryToLongLong(token_buf));
    _query_num(int readBinaryToLongLong(int length), readBinaryWord(length), binaryToLongLong(token_buf));
    _query_num(int peekBinaryToLongLong(int length), peekBinaryWord(length), binaryToLongLong(token_buf));

    // readAlphaWord and peekAlphaWord
    _peek_word(peekAlphaWord, isalpha, EMPTY_APPEND_STATEMENT);
    _read_word(readAlphaWord, isAlpha(), EMPTY_APPEND_STATEMENT);
    _peek_word_with_length(peekAlphaWord, isalpha, EMPTY_APPEND_STATEMENT);
    _read_word_with_length(readAlphaWord, isAlpha(), EMPTY_APPEND_STATEMENT);


#define CREATE_BS_CHARSET_FUNC(function_name, x) inline bool function_name(char ch) {  \
    static vector<char> charset;    \
    static bool init = true;    \
    if (init) {x; init = false; sort(charset.begin(), charset.end());}    \
    return binary_search(charset.begin(), charset.end(), ch);   \
}   \
inline bool function_name() {return function_name(peekTop());}

#define pbbs(a) charset.push_back(a);
#define pbbs_all(b, c) for (char i = b; i <= c; ++i) charset.push_back(i);

#define CREATE_MP_CHARSET_FUNC(function_name, x) inline bool function_name(char ch) {   \
    static bool charmp[256] = {0}; \
    static bool init = true;    \
    if (init) {x; init=false;}  \
    return charmp[int(ch)];  \
}   \
inline bool function_name() {return function_name(peekTop());}

#define pbmp(a) charmp[a] = true;
#define pbmp_all(b, c) for (char i = b; i <= c; ++i) charmp[int(i)] = true;

// example using of CREATE_BS_CHARSET_FUNC macro
// CREATE_BS_CHARSET_FUNC(myis, pbbs('a')pbbs('b')pbbs('c')pbbs_all('u', 'z'))
// _peek_word(peekMyWord, myis, EMPTY_APPEND_STATEMENT)

// example using of CREATE_MP_CHARSET_FUNC macro
// CREATE_MP_CHARSET_FUNC(myis, pbmp('a')pbmp('b')pbmp_all('u', 'z'))
// _peek_word(peekMyWord, myis, EMPTY_APPEND_STATEMENT)

/*
*   Enhanced read & peek word macro
*/
#define _seg_read_word(function_name, append_statements, read_statements) int function_name() {\
    token_buf_ptr = token_buf;  \
    skipBlanks();   \
    append_statements   \
    char ch;    \
    read_statements \
    *token_buf_ptr = '\0';  \
    int ret = (token_buf_ptr - token_buf);  \
    if (ret > 0 && *(token_buf_ptr - 1) == EOF) {   \
        *(--token_buf_ptr) = '\0';  \
        --ret;  \
    }   \
    return ret;}
#define _seg_read_word_read_state(accept_condition) while (accept_condition && ~(ch = readTop())) {   \
    *token_buf_ptr++ = ch;   \
}

#define _seg_peek_word(function_name, append_statements, peek_statements) int function_name() {    \
    token_buf_ptr = token_buf;  \
    int off = skipBlanks(0);    \
    append_statements   \
    char ch;    \
    peek_statements \
    *token_buf_ptr = '\0';  \
    int ret = (token_buf_ptr - token_buf);  \
    if (ret > 0 && *(token_buf_ptr - 1) == EOF) {   \
        *(--token_buf_ptr) = '\0';  \
        --ret;  \
    }   \
    return ret;}
#define _seg_peek_word_peek_state(accept_condition) while (accept_condition(peekOff(off)) && ~(ch = peekOff(off++))) {    \
    *token_buf_ptr++ = ch;  \
}

#define _seg_peek_word_with_length(function_name, append_statements, peek_statements) int function_name(int length) {  \
    if (length < 0) {   \
        printf("Error: negative length.\n");    \
        exit(1);    \
    }   \
    token_buf_ptr = token_buf;  \
    int off = skipBlanks(0);    \
    append_statements   \
    char ch;    \
    peek_statements \
    *token_buf_ptr = '\0';  \
    int ret = (token_buf_ptr - token_buf);  \
    if (ret > 0 && *(token_buf_ptr - 1) == EOF) {   \
        *(--token_buf_ptr) = '\0';  \
        --ret;  \
    }   \
    return ret;}
#define _seg_peek_word_with_length_peek_state(accept_condition) while (accept_condition(peekOff(off)) && length-- > 0 && ~(ch = peekOff(off++))) {    \
    *token_buf_ptr++ = ch;  \
}

#define _seg_read_word_with_length(function_name, append_statements, read_statements) int function_name(int length) {   \
    if (length < 0) {   \
        printf("Error: negative length.\n");    \
        exit(1);    \
    }   \
    token_buf_ptr = token_buf;  \
    skipBlanks();   \
    append_statements   \
    char ch;    \
    read_statements   \
    *token_buf_ptr = '\0';  \
    int ret = (token_buf_ptr - token_buf);  \
    if (ret > 0 && *(token_buf_ptr - 1) == EOF) {   \
        *(--token_buf_ptr) = '\0';  \
        --ret;  \
    }   \
    return ret;}
#define _seg_read_word_with_length_read_state(accept_condition) while (accept_condition && length-- > 0 && ~(ch = readTop())) {   \
    *token_buf_ptr++ = ch;   \
}

// read & peek Token
CREATE_MP_CHARSET_FUNC(istokenhead, pbmp_all('a', 'z')pbmp_all('A', 'Z')pbmp('_'))
CREATE_MP_CHARSET_FUNC(istoken, pbmp_all('a', 'z')pbmp_all('A', 'Z')pbmp('_')pbmp_all('0','9'))
_seg_peek_word(peekToken, EMPTY_APPEND_STATEMENT, _seg_peek_word_peek_state(istokenhead)_seg_peek_word_peek_state(istoken))
_seg_read_word(readToken, EMPTY_APPEND_STATEMENT, _seg_read_word_read_state(istokenhead())_seg_read_word_read_state(istoken()))

    void readLine() {
        token_buf_ptr = token_buf;
        char ch;
        while (~(ch = peekTop()) && ch != '\n') *token_buf_ptr++ = readTop();
        *token_buf_ptr = '\0';
    }
    void readLine(char *ptr) {
        char ch;
        while (~(ch = peekTop()) && ch != '\n') *ptr++ = readTop();
        *ptr = '\0';
    }
    void readLine(string &s) {
        char ch;
        while (~(ch = peekTop()) && ch != '\n') s += readTop();
    }

    void peekLine() {
        token_buf_ptr = token_buf;
        char ch;
        int pos = 0;
        while (~(ch = peekOff(pos)) && ch != '\n') {
            *token_buf_ptr++ = ch;
            ++pos;
        }
        *token_buf_ptr = '\0';
    }
    void peekLine(char *ptr) {
        char ch;
        int pos = 0;
        while (~(ch = peekOff(pos)) && ch != '\n') {
            *ptr++ = ch;
            ++pos;
        }
        *ptr = '\0';
    }
    void peekLine(string &s) {
        char ch;
        int pos = 0;
        while (~(ch = peekOff(pos)) && ch != '\n') {
            s += ch;
            ++pos;
        }
    }

    /**
    *   pattern class
    *   NOTE: This class is adpated from the pattern class in testlib.h:
    *   https://github.com/MikeMirzayanov/testlib
    */
    class pattern {
    public:
        pattern(const string &s);
        bool matches(const string &s, size_t pos) const;
        string c_str() const;
    private:
        string str;
        vector<char> charset;
        vector<pattern> subnodes;
        int from, to;
    };

#define ILLEGAL_PATTERN_EXIT(x) {printf("Error: Illegal pattern %s\n", x.c_str());exit(1);}

    bool isCommandChar(const string &str, size_t idx, char ch) {
        if (idx < 0 || idx >= str.size()) return false;
        int slashes = 0;
        int from = int(idx) - 1;
        while (from >= 0 && str[from] == '\\')
            --from, ++slashes;
        return !(slashes & 1) && str[idx] == ch;
    }
    char _pattern_getchar(string &str, size_t &idx) {
        char ret;
        if (str[idx] == '\\') {
            ++idx;
            if (str[idx] == 'n') ret = '\n';
            else if (str[idx] == 't') ret = '\t';
            else ret = str[idx];
        } else {
            ret = str[idx];
        }
        ++idx;
        return ret;
    }
    int greedyMatch(const string &str, size_t &idx, const vector<char> charset) {
        int ret = 0;
        while (idx < str.size()) {
            char ch = str[idx++];
            if (!binary_search(charset.begin(), charset.end(), ch))
                break;
            else
                ++ret;
        }
        return ret;
    }
    vector<char> scanCharSet(string &str, size_t &idx) {
        if (idx < 0 || idx >= str.size()) ILLEGAL_PATTERN_EXIT(str)
        vector<char> res;
        if (isCommandChar(str, idx, '[')) {
            ++idx;
            bool neg_flag;
            if ((neg_flag = isCommandChar(str, idx, '^')))
                ++idx;
            char pre = '\0';
            while (idx < str.size() && !isCommandChar(str, idx, ']')) {
                if (isCommandChar(str, idx, '-') && pre) {
                    ++idx;
                    if (idx + 1 >= str.size() || isCommandChar(str, idx, ']')) {
                        res.push_back(pre);
                        pre = '-';
                        continue;
                    }
                    char nxt = _pattern_getchar(str, idx);
                    if (pre > nxt) ILLEGAL_PATTERN_EXIT(str)
                    for (char ch = pre; ch != nxt; ++ch)
                        res.push_back(ch);
                    res.push_back(nxt);
                    pre = 0;
                } else {
                    if (pre) res.push_back(pre);
                    pre = _pattern_getchar(str, idx);
                }
            }
            if (pre) res.push_back(pre);
            if (!isCommandChar(str, idx, ']')) ILLEGAL_PATTERN_EXIT(str)
            ++idx;
            if (neg_flag) {
                sort(res.begin(), res.end());
                vector<char> comp;
                for (int code = 0, res_pos = 0; code < 255; ++code) {
                    if (res_pos < int(res.size()) && res[res_pos] == code) {
                        ++res_pos;
                        continue;
                    }
                    comp.push_back(code);
                }
                res = comp;
            }
            sort(res.begin(), res.end());
        } else {
            res.push_back(_pattern_getchar(str, idx));
        }
        return res;
    }
    void scanCounts(string &str, size_t &idx, int &from, int &to) {
        if (idx >= str.size()) {
            from = to = 1;
            return;
        }
        if (isCommandChar(str, idx, '{')) {
            vector<string> parts;
            string part = "";
            idx++;
            while (idx < str.size() && !isCommandChar(str, idx, '}')) {
                if (isCommandChar(str, idx, ','))
                    parts.push_back(part), part = "", idx++;
                else
                    part += _pattern_getchar(str, idx);
            }
            if (part != "") parts.push_back(part);
            if (!isCommandChar(str, idx, '}')) ILLEGAL_PATTERN_EXIT(str)
            ++idx;
            if (parts.size() < 1 || parts.size() > 2) ILLEGAL_PATTERN_EXIT(str)
            vector<int> numbers;
            for (size_t i = 0; i < parts.size(); i++) {
                if (parts[i].length() == 0) ILLEGAL_PATTERN_EXIT(str)
                int number;
                if (sscanf(parts[i].c_str(), "%d", &number) != 1)
                    ILLEGAL_PATTERN_EXIT(str)
                numbers.push_back(number);
            }
            if (numbers.size() == 1)
                from = to = numbers[0];
            else
                from = numbers[0], to = numbers[1];
            if (from > to) ILLEGAL_PATTERN_EXIT(str)
        } else {
            if (isCommandChar(str, idx, '?')) {
                from = 0, to = 1, idx++;
                return;
            }
            if (isCommandChar(str, idx, '*')) {
                from = 0, to = INT_MAX, idx++;
                return;
            }
            if (isCommandChar(str, idx, '+')) {
                from = 1, to = INT_MAX, idx++;
                return;
            }
            from = to = 1;
        }
    }
    pattern::pattern(const string &s) : from(0), to(0) {
        string tmp;
        // remove the blank space from string s
        for (size_t i = 0; i < s.size(); ++i) {
            if (!isCommandChar(s, i, ' '))
                tmp += s[i];
        }
        str = tmp;
        int opened = 0, fstClose = -1;
        vector<int> seps;
        for (size_t i = 0; i < str.size(); ++i) {
            if (isCommandChar(str, i, '(')) {
                ++opened;
                continue;
            }
            if (isCommandChar(str, i, ')')) {
                --opened;
                if (opened == 0 && fstClose == -1)
                    fstClose = int(i);
                continue;
            }
            if (opened < 0) {
                printf("Error: Illegal pattern %s\n", str.c_str());
                exit(1);
            }
            if (isCommandChar(str, i, '|') && opened == 0) {
                seps.push_back(int(i));
            }
        }
        if (opened) {
            printf("Error: Illegal pattern %s\n", str.c_str());
            exit(1);
        }
        if (!seps.size() && fstClose + 1 == (int)str.size()
            && isCommandChar(str, 0, '(') && isCommandChar(str, str.size() - 1, ')')) {
            subnodes.push_back(pattern(str.substr(1, str.size() - 2)));
        } else if (seps.size() > 0) {
            seps.push_back(int(str.size()));
            int idx = 0;
            for (size_t i = 0; i < seps.size(); ++i) {
                subnodes.push_back(pattern(str.substr(idx, seps[i] - idx)));
                idx = seps[i] + 1;
            }
        } else {
            size_t idx = 0;
            charset = scanCharSet(str, idx);
            scanCounts(str, idx, from, to);
            if (idx < str.size()) {
                subnodes.push_back(pattern(str.substr(idx)));
            }
        }
    }

    string pattern::c_str() const {
        return str;
    }
    bool pattern::matches(const string &s, size_t pos = 0) const {
        string ret;
        if (to > 0) {
            int sz = greedyMatch(s, pos, charset);
            if (sz < from) return false;
            if (sz > to) sz = to;
            pos += sz;
        }
        if (subnodes.size()) {
            for (size_t i = 0; i < subnodes.size(); ++i)
                if (subnodes[i].matches(s, pos))
                    return true;
            return false;
        } else {
            return pos == s.size();
        }
    }

    inline void tolower(string &s) {
        for (size_t i = 0; i < s.size(); ++i) {
            if ('A' <= s[i] && s[i] <= 'Z') s[i] += 32;
        }
    }
    inline void toupper(string &s) {
        for (size_t i = 0; i < s.size(); ++i) {
            if ('a' <= s[i] && s[i] <= 'z') s[i] -= 32;
        }
    }
    inline void trim(string &s) {
        size_t from = 0, to = s.size();
        while (from < s.size() && isBlanks(s[from])) ++from;
        while (to >= s.size() && isBlanks(s[to])) --to;
        if (to < from) s = "";
        else s = s.substr(from, to - from + 1);
    }

// uncomment the following macros to enable short-name of util functions
#define Ri readInt
#define Pi peekInt
#define Rll readLongLong
#define Pll peekLongLong
#define Pw peekWord
#define Rw readWord
}
using namespace readp;

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
#define isbk isBlanks
#define isdg isDigit
#define isal isAlpha
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
    while (peekTop() != ';') skipChar();
    skipChar();
    string wdm;
    while (~peekTop()) {
        if (isBlanks()) {
            skipChar();
            continue;
        } else if (isal() || peekTop() == '_') {
            readToken();
            wdm = genWord();
            if (keys.count(wdm)) Itk(0, keys[wdm]);
            else {
                if (!ides.count(wdm)) ides[wdm] = idid++;
                Itk(1, ides[wdm]);
            }
        } else if (isdg()) {
            int x = Ri();
            Itk(2, x);
        } else {
            static char wdb[5];
            wdb[0] = readTop();
            wdb[1] = peekTop();
            wdb[2] = 0;
            wdm = string(wdb);
            if (opes.count(wdm)) {
                skipChar();
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
