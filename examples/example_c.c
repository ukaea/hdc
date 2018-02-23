#include <stdio.h>
#include "hdc_types.h"
#include "hdc_c.h"

struct hdc_t* init_hdc_data();
void manipulate(struct hdc_t* data);

int main() {
    HDC_init_c("","");
    // init data
    struct hdc_t* tree = init_hdc_data();

    // Modify the data
    manipulate(tree);

    //Serialize data to JSON
    hdc_to_json(tree,"tree.txt",0);

    // Dump to screen
    printf("Final dump:\n");
    hdc_dump(tree);
    return 0;
}

struct hdc_t* init_hdc_data() {
    // Create new HDC tree
    struct hdc_t* tree = hdc_new_empty();

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
    struct hdc_t* subtree = hdc_get(tree,"groupA/data");
    // Get node
    struct hdc_t* node = hdc_get(subtree,"int_array");

    // Ask whether child exists
    printf("has_child: %d (should be true)\n", hdc_has_child(tree, "groupA/data/int_array"));
    printf("has_child: %d (should be false)\n", hdc_has_child(tree, "groupA/data/this_does_not_exist"));

    // Delete node
    hdc_delete_child(tree,"GroupA/data/junk");
    // Delete subtree
    hdc_delete_child(tree,"GroupB/data");

    // Prepare some data
    int32_t array[4] = {7,2,3,4};
    size_t shape[1];
    shape[0] = 4;

    // Add data to a single node
    struct hdc_t* data = hdc_new_empty();
    hdc_set_int32(data,1,shape,(void*)array);

    // Add data to a subtree
    hdc_set_int32_path(tree,"groupA/data/int_array",1,shape,(void*)array);

    // Ask on some data details, use subtree to shorten the path
    int8_t ndim2 = hdc_get_ndim(node);
    size_t* shape2 = hdc_get_shape(node);
    printf("Dimension: %d\nShape: ", ndim2);
    for (int8_t i=0; i<ndim2; i++) printf("%ld ",shape2[i]);
    printf("\n");
    printf("dtype: %s\n",hdc_get_type_str(node));

    // Get data back from tree
    int32_t* array2 = hdc_as_int32_1d(node);
    printf("Data: ");
    for (size_t i=0; i<shape2[0]; i++) printf("%d ",array2[i]);
    printf("\n");

    return tree;

}

void manipulate(struct hdc_t* data) {
    printf("-------------------\n");
    printf("Entering manipulate\n");
    printf("Input data:\n");
    hdc_dump(data);
    // Get the HDC node
    struct hdc_t* node = hdc_get(data, "groupA/data/int_array");
    // Get the pointer to the data
    int32_t* array2 = hdc_as_int32_1d(node);
    // and shape (assime ndim = 1)
    size_t* shape2 = hdc_get_shape(node);
    // Multiply by 2
    for (size_t i=0; i<shape2[0]; i++)
        array2[i] *= 2;
    printf("Output data:\n");
    hdc_dump(data);
    printf("-------------------\n");

}
