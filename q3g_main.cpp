#include "basicIO.h"
#include "q3g.h"

extern basicIO io;

int main() {
    try {
        io.activateInput();
        ThreeGSimulator sim;

        io.outputstring("3G Communication - Capacity Calculator\n");
        io.outputstring("Assuming default allocation 1 MHz (1000 kHz).\n");

        int total_khz = 1000;

        int channels = sim.channelsFor(total_khz);
        int maxUsers = sim.maxUsersFor(total_khz);

        io.outputstring("Channels of 200 kHz available: ");
        io.outputint(channels);
        io.terminate();

        io.outputstring("Maximum users supported (");
        io.outputint(ThreeGSimulator::USERS_PER_3G_CHANNEL);
        io.outputstring(" per 200kHz): ");
        io.outputint(maxUsers);
        io.terminate();

        // -----------------------------
        // Read users
        // -----------------------------
        io.errorstring("\nEnter number of devices (<= max users): ");
        int n = io.inputint();

        if (n == IO_EOF) return 1;
        if (n < 0) n = 0;
        if (n > maxUsers) n = maxUsers;

        int* devices = new int[n];

        for (int i = 0; i < n; i++) {
            io.errorstring("Enter device ID for device ");
            io.errorint(i+1);
            io.errorstring(": ");

            int id = io.inputint();
            if (id == IO_EOF) return 1;

            devices[i] = id;
        }

        // -----------------------------
        // Print channel assignment
        // -----------------------------
        io.outputstring("\nPer-device channel assignment:\n");

        for (int i = 0; i < n; i++) {
            int chIndex = (i / ThreeGSimulator::USERS_PER_3G_CHANNEL) + 1;

            io.outputstring(" Device ");
            io.outputint(i+1);
            io.outputstring(": ID=");
            io.outputint(devices[i]);
            io.outputstring(", channel=");
            io.outputint(chIndex);
            io.terminate();
        }

        // -----------------------------
        // First channel occupants
        // -----------------------------
        int* first = new int[ThreeGSimulator::USERS_PER_3G_CHANNEL];
        int countFirst = sim.listFirstChannel(
            devices, n, total_khz,
            first, ThreeGSimulator::USERS_PER_3G_CHANNEL
        );

        io.outputstring("\nUsers in first 200 kHz channel: ");

        int shown = (countFirst < ThreeGSimulator::USERS_PER_3G_CHANNEL)
                  ? countFirst
                  : ThreeGSimulator::USERS_PER_3G_CHANNEL;

        if (shown == 0) {
            io.outputstring("(none)\n");
        } else {
            for (int i = 0; i < shown; i++) {
                io.outputint(first[i]);
                io.outputstring(" ");
            }
            io.terminate();
        }

        // -----------------------------
        // messages per device
        // -----------------------------
        io.outputstring("\nEach 3G device generates ");
        io.outputint(ThreeGSimulator::MSG_PER_DEVICE_3G);
        io.outputstring(" messages.\n");

        delete [] devices;
        delete [] first;

    } catch (...) {
        io.errorstring("Unknown error\n");
        return 2;
    }

    return 0;
}
