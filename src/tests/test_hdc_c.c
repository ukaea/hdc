#include <stdio.h>
#include "types.h"
#include "hdc_c.h"

int main(int argc, char **argv) {
    printf("Hello from C...\n");
    
    struct hdc_t* tree = hdc_new_empty();
    
    // test add
    hdc_add_child(tree,"aaa/bbb/ccc",hdc_new_empty());
    hdc_add_child(tree,"aaa/bbb/eee",hdc_new_empty());
    hdc_add_child(tree,"bbb/eee/aaa",hdc_new_empty());
    hdc_add_child(tree,"123/123/123",hdc_new_empty());
    
    // test get
    struct hdc_t* n = hdc_get_child(tree,"aaa/bbb");
    struct hdc_t* k = hdc_get_child(tree,"ccc");
    
    // test has
    printf("has_child: %d\n",hdc_has_child(tree,"aaa/bbb/ccc"));
    printf("has_child: %d\n",hdc_has_child(tree,"aaa/bbb/ddd"));
    printf("has_child: %d\n",hdc_has_child(tree,"aaa/bbb"));
    // test delete
    hdc_delete_child(tree,"aaa/bbb");
    
    printf("has_child: %d\n",hdc_has_child(tree,"aaa/bbb"));
    // test set
    hdc_set_child(tree,"bbb/eee/aaa",hdc_new_empty());
    hdc_set_child(tree,"bbb",hdc_new_empty());
    printf("has_child: %d\n",hdc_has_child(tree,"bbb/eee/aaa"));
    
    // test data_set
    struct hdc_t* data = hdc_new_empty();
    int8_t array[4] = {1,2,3,4};
    long int shape[1];
    shape[0] = 4;
    hdc_set_data_int8(data,1,shape,(void*)array);
    int8_t* aaa = (int8_t*)hdc_as_int_1d(data);
    printf("%d %d\n",(int)aaa[0],(int)aaa[2]); 
    
    struct hdc_t* new = hdc_copy(tree);
    hdc_delete(tree);
    printf("has_child: %d\n",hdc_has_child(new,"123/123/123"));
    
    struct hdc_t* scd = hdc_new_empty();
    hdc_set_data_double_scalar(scd,56789.987654321);
    printf("has_child: %f\n",hdc_as_double_scalar(scd));
    return 0;
}