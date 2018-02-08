#include <iostream>
#include "hdc.hpp"

#include <vector>
#include <string>
using namespace std;

int main(int argc, const char *argv[]) {

    for (int i=0;i<=33;i++) {
        if (i == 17) continue;
        if (i == 29) continue;
        std::cout << "UDA Test #" << i << std::endl;
        HDC t = HDC::from_uda("TESTPLUGIN::test"+std::to_string(i)+"()","");
        t.dump();
        std::getchar();
    }

    return 0;
}
