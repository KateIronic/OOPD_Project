#pragma once
#ifndef SIMULATOR_H
#define SIMULATOR_H

// Device type constants: used across all generations (2G–5G)
#define DEVICE_VOICE 1
#define DEVICE_DATA  2
#define DEVICE_BOTH  3

// ------------------------------------------------------------------
// Abstract Simulator: defines the API that every generation must follow
// ------------------------------------------------------------------
class Simulator {
public:
    virtual ~Simulator() {}

    // Number of channels for this generation given total kHz bandwidth
    virtual int channelsFor(int total_khz) const = 0;

    // Maximum users supported given total bandwidth and antenna count
    // (2G ignores antennas, 3G+ use antennas)
    virtual int maxUsersFor(int total_khz, int antennas = 1) const = 0;

    // Messages generated per device depending on device type
    virtual int messagesPerDevice(int deviceType) const = 0;

    // Return name of generation ("2G", "3G", "4G", "5G")
    virtual const char* name() const = 0;
};

#endif
