! gfortran  -fPIC -g -O0 -ffree-line-length-none -pedantic -fbacktrace -std=f2008 -shared -o hdc_fortran_module.so hdc_fortran_module.f90 -I$PWD/build -L$PWD/build -lfhdc
! module hdc_fortran_module
! contains

    subroutine hello_f_module() bind(c, name="hello_f_module")
        use hdc_fortran
        use iso_c_binding
        implicit none
        print *, 'hello_f_module'
    end subroutine
    subroutine change_data(tree) bind(c, name="change_data")
    use hdc_fortran
    use iso_c_binding
    implicit none

        type(hdc_t), value :: tree
        type(hdc_t) :: node_int, node_double_2d
        integer(kind=c_int8_t), dimension(1:4) :: array
        ! real(kind=dp), dimension(1:4) :: array_of_double
        integer :: i

        ! print *, "Input i: "
        ! read(*,*)i

        print *, "Fortran: hdc_get_child(tree,'group1/int8_data')"
        ! print *, tree%obj
        print *, hdc_has_child(tree, "group1/int8_data")
        node_int = hdc_get_child(tree, "group1/int8_data")

        print *, "Reading data in Fortran"
        array = hdc_get_int8_1d(node_int)
        ! test data get
        print *,"Data: ", array

        array = array + 1
        print *, "Fortran: modified data: ", array
        print *, "Fortran: call hdc_set_data "
        call hdc_set_data(node_int, array)

        ! print *, "Fortran: get_child again"
        ! node_int = hdc_get_child(tree, "group1/int8_data")

        print *, "Reading data in Fortran"
        array = hdc_get_int8_1d(node_int)
        ! test data get
        print *,"Data: ", array

        print *, "Fortran: create group1/double_2d_data"
        node_double_2d = hdc_new_empty()
        call hdc_set_data(node_double_2d, reshape((/1.0d0,2.0d0,3.14d0,5.9d0/),(/2,2/)))
        call hdc_add_child(tree, "group1/double_2d_data", node_double_2d)
        print *,"Fortran: hdc_get_double_2d: ", (hdc_get_double_2d(node_double_2d))

    end subroutine change_data
    
! end module
