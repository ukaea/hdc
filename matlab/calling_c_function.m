% results in segfault - probably Boost?

% Prepare HDC
h = HDC(1.23456789);
sm = h.as_hdc_t();

% Load library
header = '/home/david/projects/hdc/install/include/hdc/hdc_c.h';
lib = '/home/david/projects/hdc/install/lib/libchdc.so';
loadlibrary(lib,header)
libfunctions('libchdc')

% Prepare c struct
sc = libstruct('hdc_t')
sc.storage = sm.storage
sc.uuid(1:end-1) = sm.uuid(:)

% Kill MATLAB:)
res = calllib('libchdc','hdc_as_double_scalar',sc,'')
