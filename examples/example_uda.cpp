#include <iostream>
#include "hdc.hpp"

#include <vector>
#include <string>
using namespace std;

int main(int argc, const char *argv[]) {

//     for (int i=0;i<=33;i++) {
//         if (i == 17) continue; // Not there
//         if (i == 29) continue; // Not there
//
//         if (i == 18) continue; // Too long
//         std::cout << "UDA Test #" << i << std::endl;
//         HDC t = HDC::from_uda("TESTPLUGIN::test"+std::to_string(i)+"()","");
//         t.dump();
//         std::getchar();
//     }

//     int i = 28;
//     std::cout << "UDA Test #" << i << std::endl;
//     HDC t = HDC::from_uda("TESTPLUGIN::test"+std::to_string(i)+"()","");
//     t.dump();

    std::cout << "netcdf" << std::endl;
    HDC t;
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = /home/david/sw/uda-git/test/data/test_scalars.nc)","");
//     t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = /home/david/sw/uda-git/test/data/test_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = /home/david/sw/uda-git/test/data/test_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = /home/david/sw/uda-git/test/data/test_scalars.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = /home/david/sw/uda-git/test/data/test_scalars.nc)","");
    t.dump();

//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = /home/david/sw/uda-git/test/data/test_1d_arrays.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = /home/david/sw/uda-git/test/data/test_1d_arrays.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = /home/david/sw/uda-git/test/data/test_1d_arrays.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = /home/david/sw/uda-git/test/data/test_1d_arrays.nc)","");
//     t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = /home/david/sw/uda-git/test/data/test_1d_arrays.nc)","");
    t.dump();


//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = /home/david/sw/uda-git/test/data/test_2d_arrays.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = /home/david/sw/uda-git/test/data/test_2d_arrays.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = /home/david/sw/uda-git/test/data/test_2d_arrays.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = /home/david/sw/uda-git/test/data/test_2d_arrays.nc)","");
//     t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = /home/david/sw/uda-git/test/data/test_2d_arrays.nc)","");
    t.dump();



//     [readCDFVar]: NetCDF: HDF error
//     [readCDF]: Unable to Read Data Values
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleString, file = /home/david/sw/uda-git/test/data/test_string.nc)","");
//     t.dump();

    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleString, file = /home/david/sw/uda-git/test/data/test_string_arrays.nc)","");
    t.dump();

//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleByte, file = /home/david/sw/uda-git/test/data/test_compound_1d_arrays.nc)","");
//     t.dump();
//     t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleShort, file = /home/david/sw/uda-git/test/data/test_compound_1d_arrays.nc)","");
//     t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleInt, file = /home/david/sw/uda-git/test/data/test_compound_1d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleFloat, file = /home/david/sw/uda-git/test/data/test_compound_1d_arrays.nc)","");
    t.dump();
    t = HDC::from_uda("NEWCDF4::read(signal=/testGroup/simpleDouble, file = /home/david/sw/uda-git/test/data/test_compound_1d_arrays.nc)","");
    t.dump();




    return 0;
}
