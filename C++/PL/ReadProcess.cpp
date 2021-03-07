#include <bits/stdc++.h>

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

    const int BUFLIM = 1000;
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
        while (sz-- > 0 && ~(ch = _getchar_nolock())) *pos++ = ch;
        // while (sz-- > 0 && ~(ch = getchar_unlocked())) *pos++ = ch;
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
#define _seg_read_word_limited_state(accept_condition, limit) for (int i = 0; i < limit && accept_condition && ~(ch = readTop()); ++i) { \
    *token_buf_ptr++ = ch;  \
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
#define _seg_peek_word_limited_state(accept_condition, limit) for (int i = 0; i < limit && accept_condition(peekOff(off)) && ~(ch = peekOff(off++)); ++i) {    \
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
#define _seg_peek_word_with_length_limited_state(accept_condition, limit) for (int i = 0; i < limit && accept_condition(peekOff(off)) && length-- > 0 && ~(ch = peekOff(off++)); ++i) {    \
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
#define _seg_read_word_with_length_limited_state(accept_condition, limit) for (int i = 0; i < limit && accept_condition && length-- > 0 && ~(ch = readTop()); ++i) {   \
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

    // convert hex string to value
    template<class T>
    auto xs2v(const string &s, int from, int to, T type) -> T {
        T ret = 0;
        for (int i = from; i < to; ++i) {
            char ch = s[i];
            ret = (ret << 4) | __hexDigitToDecimal(ch);
        }
        return ret;
    }
    template <class T>
    auto xs2v(const string &s, T type) -> T {
        return xs2v(s, 0, s.size(), type);
    }

    //  convert unsigned decimal string to value
    template <class T>
    auto uds2v(const string &s, int from, int to, T type) -> T {
        T ret = 0;
        for (int i = from; i < to; ++i) {
            char ch = s[i];
            ret = (ret << 3) + (ret << 1) + ch - '0';
        }
        return ret;
    }
    template <class T>
    auto uds2v(const string &s, T type) -> T {
        return uds2v(s, 0, s.size(), type);
    }

    // convert signed decimal string to value
    template <class T>
    auto ds2v(const string &s, int from, int to, T type) -> T {
        T ret = 0, f = 1;
        int i = from;
        if (s[from] == '-') ++from, f = -1;
        for (; i < to; ++i) {
            char ch = s[i];
            ret = (ret << 3) + (ret << 1) + ch - '0';
        }
        return ret * f;
    }
    template <class T>
    auto ds2v(const string &s, T type) -> T {
        return ds2v(s, 0, s.size(), type);
    }

    // convert binary string to value
    template <class T>
    auto bs2v(const string &s, int from, int to, T type) -> T {
        T ret = 0;
        for (int i = from; i < to; ++i) {
            char ch = s[i];
            ret = (ret << 1) | (ch - '0');
        }
        return ret;
    }
    template <class T>
    auto bs2v(const string &s, T type) -> T {
        return bs2v(s, 0, s.size(), type);
    }

#define _string_checker(function_prototype, constraints) bool function_prototype {   \
    constraints;    \
    return true;    \
}
#define _loop_const(func, from, to) for (int i = from; i < to; ++i) {if (!func(str[i])) return false;}

_string_checker(isDigit(const string &str), _loop_const(isdigit, 0, int(str.size())))
_string_checker(isDigit(const string &str, int from, int to), _loop_const(isdigit, from, to))
_string_checker(isHex(const string &str), _loop_const(isHex, 0, int(str.size())))
_string_checker(isHex(const string &str, int from, int to), _loop_const(isHex, from, to))

// uncomment the following macros to enable short-name of util functions
#define Ri readInt
#define Pi peekInt
#define Rll readLongLong
#define Pll peekLongLong
#define Pw peekWord
#define Rw readWord
}

// test driver
using namespace readp;
int main(void) {
    // while (true) {
    //     Rw();
    //     string ops = genWord();
    //     trim(ops);  tolower(ops);
    //     if (ops == "word") {
    //         Pw();
    //         string str = genWord();
    //         printf("Origin: %s\n", str.c_str());
    //         trim(str);
    //         toupper(str);
    //         printf("TOUPPER: %s\n", str.c_str());
    //         Rw();
    //         str = genWord();
    //         trim(str);
    //         tolower(str);
    //         printf("tolower: %s\n", str.c_str());
    //     } else if (ops == "integer") {
    //         int x = Ri();
    //         printf("Integer: %d\n", x);
    //     } else if (ops == "long") {
    //         long long x = Rll();
    //         printf("Long Long: %lld\n", x);
    //     } else if (ops == "end") {
    //         break;
    //     }
    // }

    // test of token reading and peeking
    while (~peekTop()) {
        peekToken();
        string token = genWord();
        printf("TOken: %s\n", token.c_str());
        readToken();
        token = genWord();
        printf("token is %s\n", token.c_str());
        printf("--------------------------------------------------\n");
    }
    return 0;
}
