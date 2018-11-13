function tree = test_cpos(equilibriumin)
    tree = HDC()
    header = 'hdc_fortran_module.h';
    lib = '../../install/lib/libhdc_fortran_module.so';
    libname = 'libhdc_fortran_module';
    if ~libisloaded(libname)
        loadlibrary(lib,header);
    end
    % libfunctions(libname)
    equilibriumin_c_struct = libstruct('hdc_t',equilibriumin.as_hdc_t());
    tree_c_struct = libstruct('hdc_t',tree.as_hdc_t());
    calllib(libname,'test_cpos_f2c', equilibriumin_c_struct, tree_c_struct);

    % You should eventually close the library, but you will probably have to wait for HDC objects produced by the library to end. Otherwise, you will get an error...
    % unloadlibrary(libname);
    return;
end
