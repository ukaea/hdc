# Setup the HDC environment (and UDA -- see the separate instructions)
# assume HDC is installed into ../../install
# export HDC_PREFIX=`realpath ../../install`
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HDC_PREFIX}/lib
# export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:${HDC_PREFIX}/lib/pkgconfig
# export FC=gfortran-6

SOURCE = process_data.f90
LIB = process_data.so

FCFLAGS = -fPIC $(shell pkg-config fhdc --cflags)
LFLAGS = $(shell pkg-config fhdc --libs)
LINK = ${FC}

OBJS = $(SOURCE:.f90=.o)

all: $(LIB)

$(LIB): $(OBJS)
	${FC} -shared -o $(LIB) $(OBJS) $(LFLAGS)

%.o: %.f90
	${FC} -c $< $(FCFLAGS)

clean:
	rm -f *.o

show:
	@echo "FC: ${FC} $(FCFLAGS)"
	@echo "LD: ${LINK} $(LFLAGS)"
