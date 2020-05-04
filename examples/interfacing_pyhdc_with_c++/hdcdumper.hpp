#include <hdc.hpp>
/* This is simple example of Cython binding between C++ class producing/consuming HDC objects.
 * Because AFAIK Cython cannot work directly with C++ instance, you have to pass hdc_t structure instead.
 */
class HDCDumper {
public:
    void dump(hdc_t h) {
        std::cout << "** HDCDumper:" << std::endl;
        HDC(h).dump();
    }
    hdc_t make_hdc_t() {
        HDC h("Created by HDCDumper");
        return h.as_obj();
    }
};
