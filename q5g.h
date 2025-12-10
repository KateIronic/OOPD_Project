#ifndef Q5G_H
#define Q5G_H

#include "q4g.h"

// --------------------------------------------------------
// FiveGSimulator — inherits from FourGSimulator (4G base)
// - Main band: 10 kHz subchannels (same as 4G)
// - High band (1800 MHz region): 10 MHz extra (10000 kHz)
// - High-band capacity model: USERS_PER_1MHZ_HIGH users per 1 MHz
// - Massive MIMO supported via antennas up to MAX_ANTENNAS
// --------------------------------------------------------
class FiveGSimulator : public FourGSimulator {
public:
    // main band uses 10 kHz subchannels (same base as 4G)
    static const int MAIN_SUB_BAND_KHZ = 10;
    static const int USERS_PER_10KHZ_MAIN = 30; // default users per 10kHz in main band

    // high band: 10 MHz (10000 kHz) with USERS_PER_1MHZ_HIGH per 1 MHz
    static const int HIGH_BAND_KHZ = 10000;     // 10 MHz in kHz
    static const int USERS_PER_1MHZ_HIGH = 30;  // users per 1 MHz in high band

    static const int MESSAGES_PER_DEVICE = 10;  // per-device messages in 5G
    static const int MAX_ANTENNAS = 16;         // massive MIMO up to 16

    FiveGSimulator();

    // main band: number of 10 kHz subchannels inside given main_total_khz
    int mainSubchannelsFor(int main_total_khz) const;

    // high band size in MHz
    int highBandMHz() const;

    // capacity on main band (depends on antennas)
    int maxUsersMain(int main_total_khz, int antennas) const;

    // capacity on high band (depends on antennas)
    int maxUsersHigh(int antennas) const;

    // list occupants of first main 10kHz subchannel
    int listFirstMainSubchannel(int* deviceIds,
                                int deviceCount,
                                int main_total_khz,
                                int antennas,
                                int* outFirst,
                                int maxOut) const;

    // list occupants of first high 1MHz chunk (devices assigned after main band filled)
    int listFirstHighChunk(int* deviceIds,
                           int deviceCount,
                           int main_total_khz,
                           int antennas,
                           int* outFirst,
                           int maxOut) const;
};

#endif // Q5G_H
