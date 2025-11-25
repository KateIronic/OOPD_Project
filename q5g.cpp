#include "q5g.h"

FiveGSimulator::FiveGSimulator() {}

int FiveGSimulator::mainSubchannelsFor(int main_total_khz) const {
    if (main_total_khz <= 0) return 0;
    return main_total_khz / MAIN_SUB_BAND_KHZ;
}

int FiveGSimulator::highBandMHz() const {
    return HIGH_BAND_KHZ / 1000; // convert kHz to MHz
}

int FiveGSimulator::maxUsersMain(int main_total_khz, int antennas) const {
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;
    int sub = mainSubchannelsFor(main_total_khz);
    int perSub = USERS_PER_10KHZ_MAIN * antennas;
    return sub * perSub;
}

int FiveGSimulator::maxUsersHigh(int antennas) const {
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;
    int mhz = highBandMHz();
    int perMHz = USERS_PER_1MHZ_HIGH * antennas;
    return mhz * perMHz;
}

int FiveGSimulator::listFirstMainSubchannel(int* deviceIds, int deviceCount, int main_total_khz, int antennas, int* outFirst, int maxOut) const {
    int sub = mainSubchannelsFor(main_total_khz);
    if (sub <= 0) return 0;
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;
    int cap = USERS_PER_10KHZ_MAIN * antennas;
    int written = 0;
    for (int i = 0; i < deviceCount; ++i) {
        int idx = i;
        if (idx < cap) {
            if (written < maxOut) outFirst[written] = deviceIds[i];
            ++written;
        }
    }
    return written;
}

int FiveGSimulator::listFirstHighChunk(int* deviceIds, int deviceCount, int main_total_khz, int antennas, int* outFirst, int maxOut) const {
    // devices assigned after main band is filled; first high chunk capacity:
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
