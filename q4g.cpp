#include "q4g.h"

FourGSimulator::FourGSimulator() {}

// subchannels are 10 kHz units
int FourGSimulator::channelsFor(int total_khz) const {
    if (total_khz <= 0) return 0;
    return total_khz / SUB_BAND_KHZ;
}

// max users scales with antennas (reuse factor)
// clamp antennas to 1..MAX_ANTENNAS
int FourGSimulator::maxUsersFor(int total_khz, int antennas) const {
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;
    int sub = channelsFor(total_khz);
    return sub * USERS_PER_10KHZ * antennas;
}

int FourGSimulator::messagesPerDevice(int deviceType) const {
    (void)deviceType;
    return MESSAGES_PER_DEVICE;
}

// list which device IDs fall into the FIRST 10kHz subchannel
// device allocation is sequential into buckets of size (USERS_PER_10KHZ * antennas)
int FourGSimulator::listFirstSubchannel(int* deviceIds,
                                        int deviceCount,
                                        int total_khz,
                                        int antennas,
                                        int* outFirst,
                                        int maxOut) const
{
    int sub = channelsFor(total_khz);
    if (sub <= 0) return 0;
    if (antennas < 1) antennas = 1;
    if (antennas > MAX_ANTENNAS) antennas = MAX_ANTENNAS;

    int cap = USERS_PER_10KHZ * antennas;
    int written = 0;

    for (int i = 0; i < deviceCount; ++i) {
        int subIndex = (i / cap) + 1; // 1-based subchannel index
        if (subIndex == 1) {
            if (written < maxOut) outFirst[written] = deviceIds[i];
            ++written;
        }
    }
    return written;
}
