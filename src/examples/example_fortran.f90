program test_hdc_fortran
    use hdc_fortran
    use iso_c_binding
    implicit none
    call f_main()
contains

    subroutine f_main()
        type(hdc_t) :: tree, subtree, data
        integer, dimension(1:4) :: array
        integer, dimension(:), pointer:: array2
        integer :: ndim2, shape2

        ! Create new HDC tree
        tree = hdc_new_empty()

        ! Add some children
        call hdc_add_child(tree,"aaa/bbb/ccc",hdc_new_empty())
        call hdc_add_child(tree,"aaa/bbb/eee",hdc_new_empty())
        call hdc_add_child(tree,"bbb/eee/aaa",hdc_new_empty())
        
        ! Get subtree
        subtree = hdc_get_child(tree,"aaa/bbb")
        
        ! Ask whether child exists
        print *,"has_child: ", hdc_has_child(tree,"aaa/bbb/ccc")

        ! Delete subtree
        call hdc_delete_child(tree,"aaa/eee")

        call hdc_dump(tree)
        
        ! Prepare some data
        array = (/7,2,3,4/)
        
!         ! Add data to a single node
!         data = hdc_new_empty()
!         call hdc_set_data(data,array)
!         
!         ! Add data to subtree
!         call hdc_set_data(tree,"aaa/bbb/ccc",array)
!         
!         ! Ask on some data details, use subtree to shorten path
!         ndim2 = hdc_get_ndim(subtree,"ccc")
!         shape2 = hdc_get_shape(subtree,"ccc")
!         print *,"Dimension: ", ndim2
!         print *,"Shape: ", shape2
!         print *, "dtype: ", hdc_get_type_str

!BIG TODO

    end subroutine f_main
    
end program test_hdc_fortran