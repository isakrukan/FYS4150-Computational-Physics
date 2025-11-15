
#------------------------------
# Some default flags and paths
#------------------------------
BUILD 		:= build
TESTS		:= tests
LIB			:= 
INCL 		:= -I./include
CXXFLAGS 	:= -std=c++17 -O3 -march=native
LDFLAGS 	:= -larmadillo
PY			:= 
EXE			:= 
LOGFILE 	:= problem9.log

SRC_OBJS := $(patsubst src/%.cpp, %.o, $(wildcard src/*.cpp))
MAIN_OBJS := $(SRC_OBJS) main.o
TEST_OBJS := $(SRC_OBJS) test_lattice.o

#------------------------------
# OS specific settings
#------------------------------

# Distinguishing between mac/linux and windows:
UNAME 		:= $(strip $(OS))

# Checking whether folder exists or not is different between mac/linux and windows
ifeq ($(UNAME),Windows_NT)
$(info I am sorry for using Windows)
INCL 		+= -I"C:/vcpkg/installed/x64-mingw-dynamic/include"
LIB 		:= -L"C:/vcpkg/installed/x64-mingw-dynamic/lib"
LDFLAGS   	+= #-lhdf5 -lhdf5_hl
DELETE		:= del /Q
PY 			+= py 
EXE			:= .exe

define MKDIR 
	if not exist "$(1)" mkdir "$(1)"
endef 

else 
$(info I can't update my mac but I love it so much)
CXXFLAGS += #$(shell pkg-config --cflags hdf5)
LDFLAGS  += #$(shell pkg-config --libs hdf5)
DELETE		:= rm -f
PY 			+= python3

define MKDIR 
	mkdir -p "$(1)"
endef 

endif 

define LOGTIME
	@echo "Log time: $$(date)" $(LOG_APPEND)
endef



#------------------------------
# Makefile rules
#------------------------------

OS_message:
	@echo $(UNAME)

outfolder:
	@$(call MKDIR,$(BUILD))

# - - - - - - -  
# Serial build
# - - - - - - -  
compile:
	g++ -c $(wildcard src/*.cpp) $(INCL) $(LIB) $(CXXFLAGS)
	g++ -c main.cpp -o main.o	 $(INCL) $(LIB) $(CXXFLAGS) 
	g++ -c $(TESTS)/test_lattice.cpp -o test_lattice.o $(INCL) $(LIB) $(CXXFLAGS) 

link:
	g++ $(MAIN_OBJS) $(LIB) $(LDFLAGS) -o $(BUILD)/main$(EXE)
	g++ $(TEST_OBJS) $(LIB) $(LDFLAGS) -o $(BUILD)/test_lattice$(EXE)

# - - - - - - -  
# OpenMP build
# - - - - - - -  
compile_openmp:
	g++ -fopenmp -c $(wildcard src/*.cpp) $(INCL) $(LIB) $(CXXFLAGS)
	g++ -fopenmp -c main.cpp -o main_openmp.o $(INCL) $(LIB) $(CXXFLAGS)

link_openmp:
	g++ $(SRC_OBJS) main_openmp.o $(LIB) $(LDFLAGS) -fopenmp -o $(BUILD)/main_openmp$(EXE)

clean:
	-$(DELETE) *.o

# Clean between serial and OpenMP builds for debugging/safety
all: outfolder compile link clean compile_openmp link_openmp clean


# Generates data for problems 5 & 6
# Should take around ~30 sec w/ 1e6 MC cycles
run_problem_5:
	@$(BUILD)/main --run --seed 1 -L 20 -T 1.0 -n_cycles 1000000
	@$(BUILD)/main --run --seed 2 -L 20 -T 1.0 -n_cycles 1000000 --ordered
	@$(BUILD)/main --run --seed 3 -L 20 -T 2.4 -n_cycles 1000000
	@$(BUILD)/main --run --seed 4 -L 20 -T 2.4 -n_cycles 1000000 --ordered

# Runs timing tests for problem 7 (see run_timing_tests.sh for details)
run_problem_7:
	bash run_timing_tests.sh

# NOTE: Runs a smaller test version of problem 8, meant to verify that the code
# is working (more accurate runs take a long time and should be done manually).
# These should take around 2-4 minutes in total.
run_downscaled_problem_8:
	@$(BUILD)/main_openmp --temperature_scan --seed 42*1 --outdir out/temperature_scans/tests/results-L40 --name scan_1 -T_steps 10 -L 40 -n_cycles 100000 -n_burn 10000
	@$(BUILD)/main_openmp --temperature_scan --seed 42*2 --outdir out/temperature_scans/tests/results-L60 --name scan_1 -T_steps 10 -L 60 -n_cycles 100000 -n_burn 10000
	@$(BUILD)/main_openmp --temperature_scan --seed 42*3 --outdir out/temperature_scans/tests/results-L80 --name scan_1 -T_steps 10 -L 80 -n_cycles 100000 -n_burn 10000
	@$(BUILD)/main_openmp --temperature_scan --seed 42*4 --outdir out/temperature_scans/tests/results-L100 --name scan_1 -T_steps 10 -L 100 -n_cycles 100000 -n_burn 10000
			

py_plots:
	@$(PY) problem_4.py
	@$(PY) problem_5.py
	@$(PY) problem_6.py
	@$(PY) problem_7.py
	@$(PY) problem_8.py
	@$(PY) problem_9.py

run_tests:
	@$(BUILD)/test_lattice$(EXE)


# Build first (via 'all'), then run everything and plot
project: all run_tests run_problem_5 run_downscaled_problem_8 run_problem_7 py_plots
	@echo "Complete project finished."