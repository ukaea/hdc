! module hdc_fortran_module
    use types
    use hdc_fortran
    use iso_c_binding
    implicit none

! contains

    subroutine change_data(tree)
        type(hdc_t), intent(inout) :: tree
        type(hdc_t) :: n, k, node_int, node_double, node_double_2d, node_double_ad
        integer(kind=c_int8_t), dimension(1:4) :: array
        real(kind=dp), dimension(1:4) :: array_of_double

        print *, "Fortran: hdc_get_child(tree,'group1/int8_data')"
        node_int = hdc_get_child(tree, "group1/int8_data")
        
        print *, "Reading data in Fortran"
        array = hdc_get_int8_1d(node_int)
        ! test data get
        print *,"Data: ", array

        array = array * 2
        print *, "Fortran: modified data: ", array
        print *, "Fortran: call hdc_set_data "
        call hdc_set_data(node_int, array)
        
        print *, "Fortran: create group1/double_2d_data"
        node_double_2d = hdc_new_empty()
        call hdc_set_data(node_double_2d, reshape((/1.0d0,2.0d0,3.14d0,5.9d0/),(/2,2/)))
        call hdc_add_child(tree, "group1/double_2d_data", node_double_2d)
        print *,"Fortran: hdc_get_double_2d: ", (hdc_get_double_2d(node_double_2d))
        
        
    end subroutine change_data
    
! end module
