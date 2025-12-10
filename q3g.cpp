#include "q3g.h"

// ---------------------------------------------
ThreeGSimulator::ThreeGSimulator() {}

// ---------------------------------------------
// maxUsersFor → same channels but 3G capacity = 32 per channel
// ---------------------------------------------
int ThreeGSimulator::maxUsersFor(int total_khz, int antennas) const {
    (void) antennas; // 3G does not use MIMO
    int ch = channelsFor(total_khz);
    return ch * USERS_PER_3G_CHANNEL;
}

// ---------------------------------------------
// messagesPerDevice → every 3G device sends 10 messages
// ---------------------------------------------
int ThreeGSimulator::messagesPerDevice(int deviceType) const {
    (void) deviceType; // 3G uses packet-switched → fixed
    return MSG_PER_DEVICE_3G;
}

// ---------------------------------------------
// listFirstChannel (similar to 2G but uses new capacity)
// ---------------------------------------------
int ThreeGSimulator::listFirstChannel(int* deviceIds,
                                      int deviceCount,
                                      int total_khz,
                                      int* outFirst,
                                      int maxOut) const
{
    int ch = channelsFor(total_khz);
    if (ch <= 0) return 0;

    int written = 0;

    // channel index = (i / 32) + 1
    for (int i = 0; i < deviceCount; i++) {
        int channelIndex = (i / USERS_PER_3G_CHANNEL) + 1;

        if (channelIndex == 1) {
            if (written < maxOut) {
                outFirst[written] = deviceIds[i];
            }
            written++;
        }
    }

    return written;
}
