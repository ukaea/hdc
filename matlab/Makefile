INC = -I/home/david/projects/hdc/install/include -I/home/david/projects/hdc/install/include/hdc
LIB = -L/home/david/projects/hdc/install/lib -lhdc -lboost_system
RPATH = LD_RUN_PATH=/home/david/projects/hdc/install/lib
MEX = mex -v

all: hdc_mex.mexa64

hdc_mex.mexa64: hdc_mex.cpp
	$(RPATH) $(MEX) $(INC) hdc_mex.cpp $(LIB)

clean:
	rm -f hdc_mex.mexa64
