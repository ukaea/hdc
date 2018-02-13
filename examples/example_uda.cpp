#include <iostream>
#include "hdc.hpp"

#include <vector>
#include <string>
using namespace std;

int main(int argc, const char *argv[]) {

    for (int i=0;i<=33;i++) {
        if (i == 17) continue; // Not there
        if (i == 29) continue; // Not there

        if (i == 18) continue; // Too long
        std::cout << "UDA Test #" << i << std::endl;
        HDC t = HDC::from_uda("TESTPLUGIN::test"+std::to_string(i)+"()","");
        t.dump();
        std::getchar();
    }

    int i = 28;
    std::cout << "UDA Test #" << i << std::endl;
    HDC t = HDC::from_uda("TESTPLUGIN::test"+std::to_string(i)+"()","");
    t.dump();

if (argc == 2) {
    std::string dirname = argv[1];
    std::cout << "netcdf" << std::endl;
    HDC t;


    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = "+dirname+"/test_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = "+dirname+"/test_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = "+dirname+"/test_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = "+dirname+"/test_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = "+dirname+"/test_scalars.nc)","");
    t.dump();

//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = "+dirname+"/test_1d_arrays.nc)","");
//     t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = "+dirname+"/test_1d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = "+dirname+"/test_1d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = "+dirname+"/test_1d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = "+dirname+"/test_1d_arrays.nc)","");
    t.dump();


//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = "+dirname+"/test_2d_arrays.nc)","");
//     t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = "+dirname+"/test_2d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = "+dirname+"/test_2d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = "+dirname+"/test_2d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = "+dirname+"/test_2d_arrays.nc)","");
    t.dump();



//     [readCDFVar]: NetCDF: HDF error
//     [readCDF]: Unable to Read Data Values
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleString, file = "+dirname+"/test_string.nc)","");
    t.dump();

    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleString, file = "+dirname+"/test_string_arrays.nc)","");
    t.dump();

//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = "+dirname+"/test_compound_1d_arrays.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = "+dirname+"/test_compound_1d_arrays.nc)","");
//     t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = "+dirname+"/test_compound_1d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = "+dirname+"/test_compound_1d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = "+dirname+"/test_compound_1d_arrays.nc)","");
    t.dump();


//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = "+dirname+"/test_compound_scalars.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = "+dirname+"/test_compound_scalars.nc)","");
//     t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = "+dirname+"/test_compound_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = "+dirname+"/test_compound_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = "+dirname+"/test_compound_scalars.nc)","");
    t.dump();


//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = "+dirname+"/test_array_compound_scalars.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = "+dirname+"/test_array_compound_scalars.nc)","");
//     t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = "+dirname+"/test_array_compound_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = "+dirname+"/test_array_compound_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = "+dirname+"/test_array_compound_scalars.nc)","");
    t.dump();
}
    return 0;
}
