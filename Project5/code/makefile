
#------------------------------
# Some default flags and paths
#------------------------------
BUILD 		:= build
TESTS		:= tests
LIB			:= 
INCL 		:= -I./include
CXXFLAGS 	:= -DARMA_USE_HDF5 -std=c++17 -O3 -march=native#  -g -O0 
LDFLAGS 	:= -larmadillo -lsuperlu -lopenblas -llapack
PY			:= 
EXE			:= 
LOGFILE 	:= problem9.log

SRC_OBJS := $(patsubst src/%.cpp, %.o, $(wildcard src/*.cpp))
MAIN_OBJS := $(SRC_OBJS) main.o
TEST_OBJS := $(SRC_OBJS) test.o

#------------------------------
# OS specific settings
#------------------------------

# Distinguishing between mac/linux and windows:
UNAME 		:= $(strip $(OS))

# Checking whether folder exists or not is different between mac/linux and windows
ifeq ($(UNAME),Windows_NT)
$(info I am sorry for using Windows)
# INCL 		+= -I"C:/vcpkg/installed/x64-mingw-dynamic/include"
# LIB 		:= -L"C:/vcpkg/installed/x64-mingw-dynamic/lib"
 LDFLAGS   += -lhdf5 -lhdf5_hl
# DELETE	:= del /Q
DELETE      := rm -f
PY 			+= py 
EXE			:= .exe

# define MKDIR 
# 	if not exist "$(1)" mkdir "$(1)"
# endef 
define MKDIR
	mkdir -p "$(1)"
endef

else 
$(info I can't update my mac but I love it so much)
CXXFLAGS += $(shell pkg-config --cflags hdf5)
#CXXFLAGS += $(shell pkg-config --cflags superlu)
LDFLAGS  += $(shell pkg-config --libs hdf5)
LDFLAGS  += $(shell pkg-config --libs superlu)
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
	g++ -c $(TESTS)/test.cpp -o test.o $(INCL) $(LIB) $(CXXFLAGS) 

link:
	g++ $(MAIN_OBJS) $(LIB) $(LDFLAGS) -o $(BUILD)/main$(EXE)
	g++ $(TEST_OBJS) $(LIB) $(LDFLAGS) -o $(BUILD)/test$(EXE)

clean:
	-$(DELETE) *.o

all: outfolder compile link clean

			

py_plots:

run_tests:
	@$(BUILD)/test$(EXE)


# Build first (via 'all'), then run everything and plot
project: all run_tests py_plots
	@echo "Complete project generated."