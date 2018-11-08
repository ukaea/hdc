function tree = test_cpos(equilibriumin)
    tree = HDC()
    header = 'hdc_fortran_module.h';
    lib = '../install/lib/libhdc_fortran_module.so';
    loadlibrary(lib,header);
    libfunctions('libhdc_fortran_module')
    equilibriumin_c_struct = libstruct('hdc_t',equilibriumin.as_hdc_t());
    tree_c_struct = libstruct('hdc_t',tree.as_hdc_t());
    calllib('libhdc_fortran_module','test_cpos_f2c', equilibriumin_c_struct, tree_c_struct);
%     unloadlibrary('libhdc_fortran_module');
    return;
end
