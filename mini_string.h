#pragma once

#ifndef MINI_STRING_H
#define MINI_STRING_H

// Minimal fixed-size string class. No std::string used.
#define MINI_STR_LEN 64

class MiniString {
public:
    char buf[MINI_STR_LEN];

    MiniString() {
        buf[0] = '\0';
    }

    MiniString(const char* s) {
        set(s);
    }

    void set(const char* s) {
        if (!s) {
            buf[0] = '\0';
            return;
        }
        int i = 0;
        // copy up to MINI_STR_LEN-1
        while (s[i] != '\0' && i < MINI_STR_LEN - 1) {
            buf[i] = s[i];
            ++i;
        }
        buf[i] = '\0';
    }

    // safe concat (appends src to this)
    void append(const char* src) {
        if (!src) return;
        int i = 0;
        while (buf[i] != '\0' && i < MINI_STR_LEN - 1) ++i;
        int j = 0;
        while (src[j] != '\0' && i < MINI_STR_LEN - 1) {
            buf[i++] = src[j++];
        }
        buf[i] = '\0';
    }

    const char* c_str() const { return buf; }

    // simple compare: returns 0 if equal, <0 if this < other, >0 otherwise
    int cmp(const char* other) const {
        int i = 0;
        while (buf[i] != '\0' && other[i] != '\0') {
            if (buf[i] != other[i]) return (int)(buf[i]) - (int)(other[i]);
            ++i;
        }
        return (int)(buf[i]) - (int)(other[i]);
    }
};

#endif
