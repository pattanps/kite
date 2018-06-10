OS := $(shell uname)

ifeq ($(OS),Darwin)
CC = g++-mp-6  -DEIGEN_DONT_PARALLELIZE -fdiagnostics-color=always  -O2  #-ftree-vectorize  -ftree-vectorizer-verbose=7 -fopt-info-vec-missed
else
CC = g++ -march=native  -O2 
endif	

CFLAGS = -DEIGEN_DONT_PARALLELIZE -fopenmp -std=gnu++11 -Wall -ffast-math 
CINCLUDE = -I/opt/local/lib/ -ISrc -I/usr/local/hdf5/include
CINCLUDE += -I$(HOME)/include/eigen3 -I/opt/local/include/eigen3 -I/usr/include/eigen3/ -I/usr/include/hdf5/serial
CLIBS = -L/usr/local/hdf5/lib/ -L/usr/lib/x86_64-linux-gnu/hdf5/serial
CLIBS += -lhdf5_hl_cpp -lhdf5_cpp -lhdf5_hl -lhdf5  
VPATH = .
OBJS = *.o

compile_main=0
verbose=1
debug=0
estimate_time=1

all:    clean
	cd Src; $(CC) $(CFLAGS) $(CINCLUDE) -DDEBUG=$(debug) -DCOMPILE_MAIN=$(compile_main) -DVERBOSE=$(verbose) -DESTIMATE_TIME=$(estimate_time)  -c *.cpp  
	@echo "linking..."
	cd Src; $(CC) $(OBJS) $(CLIBS) $(CFLAGS) -o ../KITEx
	rm -f Src/*.o
	cd ..

debug:  clean  
	cd Src; $(CC) $(CFLAGS) $(CINCLUDE) $(CDEFS)  -DMEM1=$(MEM1) -DMEM2=$(MEM2) -c *.cpp   -g
	@echo "linking..."
	cd Src; $(CC) $(CLIBS) $(CFLAGS) $(CINCLUDE) $(CDEFS) $(OBJS)  -o ../KITEx -g
	cd ..

clean:
	rm -f Src/*.o Src/*~ core *~
