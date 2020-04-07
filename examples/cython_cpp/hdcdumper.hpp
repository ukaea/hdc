#include <hdc.hpp>

class HDCDumper {
public:
    void dump_obj(HDC h) {
        h.dump();
    }
    void dump_struct(hdc_t h) {
        std::cout << "** HDCDumper:" << std::endl;
        dump_obj(HDC(h));
    }
    HDC make_hdc() {
        return HDC("test");
    }
    hdc_t make_hdc_t() {
        HDC h("Created by HDCDumper");
        return h.as_obj();
    }
};
