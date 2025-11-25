#ifndef Q4G_H
#define Q4G_H

#include "q3g.h"

class FourGSimulator : public ThreeGSimulator {
public:
    static const int SUB_BAND_KHZ = 10; // 10 kHz subchannels
    static const int USERS_PER_10KHZ = 30; // 30 users per 10 kHz
    static const int MESSAGES_PER_DEVICE = 10; // 10 messages per device
    static const int MAX_ANTENNAS = 4; // up to 4 antennas (MIMO reuse)

    FourGSimulator();

    int channelsFor(int total_khz) const;
    // max users with given antennas (1..MAX_ANTENNAS)
    int maxUsersFor(int total_khz, int antennas) const;

    // list users occupying the first subchannel (10 kHz) when devices are assigned sequentially
    int listFirstSubchannel(int* deviceIds, int deviceCount, int total_khz, int antennas, int* outFirst, int maxOut) const;
};

#endif // Q4G_H
