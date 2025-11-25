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
    // Read next token (sequence of non-whitespace chars) and parse as integer.
    while (true) {
        // skip leading whitespace
        int c = readNextChar();
        if (c < 0) return IO_EOF;
        while (c >= 0 && (c == ' ' || c == '\t' || c == '\r' || c == '\n')) {
            c = readNextChar();
            if (c < 0) return IO_EOF;
        }

        // collect token
        char token[64];
        int tpos = 0;
        bool hasDigit = false;
        while (c >= 0 && c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            if (tpos < (int)sizeof(token) - 1) token[tpos++] = (char)c;
            if (c >= '0' && c <= '9') hasDigit = true;
            c = readNextChar();
            if (c < 0) break;
        }
        token[tpos] = '\0';

        if (!hasDigit) {
            // invalid token for integer; notify user and continue to next token
            const char* msg = "Invalid integer input; please enter a number\n";
            long len = 0; while (msg[len]) ++len;
            syscall3(SYS_WRITE, 2, (long)msg, len);
            if (c < 0) return IO_EOF;
            continue; // read next token
        }

        // parse integer from token (handle optional leading sign)
        int idx = 0;
        bool neg = false;
        if (token[idx] == '+' || token[idx] == '-') {
            if (token[idx] == '-') neg = true;
            ++idx;
        }
        long result = 0;
        while (token[idx]) {
            char ch2 = token[idx];
            if (ch2 >= '0' && ch2 <= '9') {
                result = result * 10 + (ch2 - '0');
            } else {
                // stop parsing at first non-digit inside token
                break;
            }
            ++idx;
        }
        if (neg) result = -result;
        return (int)result;
    }
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
