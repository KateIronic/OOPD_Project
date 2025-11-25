# OOPD_Project — Cellular Network Simulators (Q1–Q5)

**Project Summary:**
- This repository implements a set of small simulators for Questions 1–5 of the assignment. Each simulator models a cellular technology (Q1 basic core/tower model, Q2 2G TDMA, Q3 3G CDMA, Q4 4G OFDM/MIMO, Q5 5G with additional high-band). The simulators are intentionally minimal and use a small custom I/O layer (`basicIO`) that performs low-level syscalls.

**Files of Interest:**
- `basicIO.h`, `basicIO.cpp` — minimal I/O wrapper used by all programs (reads/writes via a syscall wrapper).
- `syscall.S` — tiny assembly wrapper for issuing syscalls from C++.
- `main.cpp` — Question 1 simulator (core + cell tower simulation).
- `q2g.*` — Question 2 (2G) implementation and runner (`q2g.h`, `q2g.cpp`, `q2g_main.cpp`).
- `q3g.*` — Question 3 (3G) implementation and runner.
- `q4g.*` — Question 4 (4G) implementation and runner.
- `q5g.*` — Question 5 (5G) implementation and runner.
- Test inputs: `test.txt`, `q2g_test.txt`, `q3g_test.txt`, `q4g_test.txt`, `q5g_test.txt` — example inputs used by the demos.
- `Makefile` — build targets, run targets and an interactive `run-menu` that lets you choose which simulator to run and whether to use file or interactive input.

**Environment / Assumptions**
- Linux x86-64 (dev container). Requires `g++` (GNU C++ compiler) and a standard build toolchain.
- Programs are compiled with `g++` and link the small `syscall.S` assembly helper.

Build
-----
Build everything with:
```
make
```
This compiles the following binaries:
- `sim`    (Q1)
- `sim2g`  (Q2)
- `sim3g`  (Q3)
- `sim4g`  (Q4)
- `sim5g`  (Q5)

Run (Makefile helpers)
----------------------
- Run a single simulator with its test input:
  - `make run-sim`     # runs `./sim < test.txt`
  - `make run-sim2g`   # runs `./sim2g < q2g_test.txt`
  - `make run-sim3g`   # runs `./sim3g < q3g_test.txt`
  - `make run-sim4g`   # runs `./sim4g < q4g_test.txt`
  - `make run-sim5g`   # runs `./sim5g < q5g_test.txt`

- Interactive menu (recommended):
```
make run-all
```
This opens an interactive menu where you:
  1) pick the question (1..5)
  2) choose run mode: file input or interactive
  3) if file, enter the input file path (default shown)
  4) the program runs and when it finishes you are returned to the menu
  5) choose option `6` to exit the menu

CLI options / supported invocation patterns
-----------------------------------------
- Each simulator is a simple CLI program that accepts input from stdin.
- Run interactively (type answers when prompted):
  - `./sim`  (or `./sim2g`, etc.)
- Run using a file as input (non-interactive):
  - `./sim < test.txt`
- The Makefile `run-menu` asks whether you want file input or interactive. If you pick file input it will prompt for an input filename (default provided).

Input formats (examples)
------------------------
- `test.txt` (Q1) — example: number of devices, then for each device: id, type, messages; tower frequency count; per-frequency capacity; core overhead and core capacity. See `test.txt` for exact layout used for the demo.
- `q2g_test.txt` (Q2) and `q3g_test.txt` (Q3) — first line: number of devices `n`, followed by `n` device IDs (one per line). The runner computes channels and lists occupants of the first channel.
- `q4g_test.txt` and `q5g_test.txt` — test files contain antenna count, device count, device IDs, and then a core capacity value for the final computation. See the included test files for exact example values.

How simulators behave (high-level)
---------------------------------
- Q1 (main.cpp): builds devices, assigns frequencies round-robin, computes raw and effective messages via the `CellularCore` overhead policy, and reports per-device status (ACTIVE/DROPPED) and per-frequency lists.
- Q2 (2G): TDMA channels (200 kHz); capacity computed as channels * 16 users per 200 kHz. Lists first-channel occupants.
- Q3 (3G): CDMA model; 32 users per 200 kHz. Lists first-channel occupants and reports that each device generates 10 messages.
- Q4 (4G): OFDM with 10 kHz subchannels and MIMO (up to 4 antennas). The runner prints step-by-step calculations for per-subchannel effective capacity and assignment.
- Q5 (5G): Main band (10 kHz subchannels) plus an 1800 MHz 10 MHz high band (30 users per 1 MHz), massive MIMO (up to 16). The runner explains how main and high-band capacities are combined and computes cores needed for total messages.

Notes about I/O and behavior
---------------------------
- `basicIO` is a tiny custom I/O layer that uses a syscall wrapper. It reads tokens from stdin and writes prompts and results to stdout.
- Prompts are printed to stderr (so stdout is primarily for simulation results). This was done so that piping stdout or capturing program output leaves prompt text separate.
- `basicIO::inputint()` will detect EOF and return a sentinel; simulators handle EOF gracefully and print a helpful message if input ends prematurely.
- If you enter a non-numeric token where an integer is expected, the program prints a brief error message to stderr and re-prompts (or will fail with EOF if input runs out while piped).

Extending or scripting runs
--------------------------
- Non-interactive scripted runs can be done by redirecting stdin from a prepared test file: `./sim4g < my_input.txt`.
- The `Makefile` `run-menu` can be used interactively to choose simulators and input files. It loops until you choose option `6` to exit.
- If you prefer automated runs, let me know and I can add a scripted `make` target such as:
  - `make run-quick CHOICE=4 MODE=1 INFILE=q4g_test.txt`
  which will run the selected simulator without interactive prompts.

Troubleshooting
---------------
- If a binary is missing, run `make` first.
- If you see an assembler/linker error involving `syscall.S`, ensure you are using `g++` and that `syscall.S` is assembled by GNU assembler (the repo uses GAS/AT&T syntax).
- If programs appear to hang, check whether they are waiting for stdin (type inputs or run with `< somefile`). Use `ps` or `strace` if needed.

Possible next improvements
--------------------------
- Add `--help` to each simulator so `./sim4g --help` prints usage and input format.
- Add `make test` that runs each simulator with a test file and checks outputs against expected results.
- Add a quiet/batch mode where prompts are suppressed automatically when stdin is not a TTY.

Contact / Next steps
--------------------
If you want any of the improvements above or need me to modify input formats, add a `--help` flag, or produce machine-readable summaries (CSV/JSON), tell me which option and I will implement it.

---
Generated by the project assistant. Files and examples are present in the repository root — run `make` then `make run-all` to use the interactive menu.
