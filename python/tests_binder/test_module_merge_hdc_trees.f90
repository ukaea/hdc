module test_module_merge_hdc_trees
contains

subroutine merge_hdc_trees(tree1, tree2, tree_out)
    use iso_c_binding
    use hdc_fortran
    implicit none
    type(hdc_t), intent(in) :: tree1, tree2
    type(hdc_t), intent(inout) :: tree_out
    call hdc_dump(tree_out)
    call hdc_add_child(tree_out,"tree1",tree1)
    call hdc_add_child(tree_out,"tree2",tree2)
end subroutine merge_hdc_trees

end module test_module_merge_hdc_trees
