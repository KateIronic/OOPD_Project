#pragma once

// Abstract base class for all generation simulators
class Simulator {
public:
    virtual ~Simulator() {}

    // number of channels/subchannels depending on generation
    virtual int channelsFor(int total_khz) const = 0;

    // maximum users supported given total_khz and antennas (antennas default 1)
    virtual int maxUsersFor(int total_khz, int antennas = 1) const = 0;

    // optional messages per device (override if meaningful)
    virtual int messagesPerDevice() const { return 0; }
};
