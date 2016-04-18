program test_hdc_fortran
    use types
    use hdc_fortran
    use iso_c_binding
    implicit none
    call f_main()
contains

    subroutine f_main()
        type(hdc_t) :: tree, n, k, node_int, node_double, node_double_2d
        integer(kind=c_int8_t), dimension(1:4) :: array
        real(kind=dp), dimension(1:4) :: array_of_double

        call hello()
            
        tree = hdc_new_empty()   
        ! test add
        call hdc_add_child(tree,"aaa/bbb/ccc",hdc_new_empty())
        call hdc_add_child(tree,"aaa/bbb/eee",hdc_new_empty())
        call hdc_add_child(tree,"bbb/eee/aaa",hdc_new_empty())
        
        ! test get
        n = hdc_get_child(tree,"aaa/bbb")
        k = hdc_get_child(tree,"ccc")
        
        ! test has
        print *,"has_child: ", hdc_has_child(tree,"aaa/bbb/ccc")
        print *,"has_child: ", hdc_has_child(tree,"aaa/bbb/ddd")
        print *,"has_child: ", hdc_has_child(tree,"aaa/bbb")

        ! test delete
        call hdc_delete_child(tree,"aaa/bbb")
        print *,"has_child: ", hdc_has_child(tree,"aaa/bbb")
            
        ! test set
        call hdc_set_child(tree,"bbb/eee/aaa",hdc_new_empty())
        call hdc_set_child(tree,"bbb",hdc_new_empty())
        print *,"has_child: ", hdc_has_child(tree,"bbb/eee/aaa")
        
        ! test data_set
        node_int = hdc_new_empty()
        array = (/1,2,3,4/)
        
        call hdc_set_data(node_int,array)    
        
        ! test data get
        print *,"DATA", hdc_get_int8(node_int)
        
        node_double = hdc_new_empty()
        call hdc_set_data(node_double,(/1.0d0,2.0d0/))
        print *,"1D DATA", (hdc_get_double_1d(node_double))
        node_double_2d = hdc_new_empty()
        call hdc_set_data(node_double_2d,reshape((/1.0d0,2.0d0,3.14d0,5.9d0/),(/2,2/)))
        print *,"2D DATA", (hdc_get_double_2d(node_double_2d))
        
        call hdc_delete(tree)
        
    end subroutine f_main
    
end program test_hdc_fortran