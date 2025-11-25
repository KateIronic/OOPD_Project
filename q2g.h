#pragma once
#include "basicIO.h"

class TwoGSimulator {
public:
    static const int USERS_PER_200KHZ = 16;
    static const int BAND_KHZ = 200;
    static const int DATA_MSG = 5;
    static const int VOICE_MSG = 15;

    TwoGSimulator();

    // compute number of 200 kHz channels available in `total_khz`
    int channelsFor(int total_khz) const;

    // compute maximum users supported given total_khz
    int maxUsersFor(int total_khz) const;

    // assign device ids (array) to channels; returns number assigned to first channel
    int listFirstChannel(int* deviceIds, int deviceCount, int total_khz, int* outFirst, int maxOut);
};
#pragma once

// 2G calculation helper
// No C++ standard library containers used; simple arrays and ints only.

class TwoGNetwork {
public:
    // totalBandwidthKHz: total allocated bandwidth in kHz (e.g., 1000 for 1 MHz)
    // channelKHz: 200 for 2G per-channel width
    // usersPerChannel: maximum users per channel (16 for 2G TDM)
    TwoGNetwork(int totalBandwidthKHz, int channelKHz = 200, int usersPerChannel = 16);

    int totalChannels() const;
    int maxUsers() const;

    // Given a total number of user IDs (1..numUsers), return how many users occupy the first channel
    // The caller provides an output buffer `out` with capacity `outCap`; the function fills it with user ids
    // and returns the number of ids written.
    int usersInFirstChannel(int numUsers, int* out, int outCap) const;

private:
    int totalBandwidthKHz;
    int channelKHz;
    int usersPerChannel;
    int channels;
};
