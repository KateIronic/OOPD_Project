#pragma once
#ifndef Q2G_H
#define Q2G_H

#include "simulator.h"

class TwoGSimulator : public Simulator {
public:
    // 2G constants
    static const int CHANNEL_KHZ = 200;
    static const int USERS_PER_CHANNEL = 16;

    static const int VOICE_MSG = 15;
    static const int DATA_MSG  = 5;

    TwoGSimulator();

    int channelsFor(int total_khz) const override;

    int maxUsersFor(int total_khz, int antennas = 1) const override;

    int messagesPerDevice(int deviceType) const override;

    const char* name() const override { return "2G"; }

    // REQUIRED BY main: must exist in class
    int listFirstChannel(int* deviceIds,
                         int deviceCount,
                         int total_khz,
                         int* outFirst,
                         int maxOut);
};

#endif
