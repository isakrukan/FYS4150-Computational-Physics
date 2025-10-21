
#------------------------------
# Some default flags and paths
#------------------------------
BUILD 		:= build
LIB			:= 
INCL 		:= -I./include
CXXFLAGS 	:= -DARMA_USE_HDF5 -O2 
LDFLAGS 	:= -larmadillo


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
DELETE		:= del /Q

define MKDIR 
	if not exist "$(1)" mkdir "$(1)"
endef 

else 
$(info I can't update my mac but I love it so much)
CXXFLAGS += $(shell pkg-config --cflags hdf5)
LDFLAGS  += $(shell pkg-config --libs hdf5)
DELETE		:= rm -f

define MKDIR 
	mkdir -p "$(1)"
endef 

endif 

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

