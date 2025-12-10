#include "basicIO.h"
#include "q5g.h"

extern basicIO io;

int main() {
    try {
        io.activateInput();
        FiveGSimulator sim;

        io.outputstring("5G Communication - Capacity Calculator\n");
        io.outputstring("Main band assumed allocation 1 MHz (1000 kHz) using 10 kHz subchannels.\n");
        io.outputstring("Additionally, an extra 10 MHz high band (e.g., 1800 MHz region) is present.\n");

        int main_total_khz = 1000; // 1 MHz main band

        int mainSub = sim.mainSubchannelsFor(main_total_khz);
        io.outputstring("Main subchannels (10 kHz) available: ");
        io.outputint(mainSub);
        io.terminate();

        io.outputstring("High band (MHz): ");
        io.outputint(sim.highBandMHz());
        io.outputstring(" (total kHz = ");
        io.outputint(FiveGSimulator::HIGH_BAND_KHZ);
        io.outputstring(")\n");
        io.outputstring("High band users per 1 MHz (base): ");
        io.outputint(FiveGSimulator::USERS_PER_1MHZ_HIGH);
        io.terminate();

        io.errorstring("\nEnter number of antennas to use (1 - 16, default 1): ");
        int antennas = io.inputint();
        if (antennas == IO_EOF) { io.errorstring("No input provided. Exiting.\n"); return 1; }
        if (antennas < 1) antennas = 1;
        if (antennas > FiveGSimulator::MAX_ANTENNAS) antennas = FiveGSimulator::MAX_ANTENNAS;

        int mainCap = sim.maxUsersMain(main_total_khz, antennas);
        int highCap = sim.maxUsersHigh(antennas);
        int totalCap = mainCap + highCap;

        io.outputstring("\nCalculations:\n");
        io.outputstring(" Main band capacity = main_subchannels * (users_per_10kHz_main * antennas) = ");
        io.outputint(mainSub); io.outputstring(" * ("); io.outputint(FiveGSimulator::USERS_PER_10KHZ_MAIN); io.outputstring(" * "); io.outputint(antennas); io.outputstring(") = "); io.outputint(mainCap); io.terminate();

        io.outputstring(" High band capacity = high_band_MHz * (users_per_1MHz_high * antennas) = ");
        io.outputint(sim.highBandMHz()); io.outputstring(" * ("); io.outputint(FiveGSimulator::USERS_PER_1MHZ_HIGH); io.outputstring(" * "); io.outputint(antennas); io.outputstring(") = "); io.outputint(highCap); io.terminate();

        io.outputstring(" Total combined capacity = main + high = ");
        io.outputint(mainCap); io.outputstring(" + "); io.outputint(highCap); io.outputstring(" = "); io.outputint(totalCap); io.terminate();

        io.errorstring("\nNow you may simulate assignment: enter number of devices present (<= total capacity): ");
        int n = io.inputint();
        if (n == IO_EOF) { io.errorstring("No input provided. Exiting.\n"); return 1; }
        if (n < 0) n = 0;
        if (n > totalCap) {
            io.outputstring("Note: provided device count exceeds theoretical total capacity; only first ");
            io.outputint(totalCap); io.outputstring(" will be considered.\n");
            n = totalCap;
        }

        int* devices = new int[n];
        for (int i = 0; i < n; ++i) {
            io.errorstring("Enter device id for device ");
            io.errorint(i+1);
            io.errorstring(": ");
            int id = io.inputint();
            if (id == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); delete [] devices; return 1; }
            devices[i] = id;
        }

        io.outputstring("\nAssignment policy and steps:\n");
        io.outputstring(" 1) Fill main band subchannels sequentially using buckets of size (users_per_10kHz_main * antennas).\n");
        io.outputstring(" 2) If main band is full, assign remaining users into high band 1 MHz chunks, each chunk capacity = users_per_1MHz_high * antennas.\n");

        int perMainCap = FiveGSimulator::USERS_PER_10KHZ_MAIN * antennas;
        int perHighCap = FiveGSimulator::USERS_PER_1MHZ_HIGH * antennas;

        for (int i = 0; i < n; ++i) {
            int band = 0; // 1=main, 2=high
            int indexInBand = 0;
            if (i < mainCap) {
                band = 1;
                indexInBand = (i / perMainCap) + 1; // subchannel index
            } else {
                band = 2;
                int offset = i - mainCap;
                indexInBand = (offset / perHighCap) + 1; // 1MHz chunk index
            }
            io.outputstring(" Device ");
            io.outputint(i+1);
            io.outputstring(": ID=");
            io.outputint(devices[i]);
            if (band == 1) {
                io.outputstring(", assigned to MAIN band subchannel=");
                io.outputint(indexInBand);
                io.outputstring(", slot=");
                io.outputint((i % perMainCap) + 1);
            } else {
                io.outputstring(", assigned to HIGH 1MHz chunk(MHz)=");
                io.outputint(indexInBand);
                io.outputstring(", slot=");
                io.outputint(((i - mainCap) % perHighCap) + 1);
            }
            io.terminate();
        }

        // list first main subchannel occupants
        int* firstMain = new int[perMainCap];
        int countFirstMain = sim.listFirstMainSubchannel(devices, n, main_total_khz, antennas, firstMain, perMainCap);
        io.outputstring("\nFirst MAIN subchannel occupants (cap="); io.outputint(perMainCap); io.outputstring("):");
        for (int i = 0; i < (countFirstMain < perMainCap ? countFirstMain : perMainCap); ++i) { io.outputstring(" "); io.outputint(firstMain[i]); }
        io.terminate();

        // list first high chunk occupants
        int* firstHigh = new int[perHighCap];
        int countFirstHigh = sim.listFirstHighChunk(devices, n, main_total_khz, antennas, firstHigh, perHighCap);
        io.outputstring("First HIGH 1MHz chunk occupants (cap="); io.outputint(perHighCap); io.outputstring("):");
        for (int i = 0; i < (countFirstHigh < perHighCap ? countFirstHigh : perHighCap); ++i) { io.outputstring(" "); io.outputint(firstHigh[i]); }
        io.terminate();

        // messages and cores
        long totalMessages = (long)n * FiveGSimulator::MESSAGES_PER_DEVICE;
        io.outputstring("\nTotal messages required = n * messages_per_device = ");
        io.outputint(n); io.outputstring(" * "); io.outputint(FiveGSimulator::MESSAGES_PER_DEVICE);
        io.outputstring(" = "); io.outputint((int)totalMessages); io.terminate();

        io.errorstring("Enter cellular core message capacity (total messages a single core can handle): ");
        int coreCap = io.inputint();
        if (coreCap == IO_EOF) { io.errorstring("No input provided. Exiting.\n"); delete [] devices; delete [] firstMain; delete [] firstHigh; return 1; }
        if (coreCap <= 0) coreCap = 1;
        int coresNeeded = (int)((totalMessages + coreCap - 1) / coreCap);
        io.outputstring("Cores needed = ceil(totalMessages / coreCap) = ceil(");
        io.outputint((int)totalMessages); io.outputstring(" / "); io.outputint(coreCap);
        io.outputstring(") = "); io.outputint(coresNeeded); io.terminate();

        delete [] devices;
        delete [] firstMain;
        delete [] firstHigh;

    } catch (const char* e) {
        io.errorstring("Error: "); io.errorstring(e); io.terminate();
        return 1;
    } catch (...) {
        io.errorstring("Unknown error\n");
        return 2;
    }

    return 0;
}
