#include "q3g.h"

ThreeGSimulator::ThreeGSimulator() {}

int ThreeGSimulator::channelsFor(int total_khz) const {
    if (total_khz <= 0) return 0;
    return total_khz / BAND_KHZ;
}

int ThreeGSimulator::maxUsersFor(int total_khz, int antennas) const {
    (void)antennas; // 3G doesn't use MIMO antennas in this model
    int ch = channelsFor(total_khz);
    return ch * USERS_PER_200KHZ;
}

int ThreeGSimulator::listFirstChannel(int* deviceIds, int deviceCount, int total_khz, int* outFirst, int maxOut) const {
    int ch = channelsFor(total_khz);
    if (ch <= 0) return 0;
    int cap = USERS_PER_200KHZ;
    int written = 0;
    for (int i = 0; i < deviceCount; ++i) {
        int channelIndex = (i / cap) + 1; // 1-based channel
        if (channelIndex == 1) {
            if (written < maxOut) outFirst[written] = deviceIds[i];
            ++written;
        }
    }
    return written;
}
