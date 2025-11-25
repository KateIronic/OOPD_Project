#include "basicIO.h"
// #include "BlockStandardIO.h"

#define SYS_READ 0
#define SYS_WRITE 1
#define STDIN 0
#define STDOUT 1


basicIO io;

extern "C" long syscall3(long number, long arg1, long arg2, long arg3);

static char inputBuffer[256];
static int inputBufPos = 0;
static int inputBufLen = 0;

void basicIO::activateInput() {
    for (int i = 0; i < 256; ++i) inputBuffer[i] = 0;
    inputBufPos = 0;
    inputBufLen = 0;
}

// helper to get next input char from buffered stdin reader
static int readNextChar() {
    char ch;
    if (inputBufPos >= inputBufLen) {
        long bytes = syscall3(SYS_READ, STDIN, (long)inputBuffer, 255);
        if (bytes <= 0) return -1;
        inputBufLen = (int)bytes;
        inputBufPos = 0;
    }
    ch = inputBuffer[inputBufPos++];
    return (unsigned char)ch;
}

int basicIO::inputint() {
    char ch = 0;
    long bytes = 0;
    // skip non-digit/non-sign characters
    bool neg = false;
    bool started = false;
    int result = 0;
    while (true) {
        int c = readNextChar();
        if (c < 0) {
            if (!started) return 0;
            break;
        }
        ch = (char)c;
        if (!started) {
            if (ch == '-') {
                neg = true;
                started = true;
                continue;
            }
            if (ch >= '0' && ch <= '9') {
                started = true;
                result = ch - '0';
                continue;
            }
            // skip other leading chars (spaces, newlines)
            continue;
        } else {
            if (ch >= '0' && ch <= '9') {
                result = result * 10 + (ch - '0');
                continue;
            }
            // stop at first non-digit after starting
            break;
        }
    }
    return neg ? -result : result;
}

const char* basicIO::inputstring() {
    inputstring(inputBuffer, 255);
    return inputBuffer;
}


void basicIO::inputstring(char* buffer, int size) {
    if (!buffer || size <= 0) return;

    char ch;
    int i = 0;

    while (i < size - 1) {
        long bytes = syscall3(SYS_READ, STDIN, (long)&ch, 1);
        if (bytes <= 0 || ch == '\n') break;
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
}

void basicIO::outputint(int number) {
    char buffer[32];
    int i = 0;
    bool isNegative = false;
    if (number == 0) {
        buffer[i++] = '0';
    } else {
        if (number < 0) {
            isNegative = true;
            number = -number;
        }
        while (number > 0) {
            buffer[i++] = '0' + (number % 10);
            number /= 10;
        }
        if (isNegative) {
            buffer[i++] = '-';
        }
    }
    for (int j = i - 1; j >= 0; --j) {
        syscall3(SYS_WRITE, STDOUT, (long)&buffer[j], 1);
    }
}

void basicIO::outputstring(const char* text) {
    long len = 0;
    while (text[len]) ++len;
    syscall3(SYS_WRITE, STDOUT, (long)text, len);
}

void basicIO::terminate() {
    char newline = '\n';
    syscall3(SYS_WRITE, STDOUT, (long)&newline, 1);
}

void basicIO::errorstring(const char* text) {
    long len = 0;
    while (text[len]) ++len;
    syscall3(SYS_WRITE, 2, (long)text, len);
}

void basicIO::errorint(int number) {
    char buffer[32];
    int i = 0;
    bool isNegative = false;
    if (number == 0) {
        buffer[i++] = '0';
    } else {
        if (number < 0) {
            isNegative = true;
            number = -number;
        }
        while (number > 0) {
            buffer[i++] = '0' + (number % 10);
            number /= 10;
        }
        if (isNegative) {
            buffer[i++] = '-';
        }
    }
    for (int j = i - 1; j >= 0; --j) {
        syscall3(SYS_WRITE, 2, (long)&buffer[j], 1);
    }
}
