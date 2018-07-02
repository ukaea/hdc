program main
  use hdc_fortran
  use iso_c_binding
  implicit none
  call f_main()
contains


    subroutine test_hdc(hdc_out)
        use hdc_fortran
        implicit none
        
        type(hdc_t), intent(out) :: hdc_out
        type(hdc_t) :: hdc

        integer, dimension(1:4) :: array1, array2, array3

        ! allocate(hdc)
        !  allocate(hdc2)

        hdc = hdc_new_empty()

        call hdc_add_child(hdc, "in", hdc_new_empty())
        call hdc_add_child(hdc, "in/arrays", hdc_new_empty())

        call hdc_dump(hdc)

        array1 = [4, 8, 16, 32]
        array2 = [1, 3,  9, 27]
        array3 = [1, 2,  3,  4]

        call hdc_set_data(hdc, "in/arr1", [4.d0, 8.d0, 16.d0, 32.d0, 64.1d0])
        call hdc_set_data(hdc, "in/arr2", array2)
        call hdc_set(hdc, "in/param_d", 3.14d0)
        call hdc_set(hdc, "in/param_i", 333)
        
        call hdc_set_data(hdc, "in/arrays/A1", array3)

        call hdc_dump(hdc)

        write(*,*) "--- Copy to hdc_out ---" 
        call hdc_copy(hdc, hdc_out) 
        
    end subroutine test_hdc


    subroutine f_main()
        ! interface
        !    subroutine hdc_as_double_1d_path_sub(this,path,res)
        !         type(hdc_t) :: this
        !         character(len=*), intent(in) :: path
        !         integer(kind=c_int8_t) :: ndim
        !         integer(kind=c_long), dimension(:), pointer :: shape_
        !         type(c_ptr) :: shape_ptr, data_ptr
        !         real(kind=dp), dimension(:), pointer, intent(inout) :: res
        !     end subroutine hdc_as_double_1d_path_sub
        ! end interface

        
        type(hdc_t) :: hdc_test

        integer    :: i 
        
        integer, dimension(5) :: arr
        integer, dimension(:), pointer  :: arr_p
        !  integer, dimension(:), pointer  :: arr3

        write(*,*) 'Starting testing sequence'

        call test_hdc(hdc_test)

        call hdc_dump(hdc_test)


        i = -1
        !  ii = -1
        if (hdc_exists(hdc_test, "in/param_i")) then
            call hdc_get(hdc_test, "in/param_i", i) ! doesn't work for some reason
            ! call hdc_as_int32_path_sub(hdc_test, "in/param_i", i) ! does work
            i = hdc_as_int32(hdc_test, "in/param_i") ! NO
!             print *, hdc_as_double(hdc_test, "in/param_d")
        end if

        if (hdc_exists(hdc_test, "in/arrays/A1")) then
            call hdc_get(hdc_test, "in/arrays/A1", arr_p)
            ! arr_p = hdc_as_int32_1d(hdc_test, "in/arrays/A1") ! doesn't work ether, even for pointer... 
            ! call hdc_as_double_1d

        !     arr_p = hdc_as_double_1d_path(hdc_test, "in/arrays/A1")
        !     call hdc_as_double_1d_path_sub(hdc_test, "in/arrays/A1", arr)
            
            arr = arr_p

            print *
            print *, arr
            print *, arr_p
            print *
        end if

        write(*,*) 'i = ', i
        !  write(*,*) 'ii = ', ii
    end subroutine f_main

end program main
