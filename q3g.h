#ifndef Q3G_H
#define Q3G_H

#include "q2g.h"

class ThreeGSimulator : public TwoGSimulator {
public:
    static const int BAND_KHZ = 200; // channel size
    static const int USERS_PER_200KHZ = 32; // 32 users per 200 kHz channel
    static const int MESSAGES_PER_DEVICE = 10; // each device generates 10 messages

    ThreeGSimulator();

    int channelsFor(int total_khz) const;
    // antennas parameter retained for polymorphic API; ignored for 3G
    int maxUsersFor(int total_khz, int antennas = 1) const;

    // deviceIds: array of device ids assigned sequentially; deviceCount: how many devices present
    // total_khz: total allocation (e.g., 1000 for 1 MHz)
    // outFirst: buffer to receive ids on first channel (maxOut is buffer size)
    // returns number of IDs written into outFirst
    int listFirstChannel(int* deviceIds, int deviceCount, int total_khz, int* outFirst, int maxOut) const;
};

#endif // Q3G_H
