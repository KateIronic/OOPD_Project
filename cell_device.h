#pragma once
#ifndef CELL_DEVICE_H
#define CELL_DEVICE_H

#include "mini_string.h"

enum DeviceType {
    DEVICE_VOICE = 1,
    DEVICE_DATA  = 2,
    DEVICE_BOTH  = 3
};

enum DeviceStatus {
    STATUS_ACTIVE,
    STATUS_DROPPED
};

class CellDevice {
public:
    int id;
    int type;            // 1,2,3
    int messages;        // messages generated per connection
    int frequencyIndex;  // 1-based assigned frequency, -1 if none
    int status;          // STATUS_ACTIVE or STATUS_DROPPED
    MiniString label;    // optional small label (not required)

    CellDevice(int id_, int t_, int msgs_) {
        id = id_;
        type = t_;
        messages = msgs_;
        frequencyIndex = -1;
        status = STATUS_DROPPED;
        label.set("");
    }

    void setLabel(const char* s) { label.set(s); }

    // fill dest with type name
    void getTypeName(char* dest, int destSize) const {
        if (!dest || destSize <= 0) return;
        if (type == DEVICE_VOICE) {
            // copy "VOICE"
            const char* t = "VOICE";
            int i = 0;
            while (t[i] != '\0' && i < destSize - 1) { dest[i] = t[i]; ++i; }
            dest[i] = '\0';
        } else if (type == DEVICE_DATA) {
            const char* t = "DATA";
            int i = 0;
            while (t[i] != '\0' && i < destSize - 1) { dest[i] = t[i]; ++i; }
            dest[i] = '\0';
        } else {
            const char* t = "BOTH";
            int i = 0;
            while (t[i] != '\0' && i < destSize - 1) { dest[i] = t[i]; ++i; }
            dest[i] = '\0';
        }
    }
};

#endif
