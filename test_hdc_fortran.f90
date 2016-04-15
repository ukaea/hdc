program test_hdc_fortran
    use types
    use hdc_fortran
    use iso_c_binding
    implicit none
    call f_main()
contains

    subroutine f_main()
        type(hdc_t) :: tree, n, k, data
        integer(kind=c_int8_t), dimension(1:4) :: array
        integer(kind=c_int8_t), dimension(:),pointer :: aaa

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
        data = hdc_new_empty()
        array = (/1,2,3,4/)
        
        call hdc_set_data(data,array)
        
        ! test data get
        call hdc_as_int8(data,aaa)
        print *, aaa
        
        call hdc_delete(tree)
        
    end subroutine f_main
    
end program test_hdc_fortran