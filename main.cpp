#include "basicIO.h"
#include <thread>

// Simple cellular network simulator for Question 1
// Uses only the provided `basicIO` for input/output.

// Minimal template utility (to satisfy assignment requirement)
template<typename T>
T minT(T a, T b) {
    return a < b ? a : b;
}

class Device {
public:
    int id;
    int type; // 1 = voice, 2 = data, 3 = both
    int messages; // messages generated per connection
    int frequencyIndex; // 1-based frequency assigned

    Device(int i, int t, int m) : id(i), type(t), messages(m), frequencyIndex(-1) {}
};

class CellTower {
public:
    int frequencies; // number of frequency channels
    int maxUsersPerFrequency; // capacity per frequency

    CellTower(int freqs, int per) : frequencies(freqs), maxUsersPerFrequency(per) {}

    int capacity() const {
        return frequencies * maxUsersPerFrequency;
    }

    // assign devices round-robin to frequencies (1..frequencies)
    void assignDevices(Device** devices, int n) const {
        if (frequencies <= 0) return;
        for (int i = 0; i < n; ++i) {
            devices[i]->frequencyIndex = (i % frequencies) + 1;
        }
    }
};

class CellularCore {
public:
    int overheadPer100; // overhead messages per 100 messages
    int capacityMessages; // core's total message handling capacity

    CellularCore(int overhead, int capacity) : overheadPer100(overhead), capacityMessages(capacity) {}

    // compute effective messages after overhead
    int effectiveMessages(int messages) const {
        int extra = (messages / 100) * overheadPer100;
        return messages + extra;
    }

    // return how many devices (starting from index 0) can be supported
    int maxDevicesSupported(Device** devices, int n) const {
        int totalRaw = 0;
        for (int i = 0; i < n; ++i) {
            // add device messages
            totalRaw += devices[i]->messages;
            int eff = effectiveMessages(totalRaw);
            if (eff > capacityMessages) {
                return i; // previous devices only
            }
        }
        return n; // all supported
    }
};

// global object from basicIO
extern basicIO io;

int main() {
    try {
        io.activateInput();

        io.outputstring("Question 1: Cellular Simulation Framework\n");

        io.errorstring("Enter number of devices to simulate: ");
        int n = io.inputint();
        if (n == IO_EOF) {
            io.errorstring("No input provided. Exiting.\n");
            return 1;
        }
        if (n <= 0) throw "Number of devices must be positive";

        // Allocate devices
        Device** devices = new Device*[n];

        for (int i = 0; i < n; ++i) {
            io.errorstring("\nDevice "); io.errorint(i+1); io.errorstring(" details:\n");

            io.errorstring(" Enter device id (int): ");
            int id = io.inputint();
            if (id == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); return 1; }

            io.errorstring(" Enter type (1=voice,2=data,3=both): ");
            int type = io.inputint();
            if (type == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); return 1; }
            if (type < 1 || type > 3) type = 2;

            io.errorstring(" Enter messages generated per connection (int): ");
            int msgs = io.inputint();
            if (msgs == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); return 1; }
            if (msgs < 0) msgs = 0;

            devices[i] = new Device(id, type, msgs);
        }

        io.errorstring("\nEnter number of frequency channels in a single cell tower: ");
        int freq = io.inputint();
        if (freq == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); return 1; }
        if (freq <= 0) throw "Frequencies must be positive";

        io.errorstring("Enter max users allowed per frequency (tower policy): ");
        int perFreq = io.inputint();
        if (perFreq == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); return 1; }
        if (perFreq <= 0) throw "Users per frequency must be positive";

        CellTower tower(freq, perFreq);
        tower.assignDevices(devices, n);

        io.errorstring("\nEnter core overhead per 100 messages (int): ");
        int overhead = io.inputint();
        if (overhead == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); return 1; }
        if (overhead < 0) overhead = 0;

        io.errorstring("Enter cellular core message capacity (total messages it can handle): ");
        int coreCap = io.inputint();
        if (coreCap == IO_EOF) { io.errorstring("Unexpected EOF. Exiting.\n"); return 1; }
        if (coreCap < 0) coreCap = 0;

        CellularCore core(overhead, coreCap);

        // compute totals
        int totalRaw = 0;
        for (int i = 0; i < n; ++i) totalRaw += devices[i]->messages;
        int totalEffective = core.effectiveMessages(totalRaw);

        int supportedByCore = core.maxDevicesSupported(devices, n);
        int supportedByTower = minT(tower.capacity(), n);
        int finalSupported = minT(supportedByCore, supportedByTower);

        io.outputstring("\n--- Simulation Results ---\n");
        io.outputstring("Total raw messages generated by all devices: "); io.outputint(totalRaw); io.terminate();
        io.outputstring("Total effective messages after core overhead: "); io.outputint(totalEffective); io.terminate();
        io.outputstring("Cell tower total capacity (users): "); io.outputint(tower.capacity()); io.terminate();
        io.outputstring("Number of devices supported by core (in insertion order): "); io.outputint(supportedByCore); io.terminate();
        io.outputstring("Number of devices accommodated by tower (max allowed by frequencies): "); io.outputint(supportedByTower); io.terminate();
        io.outputstring("Final number of devices that can be active simultaneously: "); io.outputint(finalSupported); io.terminate();

        io.outputstring("\nPer-device assignment and status:\n");
        for (int i = 0; i < n; ++i) {
            io.outputstring(" Device "); io.outputint(i+1); io.outputstring(": ID="); io.outputint(devices[i]->id);
            io.outputstring(", type="); io.outputint(devices[i]->type);
            io.outputstring(", msgs="); io.outputint(devices[i]->messages);
            io.outputstring(", freq="); io.outputint(devices[i]->frequencyIndex);
            // determine status: supported if index < finalSupported (insertion order) and within tower capacity per frequency
            bool supported = (i < finalSupported);
            io.outputstring(", status="); io.outputstring(supported ? "ACTIVE" : "DROPPED"); io.terminate();
        }

        io.outputstring("\nList users per frequency:\n");
        for (int f = 1; f <= freq; ++f) {
            io.outputstring(" Frequency "); io.outputint(f); io.outputstring(": ");
            int shown = 0;
            for (int i = 0; i < n; ++i) {
                if (devices[i]->frequencyIndex == f) {
                    if (shown < perFreq) {
                        io.outputint(devices[i]->id); io.outputstring(" ");
                        ++shown;
                    }
                }
            }
            if (shown == 0) io.outputstring("(none)");
            io.terminate();
        }

        io.outputstring("\nTo inspect a specific frequency index, run program interactively and enter index when prompted.\n");

        // cleanup
        for (int i = 0; i < n; ++i) delete devices[i];
        delete [] devices;

    } catch (const char* e) {
        io.errorstring("Error: "); io.errorstring(e); io.terminate();
        return 1;
    } catch (...) {
        io.errorstring("Unknown error occurred\n");
        return 2;
    }

    return 0;
}
