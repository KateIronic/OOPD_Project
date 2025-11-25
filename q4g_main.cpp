#include "basicIO.h"
#include "q4g.h"

extern basicIO io;

int main() {
    try {
        io.activateInput();
        FourGSimulator sim;

        io.outputstring("4G Communication - Capacity Calculator\n");
        io.outputstring("Assuming default allocation 1 MHz (1000 kHz).\n");
        int total_khz = 1000;
        int channels = sim.channelsFor(total_khz);

        io.outputstring("Subchannels of 10 kHz available: "); io.outputint(channels);
        io.outputstring("  (calculation: total_khz / 10 = "); io.outputint(total_khz); io.outputstring(" / 10 )"); io.terminate();

        io.outputstring("Users per 10 kHz (base): "); io.outputint(FourGSimulator::USERS_PER_10KHZ);
        io.outputstring("  (each 10 kHz subchannel supports "); io.outputint(FourGSimulator::USERS_PER_10KHZ); io.outputstring(" users)"); io.terminate();

        io.outputstring("Max antennas supported (MIMO): "); io.outputint(FourGSimulator::MAX_ANTENNAS);
        io.outputstring("  (antennas allow reuse of same subchannels up to this factor)"); io.terminate();

        io.errorstring("\nEnter number of antennas to use (1-4, default 1): ");
        int antennas = io.inputint();
        if (antennas == IO_EOF) { io.errorstring("No input provided. Exiting.\n"); return 1; }
        if (antennas < 1) antennas = 1;
        if (antennas > FourGSimulator::MAX_ANTENNAS) antennas = FourGSimulator::MAX_ANTENNAS;

        // explain how antennas affect capacity
        int perSubEffective = FourGSimulator::USERS_PER_10KHZ * antennas;
        io.outputstring("\nCalculating effective capacity:\n");
        io.outputstring(" Effective users per 10 kHz subchannel = users_per_10kHz * antennas = "); io.outputint(FourGSimulator::USERS_PER_10KHZ); io.outputstring(" * "); io.outputint(antennas); io.outputstring(" = "); io.outputint(perSubEffective); io.terminate();

        int maxUsers = sim.maxUsersFor(total_khz, antennas);
        io.outputstring(" Total effective max users = subchannels * effective_per_subchannel = "); io.outputint(channels); io.outputstring(" * "); io.outputint(perSubEffective); io.outputstring(" = "); io.outputint(maxUsers); io.terminate();

        io.errorstring("\nNow you may simulate assignment: enter number of devices present (<= max users): ");
        int n = io.inputint();
        if (n == IO_EOF) { io.errorstring("No input provided. Exiting.\n"); return 1; }
        if (n < 0) n = 0;
        if (n > maxUsers) {
            io.outputstring("Note: provided device count exceeds theoretical max; only first "); io.outputint(maxUsers); io.outputstring(" will be considered.\n");
            n = maxUsers;
        }

        io.outputstring("\n(Assignment policy) Devices are assigned sequentially into subchannel buckets of size effective_per_subchannel.\n");
        io.outputstring(" subchannel index = floor((device_index-1) / effective_per_subchannel) + 1\n");

        int* devices = new int[n];
        for (int i = 0; i < n; ++i) {
            io.errorstring("Enter device id for device "); io.errorint(i+1); io.errorstring(": ");
            int id = io.inputint();
            if (id == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); delete [] devices; return 1; }
            devices[i] = id;
        }

        io.outputstring("\nPer-device subchannel assignment (10 kHz units):\n");
        int perSubCap = perSubEffective;
        for (int i = 0; i < n; ++i) {
            int channelIndex = (i / perSubCap) + 1;
            io.outputstring(" Device "); io.outputint(i+1); io.outputstring(": ID="); io.outputint(devices[i]); io.outputstring(", formula_subchannel=floor(("); io.outputint(i+1); io.outputstring("-1)/"); io.outputint(perSubCap); io.outputstring(")+1="); io.outputint(channelIndex);
            io.outputstring(", capacity_slot_index="); io.outputint(((i % perSubCap) + 1)); io.terminate();
        }

        int* first = new int[FourGSimulator::USERS_PER_10KHZ * FourGSimulator::MAX_ANTENNAS];
        int countFirst = sim.listFirstSubchannel(devices, n, total_khz, antennas, first, FourGSimulator::USERS_PER_10KHZ * FourGSimulator::MAX_ANTENNAS);

        io.outputstring("\nUsers occupying the first 10 kHz subchannel (effective cap considering antennas): ");
        int capFirst = perSubEffective;
        io.outputstring("(effective cap = "); io.outputint(FourGSimulator::USERS_PER_10KHZ); io.outputstring(" * "); io.outputint(antennas); io.outputstring(" = "); io.outputint(capFirst); io.outputstring(")\n");
        int shown = (countFirst < capFirst) ? countFirst : capFirst;
        if (shown == 0) {
            io.outputstring(" (none)\n");
        } else {
            io.outputstring(" Occupants (count="); io.outputint(shown); io.outputstring("):");
            for (int i = 0; i < shown; ++i) {
                io.outputstring(" "); io.outputint(first[i]);
            }
            io.terminate();
        }

        // show message totals and cores needed
        long totalMessages = (long)n * FourGSimulator::MESSAGES_PER_DEVICE;
        io.outputstring("\nTotal messages required (n * messages_per_device): "); io.outputint(n); io.outputstring(" * "); io.outputint(FourGSimulator::MESSAGES_PER_DEVICE); io.outputstring(" = "); io.outputint((int)totalMessages); io.terminate();

        io.errorstring("Enter cellular core message capacity (total messages a single core can handle): ");
        int coreCap = io.inputint();
        if (coreCap == IO_EOF) { io.errorstring("No input provided. Exiting.\n"); delete [] devices; delete [] first; return 1; }
        if (coreCap <= 0) coreCap = 1;

        int coresNeeded = (int)((totalMessages + coreCap - 1) / coreCap);
        io.outputstring("Cores needed = ceil(totalMessages / coreCap) = ceil("); io.outputint((int)totalMessages); io.outputstring(" / "); io.outputint(coreCap); io.outputstring(") = "); io.outputint(coresNeeded); io.terminate();

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
