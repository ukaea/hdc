/**
 * This is not intended to be run directly, there are differences C and fortran string making this complicated. I have just tested the following code aftet wrapping the path string inside fortran hdc_add_child function using c_to_f_string. The better option would be to do that in C, but I don't see any reason for further investications of this. The gfortran prefixes are the same story.
 */

#include <stdio.h>
#include "types.h"
#include <string.h>
//#include "hdc_c.h"

char* c_str2fortran(char* str) {
    char* f_str = (char*)malloc(strlen(str)*sizeof(char));
    memcpy(f_str,str,strlen(str)*sizeof(char));
    return f_str;
}

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