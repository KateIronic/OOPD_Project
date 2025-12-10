#ifndef Q3G_H
#define Q3G_H

#include "q2g.h"

// --------------------------------------------------------
// 3G Simulator — Inherits from 2G but increases capacity
// --------------------------------------------------------
class ThreeGSimulator : public TwoGSimulator {
public:
    static const int USERS_PER_3G_CHANNEL = 32;   // 32 users per 200 kHz
    static const int MSG_PER_DEVICE_3G   = 10;    // packet-switched fixed

    ThreeGSimulator();

    // override channel capacity
    int maxUsersFor(int total_khz, int antennas = 1) const override;

    // return messages per device
    int messagesPerDevice(int deviceType) const override;

    // list IDs in first channel
    int listFirstChannel(int* deviceIds,
                         int deviceCount,
                         int total_khz,
                         int* outFirst,
                         int maxOut) const;
};

#endif
