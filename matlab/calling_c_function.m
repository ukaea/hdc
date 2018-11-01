% Prepare HDC
%HDC.init('mdbm');
h = HDC(1.23456789);
sm = h.as_hdc_t()

% Load library
header = '/home/david/projects/hdc/install/include/hdc/hdc_c.h';
lib = '/home/david/projects/hdc/install/lib/libchdc.so';
loadlibrary(lib,header)
libfunctions('libchdc')

% Prepare c struct
sc = libstruct('hdc_t',sm)

% call the library
res = calllib('libchdc','hdc_as_double_scalar',sc,'')
