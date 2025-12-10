#include "basicIO.h"
#include "q2g.h"

extern basicIO io;

int main() {
    try {
        io.activateInput();
        TwoGSimulator sim;

        io.outputstring("2G Communication - Capacity Calculator\n");

        io.outputstring("Assuming default allocation 1 MHz (1000 kHz).\n");
        int total_khz = 1000;
        int channels = sim.channelsFor(total_khz);
        int maxUsers = sim.maxUsersFor(total_khz);

        io.outputstring("Channels of 200 kHz available: ");
        io.outputint(channels);
        io.terminate();

        io.outputstring("Maximum users supported (");
        io.outputint(TwoGSimulator::USERS_PER_CHANNEL);
        io.outputstring(" per 200kHz): ");
        io.outputint(maxUsers);
        io.terminate();

        io.errorstring("\nNow you may simulate assignment: enter number of devices present (<= max users): ");
        int n = io.inputint();
        if (n == IO_EOF) { io.errorstring("No input provided. Exiting.\n"); return 1; }
        if (n < 0) n = 0;
        if (n > maxUsers) {
            io.outputstring("Note: provided device count exceeds theoretical max; only first ");
            io.outputint(maxUsers);
            io.outputstring(" will be considered.\n");
            n = maxUsers;
        }

        // allocate devices array
        int* devices = new int[n];
        for (int i = 0; i < n; ++i) {
            io.errorstring("Enter device id for device ");
            io.errorint(i+1);
            io.errorstring(": ");
            int id = io.inputint();
            if (id == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); return 1; }
            devices[i] = id;
        }

        // Show per-device assignment to channels (sequential filling)
        io.outputstring("\nPer-device channel assignment:\n");
        for (int i = 0; i < n; ++i) {
            int channelIndex = (i / TwoGSimulator::USERS_PER_CHANNEL) + 1; // 1-based
            io.outputstring(" Device ");
            io.outputint(i+1);
            io.outputstring(": ID=");
            io.outputint(devices[i]);
            io.outputstring(", channel=");
            io.outputint(channelIndex);
            io.terminate();
        }

        // list occupants of first channel
        int* first = new int[TwoGSimulator::USERS_PER_CHANNEL];
        int countFirst = sim.listFirstChannel(
            devices,
            n,
            total_khz,
            first,
            TwoGSimulator::USERS_PER_CHANNEL
        );

        io.outputstring("\nUsers occupying the first 200 kHz channel (first channel): ");
        int shown = (countFirst < TwoGSimulator::USERS_PER_CHANNEL)
                    ? countFirst
                    : TwoGSimulator::USERS_PER_CHANNEL;

        if (shown == 0) {
            io.outputstring("(none)\n");
        } else {
            for (int i = 0; i < shown; ++i) {
                io.outputint(first[i]);
                io.outputstring(" ");
            }
            io.terminate();
        }

        delete [] devices;
        delete [] first;

    } catch (const char* e) {
        io.errorstring("Error: ");
        io.errorstring(e);
        io.terminate();
        return 1;
    } catch (...) {
        io.errorstring("Unknown error\n");
        return 2;
    }

    return 0;
}
