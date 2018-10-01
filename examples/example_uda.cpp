#include <iostream>
#include "hdc.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <vector>
#include <string>
#include <vector>
using namespace std;

void imasdd_plugin() {
//     HDC h = HDC::uda2HDC("imasdd::get(path='/magnetics')", "");
    HDC h = HDC::load("uda_new://imasdd::get(path='/magnetics')");
    h.print_info();
    h.dump();
    return;
}

void mdsplus_plugin() {
    //HDC h = HDC::load ("uda://_a=1+41|MDSPLUS::localhost");
    std::vector<std::string> paths = {
        "uda://\\magnetics::vloop[*,\"001\"]|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\magnetics::flux[*,\"001\"]|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\magnetics::flux[*,\"001\"])|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\r_contour|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\z_contour|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\results::r_axis)|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\magnetics::rbphi|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\magnetics::rbphi)|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::fir:n_average|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\results::fir:n_average)|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::proffit.avg_time:neft|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\results::proffit.avg_time:neft)|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::ne_thomson:foo|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::ibs:z_eff|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::zx:comment|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\results::zx:foo)|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\results::toray.input:P_GYRO)|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\TCV_SHOT::TOP.RESULTS.TORAY.INPUT.RHO_TORAY|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\TCV_SHOT::TOP.RESULTS.TORAY.INPUT.RHO_TORAY,1)|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\TCV_SHOT::TOP.RESULTS.TORAY.OUTPUT_X:PDENS|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\ATLAS::NBH.DATA.MAIN_ADC:DATA|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://data(\\VSYSTEM::TCV_NBHDB_I[\"NBI:LCS_MODE\"])|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\ATLAS::NBH.DATA.MAIN_ADC:DATA,0)|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\results::l_i)|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::oh_power|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::total_energy|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::beta_pol|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::q_psi|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://\\results::bolo:prad:total|MDSPLUS::localhost:8001/tcv_shot/60950",
        "uda://dim_of(\\results::bolo:prad:total)|MDSPLUS::localhost:8001/tcv_shot/60950"
    };
    for (auto& path : paths) {
        HDC h = HDC::load(path);
        if (strcmp(h.get_type_str(),"null") == 0) {
            std::cout << "EE: ";
            std::cout << path << std::endl;
        }
        //h.print_info();
    }

//     h.print_info();
//     h.dump();
    return;
}


void imas_plugin() {
/* Here we assume that UDA server is running locally, if not, please comment this and set UDA_HOST environment variable... */
//     char hostname[HOST_NAME_MAX];
//     gethostname(hostname, HOST_NAME_MAX);
//     setenv("UDA_HOST", hostname, 1);

//    HDC h = HDC::from_uda("imas::get(expName='JET', idx=0, group='magnetics', variable='ids_properties/comment', type=string, rank=0, shot=84600, )","");
//    HDC h = HDC::from_uda("imas::get(expName='JET', idx=0, group='magnetics', variable='flux_loop/3/flux/data', type=double, rank=1, shot=84600, )","");
//    HDC h = HDC::from_uda("imas::get(expName='JET', idx=0, group='magnetics', variable='flux_loop/3/flux/data', type=float, rank=1, shot=84600, )","");
    HDC h = HDC::load("uda://imas::get(expName='JET', idx=0, group='magnetics', variable='flux_loop/3/flux/data', type=double, rank=1, shot=84600, )");
    h.print_info();
    h.dump();
    return;
}

void test_plugin() {
    /* TESTPLUGIN, no metadata */
    for (int i=0;i<=33;i++) {
        if (i == 17) continue; // Not there
        if (i == 29) continue; // Not there

        if (i == 18) continue; // Too long
        std::cout << "UDA Test #" << i << std::endl;
        HDC t = HDC::from_uda("TESTPLUGIN::test"+std::to_string(i)+"()","");
        t.dump();
        std::getchar();
    }
    /* TESTPLUGIN, with metadata */
    for (int i=0;i<=33;i++) {
        if (i == 17) continue; // Not there
        if (i == 29) continue; // Not there

        if (i == 18) continue; // Too long
        std::cout << "UDA Test #" << i << std::endl;
        HDC t = HDC::from_uda("TESTPLUGIN::test"+std::to_string(i)+"()","",true);
        t.dump();
        std::getchar();
    }
}

void netcdf_plugin(int argc, const char *argv[]) {
    /* NEWCDF4, no metadata */
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
    return;
}


int main(int argc UNUSED, const char *argv[] UNUSED) {
//    mdsplus_plugin();
     imasdd_plugin();
//     imas_plugin();
//     test_plugin();
//     netcdf_plugin(argc, argv);
    return 0;
}
