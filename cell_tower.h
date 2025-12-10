#pragma once
#ifndef CELL_TOWER_H
#define CELL_TOWER_H

#include "cell_device.h"

// CellTower manages frequency channels and assignment.
// It supports reuse across antennas: if antennas > 1, capacity per frequency multiplies by antennas.
class CellTower {
public:
    int frequencies;           // number of frequency channels
    int maxUsersPerFrequency;  // capacity per frequency (per antenna)
    int antennas;              // parallel antennas (MIMO reuse)

    CellTower(int freqs = 1, int perFreq = 1, int antennas_ = 1) {
        frequencies = (freqs > 0) ? freqs : 0;
        maxUsersPerFrequency = (perFreq > 0) ? perFreq : 0;
        antennas = (antennas_ >= 1) ? antennas_ : 1;
    }

    // total user capacity across all frequencies and antennas
    int capacity() const {
        return frequencies * maxUsersPerFrequency * antennas;
    }

    // assign devices round-robin into frequency indices 1..frequencies
    // devices: array of pointers length n
    void assignDevicesRoundRobin(CellDevice** devices, int n) const {
        if (frequencies <= 0) return;
        for (int i = 0; i < n; ++i) {
            if (devices[i]) {
                devices[i]->frequencyIndex = (i % frequencies) + 1;
            }
        }
    }

    // For printing, compute occupants per frequency up to capacity.
    // outputs: occupants is an array [frequencies+1][maxUsersPerFrequency*antennas] filled with user ids, and counts filled in counts[]
    // Caller must allocate occupants as int* occupantsFlat with size (frequencies+1) * (maxUsersPerFrequency*antennas)
    void usersPerFrequency(CellDevice** devices, int n, int* occupantsFlat, int* counts) const {
        if (frequencies <= 0) return;
        int perFreqCap = maxUsersPerFrequency * antennas;
        // zero counts
        for (int f = 1; f <= frequencies; ++f) counts[f] = 0;
        // iterate devices in insertion order
        for (int i = 0; i < n; ++i) {
            CellDevice* d = devices[i];
            if (!d) continue;
            int f = d->frequencyIndex;
            if (f >= 1 && f <= frequencies) {
                int c = counts[f];
                if (c < perFreqCap) {
                    // place at occupantsFlat[f * perFreqCap + c]
                    occupantsFlat[f * perFreqCap + c] = d->id;
                    counts[f] = c + 1;
                }
            }
        }
    }
};

#endif
