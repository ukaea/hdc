program test_hdc_fortran
    use hdc_fortran
    use iso_c_binding
    implicit none
    call f_main()
contains

    subroutine f_main()
        type(hdc_t), pointer :: tree, subtree, data, node
        integer, dimension(1:4) :: array
        integer, dimension(:), pointer:: array2
        integer(kind=c_long), dimension(1:1) :: shape2
        integer(kind=8) :: ndim2
        allocate(tree)
        allocate(subtree)
        allocate(data)
        allocate(node)
        ! Create new HDC tree
        tree = hdc_new_empty()

        ! Add some children
        call hdc_add_child(tree,"aaa/bbb/ccc",hdc_new_empty())
        call hdc_add_child(tree,"aaa/bbb/eee",hdc_new_empty())
        call hdc_add_child(tree,"bbb/eee/aaa",hdc_new_empty())
        
        ! Get subtree
        subtree = hdc_get_child(tree,"aaa/bbb")
        ! Get node
        node = hdc_get_child(subtree,"ccc")
        
        ! Ask whether child exists
        print *,"has_child: ", hdc_has_child(tree,"aaa/bbb/ccc")

        ! Delete subtree
        call hdc_delete_child(tree,"aaa/eee")

        call hdc_dump(tree)
        
        ! Prepare some data
        array = [7,20,3,4]
        
        ! Add data to a single node
        data = hdc_new_empty()
        call hdc_set_data(data,array)
        
        ! Add data to subtree
        call hdc_set_data(tree,"aaa/bbb/ccc",array)
        
        ! Ask on some data details, use subtree to shorten path
        ndim2 = hdc_get_ndim(node)
        shape2 = hdc_get_shape(node)
        print *,"Dimension: ", ndim2
        print *,"Shape: ", shape2
        call hdc_print_type_str(node)
        call hdc_get(node,array2)
        print *,array2
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        call hdc_set_data(node,3*array2(:))
        call hdc_get(node,array2)
        print *,array2
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        ! Serialize data to JSON
        call hdc_to_json(tree,"tree.txt",0)
        ! test dump
        call hdc_dump(tree)
    end subroutine f_main
    
end program test_hdc_fortran
