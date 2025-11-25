#ifndef Q5G_H
#define Q5G_H

#include "q4g.h"

class FiveGSimulator : public FourGSimulator {
public:
    // main band uses 10 kHz subchannels (same as 4G base)
    static const int MAIN_SUB_BAND_KHZ = 10; // 10 kHz
    // Capacity per 10 kHz subchannel on the main band.
    // By default we use the same per-10kHz user capacity as Q4 (30 users per 10 kHz).
    // If your assignment specifies a different density for 5G main band, change this value here.
    static const int USERS_PER_10KHZ_MAIN = 30; // users per 10 kHz for main band (default: 30)

    // 1800 MHz band: 10 MHz (10000 kHz) with 30 users per 1 MHz
    static const int HIGH_BAND_KHZ = 10000; // 10 MHz
    static const int USERS_PER_1MHZ_HIGH = 30; // 30 users per 1 MHz

    static const int MESSAGES_PER_DEVICE = 10; // same per-device messages
    static const int MAX_ANTENNAS = 16; // massive MIMO up to 16

    FiveGSimulator();

    int mainSubchannelsFor(int main_total_khz) const;
    int highBandMHz() const;

    int maxUsersMain(int main_total_khz, int antennas) const;
    int maxUsersHigh(int antennas) const;

    // assignment helpers: list occupants of first main subchannel and first high 1MHz chunk
    int listFirstMainSubchannel(int* deviceIds, int deviceCount, int main_total_khz, int antennas, int* outFirst, int maxOut) const;
    int listFirstHighChunk(int* deviceIds, int deviceCount, int main_total_khz, int antennas, int* outFirst, int maxOut) const;
};

#endif // Q5G_H
