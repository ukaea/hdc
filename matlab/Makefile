INC = -I../install/include -I../thirdparty/ -I../thirdparty/jsoncpp -I../thirdparty/Pluma-1.1/include
LIB = -L/compass/home/urban/workspace/hdc-git/install/lib -lhdc -lboost_system
RPATH = LD_RUN_PATH=/compass/home/urban/workspace/hdc-git/install/lib

all: hdc_mex.mexa64

hdc_mex.mexa64: hdc_mex.cpp
	$(RPATH) mex $(INC) hdc_mex.cpp $(LIB)

clean:
	rm -f hdc_mex.mexa64

