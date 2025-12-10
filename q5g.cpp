#include "q5g.h"

FiveGSimulator::FiveGSimulator() {}

// number of 10 kHz subchannels in the main band
int FiveGSimulator::mainSubchannelsFor(int main_total_khz) const {
    if (main_total_khz <= 0) return 0;
    return main_total_khz / MAIN_SUB_BAND_KHZ;
}

// high band size (MHz)
int FiveGSimulator::highBandMHz() const {
    return HIGH_BAND_KHZ / 1000; // convert kHz to MHz
}

// capacity in main band: subchannels * users_per_10kHz_main * antennas
int FiveGSimulator::maxUsersMain(int main_total_khz, int antennas) const {
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;
    int sub = mainSubchannelsFor(main_total_khz);
    int perSub = USERS_PER_10KHZ_MAIN * antennas;
    return sub * perSub;
}

// capacity in high band: high_band_MHz * users_per_1MHz_high * antennas
int FiveGSimulator::maxUsersHigh(int antennas) const {
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;
    int mhz = highBandMHz();
    int perMHz = USERS_PER_1MHZ_HIGH * antennas;
    return mhz * perMHz;
}

// list occupants of first main subchannel (first 10 kHz). Devices assigned sequentially to buckets
int FiveGSimulator::listFirstMainSubchannel(int* deviceIds,
                                            int deviceCount,
                                            int main_total_khz,
                                            int antennas,
                                            int* outFirst,
                                            int maxOut) const
{
    int sub = mainSubchannelsFor(main_total_khz);
    if (sub <= 0) return 0;
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;

    int cap = USERS_PER_10KHZ_MAIN * antennas;
    int written = 0;

    for (int i = 0; i < deviceCount; ++i) {
        int subIndex = (i / cap) + 1; // 1-based index
        if (subIndex == 1) {
            if (written < maxOut) outFirst[written] = deviceIds[i];
            ++written;
        }
    }
    return written;
}

// list occupants of first high 1MHz chunk (devices assigned after main band is filled)
int FiveGSimulator::listFirstHighChunk(int* deviceIds,
                                       int deviceCount,
                                       int main_total_khz,
                                       int antennas,
                                       int* outFirst,
                                       int maxOut) const
{
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;

    int mainCap = maxUsersMain(main_total_khz, antennas);
    int perMHz = USERS_PER_1MHZ_HIGH * antennas;

    int written = 0;
    for (int i = 0; i < deviceCount; ++i) {
        if (i >= mainCap && i < mainCap + perMHz) {
            if (written < maxOut) outFirst[written] = deviceIds[i];
            ++written;
        }
    }
    return written;
}
