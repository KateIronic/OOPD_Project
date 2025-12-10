#pragma once
#ifndef CELLULAR_CORE_H
#define CELLULAR_CORE_H

#include "cell_device.h"

// CellularCore models core overhead and capacity in messages.
class CellularCore {
public:
    int overheadPer100; // extra messages per 100 messages
    int capacityMessages; // total messages core can handle

    CellularCore(int overhead = 0, int cap = 0) {
        overheadPer100 = (overhead >= 0) ? overhead : 0;
        capacityMessages = (cap >= 0) ? cap : 0;
    }

    // compute effective messages after overhead for a raw messages count
    int effectiveMessages(int rawMessages) const {
        if (rawMessages <= 0) return 0;
        int extra = (rawMessages / 100) * overheadPer100;
        return rawMessages + extra;
    }

    // return how many devices (in insertion order) can be supported by this core before exceeding capacity
    // devices: array of CellDevice* length n
    int maxDevicesSupported(CellDevice** devices, int n) const {
        int totalRaw = 0;
        for (int i = 0; i < n; ++i) {
            CellDevice* d = devices[i];
            if (!d) continue;
            totalRaw += d->messages;
            int eff = effectiveMessages(totalRaw);
            if (eff > capacityMessages) {
                return i; // previous devices only
            }
        }
        return n;
    }

    // compute cores required to fully support given total raw messages (ceiling).
    // returns -1 if per-core raw capacity is zero (i.e., impossible).
    int coresRequiredForRawMessages(int totalRawMessages) const {
        if (capacityMessages <= 0) return -1;
        // compute maximal raw R such that effectiveMessages(R) <= capacityMessages
        int perCoreRaw = 0;
        for (int r = 0; r <= capacityMessages; ++r) {
            if (effectiveMessages(r) <= capacityMessages) perCoreRaw = r;
            else break;
        }
        if (perCoreRaw <= 0) return -1;
        int cores = (totalRawMessages + perCoreRaw - 1) / perCoreRaw;
        return cores;
    }
};

#endif
