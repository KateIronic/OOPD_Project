#include "q2g.h"

TwoGSimulator::TwoGSimulator() {
}

int TwoGSimulator::channelsFor(int total_khz) const {
    if (total_khz <= 0) return 0;
    return total_khz / BAND_KHZ;
}

int TwoGSimulator::maxUsersFor(int total_khz, int antennas) const {
    (void)antennas; // 2G does not use MIMO antennas; parameter kept for polymorphic API
    int ch = channelsFor(total_khz);
    return ch * USERS_PER_200KHZ;
}

// deviceIds: array of device ids assigned sequentially; deviceCount: how many devices present
// total_khz: total allocation (e.g., 1000 for 1 MHz)
// outFirst: buffer to receive ids on first channel (maxOut is buffer size)
// returns number of IDs written into outFirst
int TwoGSimulator::listFirstChannel(int* deviceIds, int deviceCount, int total_khz, int* outFirst, int maxOut) {
    int ch = channelsFor(total_khz);
    if (ch <= 0) return 0;
    int cap = USERS_PER_200KHZ; // users per channel
    int written = 0;
    // devices assigned sequentially into channel buckets of size cap
    for (int i = 0; i < deviceCount; ++i) {
        int channelIndex = (i / cap) + 1; // 1-based channel
        if (channelIndex == 1) {
            if (written < maxOut) outFirst[written] = deviceIds[i];
            ++written;
        }
    }
    // written may be greater than maxOut; caller should check
    return written;
}
#include "q2g.h"

TwoGNetwork::TwoGNetwork(int totalBandwidthKHz_, int channelKHz_, int usersPerChannel_)
    : totalBandwidthKHz(totalBandwidthKHz_), channelKHz(channelKHz_), usersPerChannel(usersPerChannel_) {
    if (channelKHz <= 0) channelKHz = 200;
    if (usersPerChannel <= 0) usersPerChannel = 16;
    channels = totalBandwidthKHz / channelKHz;
}

int TwoGNetwork::totalChannels() const {
    return channels;
}

int TwoGNetwork::maxUsers() const {
    return channels * usersPerChannel;
}

int TwoGNetwork::usersInFirstChannel(int numUsers, int* out, int outCap) const {
    if (!out || outCap <= 0) return 0;
    int toTake = usersPerChannel;
    if (toTake > outCap) toTake = outCap;
    int written = 0;
    // Users are assigned sequentially to channels; first channel holds user ids 1..usersPerChannel
    for (int id = 1; id <= numUsers && written < toTake; ++id) {
        // user assigned to channel = ((id-1) % channels) + 1
        int ch = channels > 0 ? ((id - 1) % channels) + 1 : 1;
        if (ch == 1) {
            out[written++] = id;
        }
    }
    return written;
}
