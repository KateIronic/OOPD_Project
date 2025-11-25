CC = g++
CFLAGS = -std=gnu++17 -O2
ASM = syscall.S

.PHONY: all sim sim2g sim3g sim4g sim5g clean run-sim run-sim2g run-sim3g run-sim4g run-sim5g run-all help

all: sim sim2g sim3g sim4g sim5g

sim: main.cpp basicIO.cpp $(ASM)
	$(CC) $(CFLAGS) main.cpp basicIO.cpp $(ASM) -o sim

sim2g: q2g_main.cpp q2g.cpp basicIO.cpp $(ASM)
	$(CC) $(CFLAGS) q2g_main.cpp q2g.cpp basicIO.cpp $(ASM) -o sim2g

sim3g: q3g_main.cpp q3g.cpp basicIO.cpp $(ASM)
	$(CC) $(CFLAGS) q3g_main.cpp q3g.cpp basicIO.cpp $(ASM) -o sim3g

sim4g: q4g_main.cpp q4g.cpp basicIO.cpp $(ASM)
	$(CC) $(CFLAGS) q4g_main.cpp q4g.cpp basicIO.cpp $(ASM) -o sim4g

sim5g: q5g_main.cpp q5g.cpp basicIO.cpp $(ASM)
	$(CC) $(CFLAGS) q5g_main.cpp q5g.cpp basicIO.cpp $(ASM) -o sim5g

run-sim:
	@echo "[RUN] Question 1 (sim): usage: ./sim                     (interactive) or ./sim < test.txt"
	./sim < test.txt

run-sim2g:
	@echo "[RUN] Question 2 (2G): usage: ./sim2g [< inputfile]  (default: q2g_test.txt)"
	@echo "  Example: ./sim2g < q2g_test.txt"
	./sim2g < q2g_test.txt

run-sim3g:
	@echo "[RUN] Question 3 (3G): usage: ./sim3g [< inputfile]  (default: q3g_test.txt)"
	@echo "  Example: ./sim3g < q3g_test.txt"
	./sim3g < q3g_test.txt

run-sim4g:
	@echo "[RUN] Question 4 (4G): usage: ./sim4g [--antennas N] [< inputfile]"
	@echo "  Example interactive: ./sim4g   OR  piped: ./sim4g < q4g_test.txt"
	./sim4g < q4g_test.txt

run-sim5g:
	@echo "[RUN] Question 5 (5G): usage: ./sim5g [--antennas N] [< inputfile]"
	@echo "  Example interactive: ./sim5g   OR  piped: ./sim5g < q5g_test.txt"
	./sim5g < q5g_test.txt

.ONESHELL:
run-menu:
	@while true; do \
	  echo; \
	  echo "Choose simulator to run (enter number):"; \
	  echo " 1) Question 1 (sim)   - usage: ./sim [< inputfile] (default: test.txt)"; \
	  echo " 2) Question 2 (2G)    - usage: ./sim2g [< inputfile] (default: q2g_test.txt)"; \
	  echo " 3) Question 3 (3G)    - usage: ./sim3g [< inputfile] (default: q3g_test.txt)"; \
	  echo " 4) Question 4 (4G)    - usage: ./sim4g [< inputfile] (default: q4g_test.txt)"; \
	  echo " 5) Question 5 (5G)    - usage: ./sim5g [< inputfile] (default: q5g_test.txt)"; \
	  echo " 6) Exit menu"; \
	  read -p "Enter choice (1-6): " choice; \
	  case "$$choice" in \
	    1) target="./sim"; default="test.txt" ;; \
	    2) target="./sim2g"; default="q2g_test.txt" ;; \
	    3) target="./sim3g"; default="q3g_test.txt" ;; \
	    4) target="./sim4g"; default="q4g_test.txt" ;; \
	    5) target="./sim5g"; default="q5g_test.txt" ;; \
	    6) echo "Exiting menu."; break ;; \
	    *) echo "Invalid choice"; continue ;; \
	  esac; \
	  if [ ! -x "$$target" ]; then \
	    echo "Binary $$target not found. Building..."; \
	    make $$(echo "$$target" | sed 's|./||'); \
	  fi; \
	  read -p "Run mode: 1) file input  2) interactive  [default 2]: " mode; \
	  if [ -z "$$mode" ]; then mode=2; fi; \
	  if [ "$$mode" = "1" ]; then \
	    read -p "Enter input file [default: $$default]: " infile; \
	    if [ -z "$$infile" ]; then infile=$$default; fi; \
	    echo "Running $$target < $$infile"; \
	    $$target < "$$infile"; \
	  else \
	    echo "Running $$target interactively..."; \
	    $$target; \
	  fi; \
	  echo; read -p "Press ENTER to return to menu..." dummy; \
	done

run-all: run-menu

clean:
	rm -f sim sim2g sim3g sim4g sim5g *.o

help:
	@echo "Usage: make [all|sim|sim2g|sim3g|sim4g|sim5g|run-sim|run-sim2g|run-sim3g|run-sim4g|run-sim5g|run-all|clean]"
