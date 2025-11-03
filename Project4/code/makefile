
#------------------------------
# Some default flags and paths
#------------------------------
BUILD 		:= build
LIB			:= 
INCL 		:= -I./include
#CXXFLAGS 	:= -DARMA_USE_HDF5 -std=c++17 -O2 
CXXFLAGS 	:= -std=c++17 -O2 
LDFLAGS 	:= -larmadillo
PY			:= 
EXE			:= 
LOGFILE 	:= problem9.log

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
LDFLAGS   	+= -lhdf5 -lhdf5_hl
DELETE		:= del /Q
PY 			+= py 
EXE			:= .exe

define MKDIR 
	if not exist "$(1)" mkdir "$(1)"
endef 

else 
$(info I can't update my mac but I love it so much)
CXXFLAGS += $(shell pkg-config --cflags hdf5)
LDFLAGS  += $(shell pkg-config --libs hdf5)
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

compile:
	g++ -c $(wildcard src/*.cpp) $(INCL) $(LIB) $(CXXFLAGS)
	g++ -c main.cpp -o main.o	 $(INCL) $(LIB) $(CXXFLAGS) 

link:
	g++ $(wildcard *.o) $(LIB) $(LDFLAGS) -o $(BUILD)/main

clean:
	-$(DELETE) *.o

all: outfolder compile link clean

run_problem_5:
	@$(BUILD)/main -L 20 -T 1.0 -n_cycles 10000 -n_steps 10000
	@$(BUILD)/main -L 20 -T 1.0 -n_cycles 10000 -n_steps 10000 --ordered
	@$(BUILD)/main -L 20 -T 2.4 -n_cycles 10000 -n_steps 10000
	@$(BUILD)/main -L 20 -T 2.4 -n_cycles 10000 -n_steps 10000 --ordered

py_plots:
	@$(PY) problem_5.py
	@$(PY) problem_6.py


# Build first (via 'all'), then run everything and plot
project: all run_problem_5 py_plots
	@echo "Complete project finished."