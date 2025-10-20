SRC 		:= penningTrap.o solver.o arg_parser.o
BUILD 		:= build

# Distinguishing between mac/linux and windows:
UNAME 		:= $(strip $(OS))

# Checking whether folder exists or not is different between mac/linux and windows
ifeq ($(UNAME),Windows_NT)
$(info I am sorry for using Windows)
INCL 		:= -I./include -I"C:/vcpkg/installed/x64-mingw-dynamic/include"
LIB 		:= -L"C:/vcpkg/installed/x64-mingw-dynamic/lib"

DELETE		:= del /Q
define MKDIR 
	if not exist "$(1)" mkdir "$(1)"
endef 
else 
$(info I can't update my mac but I love it so much)
INCL 		:= -I./include
LIB 		:=
DELETE		:= rm -f

INCL	+= -I/usr/include/hdf5/serial
LIB		+= -lhdf5_serial -lhdf5_serial_hl -L/usr/lib/x86_64-linux-gnu/hdf5/serial


define MKDIR 
	mkdir -p "$(1)"
endef 
endif 

define compile_func
	g++ -c $1 -DARMA_USE_HDF5 $(INCL) $(LIB) -o $2
endef 

OS_message:
	@echo $(UNAME)

outfolder:
	@$(call MKDIR,$(BUILD))

compile:
	@$(call compile_func, src/penningTrap.cpp, penningTrap.o)
	@$(call compile_func, src/solver.cpp, solver.o)
	@$(call compile_func, src/arg_parser.cpp, arg_parser.o)
	@$(call compile_func, main.cpp, main.o)

link:
	g++ main.o $(SRC) $(LIB) -larmadillo -lhdf5 -o $(BUILD)/main	

clean:
	-$(DELETE) *.o

all: outfolder compile link clean

