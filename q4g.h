#ifndef Q4G_H
#define Q4G_H

#include "q3g.h"

// --------------------------------------------------------
// 4G Simulator — inherits from 3G, overrides capacity rules
// --------------------------------------------------------
class FourGSimulator : public ThreeGSimulator {
public:
    // 4G design (as per rubric)
    static const int SUB_BAND_KHZ = 10;       // 10 kHz subchannels
    static const int USERS_PER_10KHZ = 30;    // 30 users per 10 kHz
    static const int MESSAGES_PER_DEVICE = 10;// per-device messages
    static const int MAX_ANTENNAS = 4;        // up to 4 antennas (MIMO reuse)

    FourGSimulator();

    // number of subchannels = total_khz / 10
    int channelsFor(int total_khz) const override;

    // max users = subchannels * USERS_PER_10KHZ * antennas (clamp antennas)
    int maxUsersFor(int total_khz, int antennas = 1) const override;

    // messages per device
    int messagesPerDevice(int deviceType) const override;

    // list occupants of first 10 kHz subchannel (considers antennas)
    int listFirstSubchannel(int* deviceIds,
                            int deviceCount,
                            int total_khz,
                            int antennas,
                            int* outFirst,
                            int maxOut) const;
};

#endif // Q4G_H
