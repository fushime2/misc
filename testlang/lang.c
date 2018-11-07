#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *p;
static char func[26][100];

__attribute__((noreturn)) static void error(char *fmt, ...) {
    // 引数をまとめる (...)
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

static void skip() {
    while (isspace(*p)) p++;
}

static char *isoperator(const char c) {
    return strchr("+-*/", c);
}

// buf = "+ . .\0"
static void read_until(const char c, char *buf) {
    for (; *p != c; p++, buf++)
        *buf = *p;
    p++;
    *buf = '\0';
}

static void expect(const char c) {
    if (*p != c)
        error("%c expected: %s", c, p);
    p++;
}


static int eval(int*); 
static int eval_string(char *code, int *args) {
    char *orig = p;
    p = code;
    int val = 0;
    while (*p)
        val = eval(args);
    p = orig;
    return val;
}

// todo
// fucntion definition
// function application
//
static int eval(int *args) {
    skip();
    // function params
    if ('a' <= *p && *p <= 'z') {
        return args[*p++ - 'a'];
    }
    // function definition
    if ('A' <= *p && *p <= 'Z' && p[1] == '[') {
        char name = *p;
        p += 2;
        read_until(']', func[name - 'A']);
        return eval(args);
    }
    
    // "P" print primitive
    if (*p == 'P') {
        p++;
        expect('(');
        int val = eval(args);
        expect(')');
        printf("%d\n", val);
        return val;
    }

    // fucttion application
    if ('A' <= *p && *p <= 'Z' && p[1] == '(') {
        int newargs[26];
        char name = *p;
        p += 2;
        int i = 0;
        for (skip(); *p != ')'; skip())
            newargs[i++] = eval(args);
        expect(')');
        return eval_string(func[name - 'A'], newargs);
    }

    if (isdigit(*p)) {
        int val = *p++ - '0';
        while (isdigit(*p))
            val = 10 * val + (*p++ - '0');
        return val;
    }

    if (isoperator(*p)) {
        char op = *p++;
        int x = eval(args);
        int y = eval(args);
        switch (op) {
            case '+': return x + y;
            case '-': return x - y;
            case '*': return x * y;
            case '/': return x / y;
            default: error("?");
        }
    }
    error("invalid character: %c", *p);
}

void unittest() {
}

int main(int argc, char **argv) {
    p = argv[1];
    while (*p) {
        printf("%d\n", eval(0));
    }
    return 0;
}
