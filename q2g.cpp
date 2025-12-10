#include "q2g.h"

// -----------------------------------------------------------
// Constructor (empty — nothing special needed for 2G)
// -----------------------------------------------------------
TwoGSimulator::TwoGSimulator()
{
    // left intentionally empty
}

// -----------------------------------------------------------
// Number of 200 kHz channels available
// -----------------------------------------------------------
int TwoGSimulator::channelsFor(int total_khz) const {
    if (total_khz <= 0) return 0;
    return total_khz / CHANNEL_KHZ;
}

// -----------------------------------------------------------
// Maximum users supported
// channels × 16 (no MIMO in 2G)
// -----------------------------------------------------------
int TwoGSimulator::maxUsersFor(int total_khz, int antennas) const {
    (void) antennas;   // 2G does not use antennas; keep for polymorphism
    int ch = channelsFor(total_khz);
    return ch * USERS_PER_CHANNEL;
}

// -----------------------------------------------------------
// Compute number of messages generated per device based on type
// -----------------------------------------------------------
int TwoGSimulator::messagesPerDevice(int deviceType) const {
    if (deviceType == DEVICE_VOICE) return VOICE_MSG;
    if (deviceType == DEVICE_DATA)  return DATA_MSG;
    return (VOICE_MSG + DATA_MSG) / 2;   // DEVICE_BOTH
}

// -----------------------------------------------------------
// List which device IDs fall into the FIRST 200 kHz channel
//
// deviceIds[]   → sequential allocation
// deviceCount   → number of devices
// total_khz     → must compute how many channels exist
// outFirst[]    → caller-provided array for output
// maxOut        → size of outFirst[]
//
// RETURNS NUMBER OF IDS WRITTEN
// -----------------------------------------------------------
int TwoGSimulator::listFirstChannel(int* deviceIds,
                                    int deviceCount,
                                    int total_khz,
                                    int* outFirst,
                                    int maxOut)
{
    int ch = channelsFor(total_khz);
    if (ch <= 0) return 0;

    int written = 0;

    // Users fill channels sequentially:
    // channel = (index / 16) + 1
    for (int i = 0; i < deviceCount; i++) {
        int channelIndex = (i / USERS_PER_CHANNEL) + 1;  // 1-based index

        if (channelIndex == 1) {
            if (written < maxOut) {
                outFirst[written] = deviceIds[i];
            }
            written++;
        }
    }

    return written;  // may be > maxOut; caller must check
}
