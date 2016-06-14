#include <stdio.h>
#include "types.h"
#include <string.h>
//#include "hdc_c.h"

void* __hdc_fortran_MOD_hdc_new_ptr();
void* __hdc_fortran_MOD_hdc_get_ptr_f(struct hdc_t* tree);
void __hdc_fortran_MOD_hdc_add_child(struct hdc_t* tree,char* path,struct hdc_t* node);
void hdc_dump(struct hdc_t* tree);

struct hdc_t* hdc_new_empty();
int main() {
    struct hdc_t* tree = (struct hdc_t*)__hdc_fortran_MOD_hdc_new_ptr();
    struct hdc_t* node = (struct hdc_t*)__hdc_fortran_MOD_hdc_new_ptr();
    __hdc_fortran_MOD_hdc_add_child(&tree,"aaa/bbb/ccc",&node);
    struct hdc_t* tree2 = (struct hdc_t*)__hdc_fortran_MOD_hdc_get_ptr_f(tree);
    hdc_dump(tree2);
    return 0;
}