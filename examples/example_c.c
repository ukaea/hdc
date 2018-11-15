#include <stdio.h>
#include "hdc_types.h"
#include "hdc_c.h"

hdc_t init_hdc_data();
void manipulate(hdc_t data);

int main() {
    hdc_init("","");
    // init data
    hdc_t tree = init_hdc_data();

    // Modify the data
    manipulate(tree);

    //Serialize data to JSON
    hdc_to_json(tree,"tree.txt",0);

    // Dump to screen
    printf("Final dump:\n");
    hdc_dump(tree);
//     hdc_t h = hdc_new_empty();

//     double array_in[4] = {1.1,2.2,3.3,4.4};

// // //     struct hdc_data_t data_in;
// // //     memset(&data_in, 0, sizeof(struct hdc_data_t));
// // //     data_in.rank = 1;
// // //     data_in.shape[0] = 4;
// // //     data_in.type = HDC_DOUBLE;
// // //     data_in.flags = HDCDefault;
// // //     data_in.data = (char*)(&array_in);
// // //     hdc_set_data(h, "", data_in);
// // //     hdc_dump(h);
// // //     struct hdc_data_t data_out = hdc_get_data(h, "");
// // //     for (size_t i=0; i<HDC_MAX_DIMS; i++) printf("%d == %d\n",data_in.shape[i] == data_out.shape[i]);
// // //
// // //     double* array_out = (double*)(data_out.data);
// // //     for (size_t i=0; i<4; i++) CHECK(array_in[i] == array_out[i]);

    hdc_destroy();
    return 0;
}

 hdc_t init_hdc_data() {
    // Create new HDC tree
    hdc_t tree = hdc_new_empty();

    // Add some children
    hdc_add_child(tree,"groupA/data/int_array",hdc_new_empty());
    hdc_add_child(tree,"groupA/data/junk",hdc_new_empty());
    hdc_add_child(tree,"GroupB/data/empty",hdc_new_empty());
    // Test hdc_keys() here:
    /*
    size_t n;
    char** keys = hdc_keys(tree,&n);
    for (size_t i=0;i<n;i++) printf("%s ",keys[i]);
    printf("\n");
    */
    // Get subtree
    hdc_t subtree = hdc_get(tree,"groupA/data");
    // Get node
    hdc_t node = hdc_get(subtree,"int_array");

    // Ask whether child exists
    printf("exists: %d (should be true)\n", hdc_exists(tree, "groupA/data/int_array"));
    printf("exists: %d (should be false)\n", hdc_exists(tree, "groupA/data/this_does_not_exist"));

    // Delete node
    hdc_delete_child(tree,"GroupA/data/junk");
    // Delete subtree
    hdc_delete_child(tree,"GroupB/data");

    // Prepare some data
    int32_t array[4] = {7,2,3,4};
    size_t shape[1];
    shape[0] = 4;

    // Add data to a single node
    hdc_t data = hdc_new_empty();
    hdc_set_int32(data,"",1,shape,(void*)array,0);

    // Add data to a subtree
    hdc_set_int32(tree,"groupA/data/int_array",1,shape,(void*)array,HDCDefault);

    // Ask on some data details, use subtree to shorten the path
    struct hdc_data_t data2 = hdc_get_data(node,"");
    printf("Dimension: %ld\nShape: ", data2.rank);
    for (size_t i=0; i<data2.rank; i++) printf("%ld ",data2.shape[i]);
    printf("\n");
    printf("dtype: %s\n",hdc_get_type_str(node,""));

    // Get data back from tree
    // int32_t* array2 = hdc_as_int32_array(node,"");
    // Or you can use this:
    int32_t* array2 = (int32_t*)data2.data;
    printf("Data: ");
    for (size_t i=0; i<data2.shape[0]; i++) printf("%d ",array2[i]);
    printf("\n");

    return tree;

}

void manipulate(hdc_t data) {
    printf("-------------------\n");
    printf("Entering manipulate\n");
    printf("Input data:\n");
    hdc_dump(data);
    // Get the HDC node
    hdc_t node = hdc_get(data, "groupA/data/int_array");
    // Get the pointer to the data
    int32_t* array2 = hdc_as_int32_array(node,"");
    struct hdc_data_t data2 = hdc_get_data(node,"");
    // Multiply by 2
    for (size_t i=0; i<data2.shape[0]; i++)
        array2[i] *= 2;
    printf("Output data:\n");
    hdc_dump(data);
    printf("-------------------\n");

}
