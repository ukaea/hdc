CXX?=g++
HDC_INCLUDE=-I. $(shell pkg-config hdc --cflags)
HDC_LIBS=$(shell pkg-config hdc --libs)
HDC_CFLAGS=-fPIC -g -pedantic -O2 -std=c++14 -Wno-type-limits -Wall -Wextra -rdynamic

all: libhdcdumper.so

libhdcdumper.so: hdcdumper.cpp
	$(CXX) $(HDC_CFLAGS) -shared -o libhdcdumper.so hdcdumper.cpp $(HDC_INCLUDE) $(HDC_LIBS)

clean:
	rm libhdcdumper.so
