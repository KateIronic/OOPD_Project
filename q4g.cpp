#include "q4g.h"

FourGSimulator::FourGSimulator() {}

int FourGSimulator::channelsFor(int total_khz) const {
    if (total_khz <= 0) return 0;
    return total_khz / SUB_BAND_KHZ;
}

int FourGSimulator::maxUsersFor(int total_khz, int antennas) const {
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;
    int ch = channelsFor(total_khz);
    // capacity per subchannel = USERS_PER_10KHZ * antennas
    return ch * (USERS_PER_10KHZ * antennas);
}

int FourGSimulator::listFirstSubchannel(int* deviceIds, int deviceCount, int total_khz, int antennas, int* outFirst, int maxOut) const {
    int ch = channelsFor(total_khz);
    if (ch <= 0) return 0;
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;
    int cap = USERS_PER_10KHZ * antennas; // effective capacity of first subchannel
    int written = 0;
    for (int i = 0; i < deviceCount; ++i) {
        int channelIndex = (i / cap) + 1; // 1-based channel index
        if (channelIndex == 1) {
            if (written < maxOut) outFirst[written] = deviceIds[i];
            ++written;
        }
    }
    return written;
}
