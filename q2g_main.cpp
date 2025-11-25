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

        io.outputstring("Channels of 200 kHz available: "); io.outputint(channels); io.terminate();
        io.outputstring("Maximum users supported (" ); io.outputint(TwoGSimulator::USERS_PER_200KHZ); io.outputstring(" per 200kHz): "); io.outputint(maxUsers); io.terminate();

        io.outputstring("\nNow you may simulate assignment: enter number of devices present (<= max users): ");
        int n = io.inputint();
        if (n < 0) n = 0;
        if (n > maxUsers) {
            io.outputstring("Note: provided device count exceeds theoretical max; only first "); io.outputint(maxUsers); io.outputstring(" will be considered.\n");
            n = maxUsers;
        }

        // allocate devices array
        int* devices = new int[n];
        for (int i = 0; i < n; ++i) {
            io.outputstring("Enter device id for device "); io.outputint(i+1); io.outputstring(": ");
            int id = io.inputint();
            devices[i] = id;
        }

        // collect first channel occupants
        int* first = new int[TwoGSimulator::USERS_PER_200KHZ];
        int countFirst = sim.listFirstChannel(devices, n, total_khz, first, TwoGSimulator::USERS_PER_200KHZ);

        io.outputstring("\nUsers occupying the first 200 kHz channel (first channel): ");
        int shown = (countFirst < TwoGSimulator::USERS_PER_200KHZ) ? countFirst : TwoGSimulator::USERS_PER_200KHZ;
        if (shown == 0) {
            io.outputstring("(none)\n");
        } else {
            for (int i = 0; i < shown; ++i) {
                io.outputint(first[i]); io.outputstring(" ");
            }
            io.terminate();
        }

        delete [] devices;
        delete [] first;

    } catch (const char* e) {
        io.errorstring("Error: "); io.errorstring(e); io.terminate();
        return 1;
    } catch (...) {
        io.errorstring("Unknown error\n");
        return 2;
    }

    return 0;
}
