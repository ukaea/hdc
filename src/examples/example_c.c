#include <stdio.h>
#include "types.h"
#include "hdc_c.h"

int main(int argc, char **argv) {
    
    // Create new HDC tree
    struct hdc_t* tree = hdc_new_empty();
    
    // Add some children
    hdc_add_child(tree,"aaa/bbb/ccc",hdc_new_empty());
    hdc_add_child(tree,"aaa/bbb/eee",hdc_new_empty());
    hdc_add_child(tree,"bbb/eee/aaa",hdc_new_empty());
    
    // Get subtree
    struct hdc_t* subtree = hdc_get_child(tree,"aaa/bbb");
    
    // Ask whether child exists
    printf("has_child: %d\n", hdc_has_child(tree,"aaa/bbb/ccc"));
    
    // Delete subtree
    hdc_delete_child(tree,"aaa/eee");
    
    // Prepare some data
    int32_t array[4] = {7,2,3,4};
    long int shape[1];
    shape[0] = 4;
    
    // Add data to a single node
    struct hdc_t* data = hdc_new_empty();
    hdc_set_data_int32(data,1,shape,(void*)array);
    
    // Add data to a subtree
    hdc_set_data_int32_path(tree,"aaa/bbb/ccc",1,shape,(void*)array);
    
    // Ask on some data details, use subtree to shorten the path
    int32_t ndim2 = hdc_get_ndim_path(subtree,"ccc");
    long int* shape2 = hdc_get_shape_path(subtree,"ccc");
    printf("Dimension: %d\nShape: ", ndim2);
    for (int i=0; i<ndim2; i++) printf("%d ",shape2[i]);
    printf("\n");
    printf("dtype: %s\n",hdc_get_type_str_path(subtree,"ccc"));
    
    // Get data back from tree
    int32_t* array2 = hdc_as_int32_1d_path(subtree,"ccc");
    printf("Data: ");
    for (int i=0; i<shape2[0]; i++) printf("%d ",array2[i]);
    printf("\n");
    
    //Serialize data to JSON
    hdc_to_json(tree,"tree.txt",0);
    
    return 0;
}