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
    array = hdc_as_int8_1d(node_int)
    ! test data get
    print *,"Data: ", array

    array = array + 1
    print *, "Fortran: modified data: ", array
    print *, "Fortran: call hdc_set_data "
    call hdc_set_data(node_int, array)

    ! print *, "Fortran: get_child again"
    ! node_int = hdc_get_child(tree, "group1/int8_data")

    print *, "Reading data in Fortran"
    array = hdc_as_int8_1d(node_int)
    ! test data get
    print *,"Data: ", array

    print *, "Fortran: create group1/double_2d_data"
    node_double_2d = hdc_new_empty()
    call hdc_set_data(node_double_2d, reshape((/1.0d0,2.0d0,3.14d0,5.9d0/),(/2,2/)))
    call hdc_add_child(tree, "group1/double_2d_data", node_double_2d)
    print *,"Fortran: hdc_as_double_2d: ", (hdc_as_double_2d(node_double_2d))

end subroutine change_data


subroutine c_test_cpos(equilibriumin) bind(c, name="c_test_cpos")
    use hdc_fortran
    use iso_c_binding
    implicit none
    ! type(hdc_t), value :: equilibriumin
    type(hdc_t), value :: equilibriumin
    ! type(hdc_t_p) :: distsourceout
    type(hdc_t) :: distsourceout
    real(kind=DP), dimension(:), pointer :: psi
    real(8) :: time

    write(*,*)'=== c_test_cpos ==='
    call hdc_dump(equilibriumin)

    write(0,*) 'size of input CPO = ', hdc_get_shape(equilibriumin)
    write(0,*) 'Received input time from equilibrium: ', hdc_as_double(equilibriumin, 'time')

    time = hdc_as_double(equilibriumin, 'time')
    ! call hdc_get(equilibriumin, 'time', time)
    write(0, *)'timein', time

    ! call hdc_get(equilibriumin, 'profiles_1d_psi', psi)
    ! write(*,*)'psi = ', psi

    write(*,*)'get equilibriumin/profiles_1d_psi'
    call hdc_get(equilibriumin, 'profiles_1d_psi', psi)
    ! psi => hdc_as_double_1d(equilibriumin, 'profiles_1d_psi')
    write(*,*)psi

    call test_cpos(equilibriumin, distsourceout)

    write(*,*)'distsourceout in c_test_cpos'
    call hdc_dump(distsourceout)

    write(*,*)'=== END c_test_cpos ==='
end subroutine


subroutine test_cpos_f2c(equilibriumin, tree_out) bind(c, name="test_cpos_f2c")
    use hdc_fortran
    use iso_c_binding
    implicit none
    ! type(hdc_t), value :: equilibriumin
    type(hdc_t), value :: equilibriumin, tree_out
    type(hdc_t) :: distsourceout
    


    write(*,*)'=== test_cpos_f2c START ==='
    write(*,*)'equilibriumin'
    call hdc_dump(equilibriumin)

    write(*,*)'--- call test_cpos ---'
    call test_cpos(equilibriumin, distsourceout)

    write(*,*)'--- set output ---'
    ! call hdc_set(tree,_out 'distsourceout', distsourceout)
    call hdc_add_child(tree_out, 'distsourceout', distsourceout)

    write(*,*)'output tree:'
    call hdc_dump(tree_out)
    call hdc_to_json(tree_out, "./tree_out.json", 0)

    write(*,*)'=== END test_cpos_f2c ==='
end subroutine


subroutine test_cpos(equilibriumin, distsourceout) 
    use hdc_fortran
    use iso_c_binding
    implicit none

    integer :: i


    !UAL ! Always describe cpo as array 
    !UAL ! In case of time slice, the size of the input cpo is 1
    !UAL type (type_equilibrium),pointer :: equilibriumin(:)
    !UAL type (type_distsource),pointer :: distsourceout(:)

    !HDC arrays (of CPO's) are internal in HDC containers
    type(hdc_t), intent(in) :: equilibriumin
    type(hdc_t), intent(inout) :: distsourceout
    ! type(hdc_t), value :: equilibriumin
    ! type(hdc_t), value :: distsourceout
    type(hdc_t) :: distsource_i, equilibrium_i
    real(kind=DP), dimension(:), pointer :: psi
    ! real(kind=DP), dimension(:):: psi
    real(kind=DP), dimension(4) :: psi_test
    real(kind=DP) :: time

    !UAL write(0,*) 'size of input CPO = ',size(equilibriumin)
    write(*,*)' == test_cpos starts =='

    write(0,*) 'size of input CPO = ', hdc_get_shape(equilibriumin)
    write(0,*) '---'

    !UAL allocate(distsourceout(size(equilibriumin)))
    !HDC we explicitely create a new container

    ! distsourceout = hdc_new_empty()

    !HDC resize will create empty containers
    !call hdc_resize(distsourceout, source=equilibriumin) resize is not in dynd

    ! call hdc_get(equilibrium_i, 'time', time)
    write(0, *) '-- get time ---'
    time = hdc_as_double(equilibriumin, 'time')
    write(0, *) time

    write(0, *) '-- before hdc_copy ---'
    call hdc_copy(equilibriumin, distsourceout)
    write(0, *) '-- hdc_copy done ---'

    !UAL do i=1,size(equilibriumin)
    !HDC we assume here that ndim = 1

    equilibrium_i = equilibriumin
    distsource_i = distsourceout

    i = 1
    ! do i=1,hdc_get_shape(equilibriumin,1)

        !UAL write(0,*) 'Received input time from equilibrium : ', equilibriumin(i)%time
        !HDC We probably need get_slice to avoid complicated string operations in fortran

        ! equilibrium_i = hdc_get_slice(equilibriumin, i)
        ! distsource_i = hdc_get_slice(distsourceout, i)

        !HDC this needs hdc_t write / format support -- HOW?
        ! write(0,*) 'Received input time from equilibrium: ', hdc_as_double(equilibrium_i, 'time')
        
        ! call hdc_get(equilibrium_i, 'time', time)
        time = hdc_as_double(equilibrium_i, 'time')
        write(0,*) 'Received input time from equilibrium: ', time
        !UAL distsourceout(i)%time = 10 + equilibriumin(i)%time
        !HDC Here, we do not need to think about data types (unless we want to do it explicitely)
        call hdc_set(distsource_i, 'time', hdc_as_double(equilibrium_i, 'time'))

        !UAL allocate(distsourceout(i)%source(1))
        !UAL allocate(distsourceout(i)%source(1)%profiles_1d%psi, source=equilibriumin(i)%profiles_1d%psi)
        !UAL allocate(distsourceout(i)%source(1)%profiles_1d%psi(size(equilibriumin(i)%profiles_1d%psi)))
        !UAL distsourceout(i)%source(1)%profiles_1d%psi = 2 * equilibriumin(i)%profiles_1d%psi

        !HDC In HDC, we can skip the allocate statements, it will populate the tree as necessary
        !HDC This is a bit magic as we have to implemet * operator for hdc_t -- Do we want to do this?
! !             call hdc_set(distsource_i, 'source/profiles_1d_psi', 2 * hdc_get_child(equilibrium_i, 'profiles_1d_psi'))
        !HDC The alternative, more explicit solutions are
        !HDC it seems that hdc_get_<type> functions will be handy in Fortran
! read *
! print *,hdc_as_double_1d(equilibrium_i, 'profiles_1d_psi')
! read *

        write(*,*)'set distsource_i/source/profiles_1d_psi'
        ! or
        write(*,*)'get equilibrium_i/profiles_1d_psi'
        call hdc_get(equilibrium_i, 'profiles_1d_psi', psi)
        ! psi = hdc_as_double(equilibrium_i, 'profiles_1d_psi')
        write(*,*)'psi=', psi
        write(*,*)'set distsource_i/source/profiles_1d_psi'

        ! call hdc_set(distsource_i, 'source/profiles_1d_psi', 2 * hdc_as_double_1d(equilibrium_i, 'profiles_1d_psi'))
        psi_test = (/-1.d0, -2.d0, 3.d0, 4.4d0/)
        ! call hdc_set(distsource_i, 'source/profiles_1d_psi', -3 * psi(:))  ! doesn work
        ! call hdc_set(distsource_i, 'source/profiles_1d_psi', psi_test)  ! this works only in current scope (subroutine / function)
        call hdc_set(distsource_i, 'source/profiles_1d_psi', psi)  ! this works as expected
        write(*,*) 2*psi

        call hdc_get(distsource_i, 'source/profiles_1d_psi', psi)
        ! psi = hdc_as_double(equilibrium_i, 'profiles_1d_psi')
        ! write(*,*)'psi (distsource_i) =', psi


        !UAL allocate(distsourceout(i)%codeparam%codename(1))   ! For a string of 132 characters max.
        !UAL distsourceout(i)%codeparam%codename(1)   = 'equ2dist'
        !UAL allocate(distsourceout(i)%codeparam%codeversion(1))   ! For a string of 132 characters max.
        !UAL distsourceout(i)%codeparam%codeversion(1)   = '1.0'
        !UAL allocate(distsourceout(i)%codeparam%parameters(1))   ! For a string of 132 characters max.
        !UAL distsourceout(i)%codeparam%parameters(1) = 'my_code_specific_parameters'
        !UAL allocate(distsourceout(i)%codeparam%output_diag(1))   ! For a string of 132 characters max.
        !UAL distsourceout(i)%codeparam%output_diag(1) = 'Made in Python'
        !UAL distsourceout(i)%codeparam%output_flag = 1   ! Integer output flag, 0 means the run was successful and can be used in the rest of the workflow, <0 means failure

        call hdc_set(distsource_i, 'codeparam/codename', 'equ2dist')
        call hdc_set(distsource_i, 'codeparam/codeversion', '1.0')
        call hdc_set(distsource_i, 'codeparam/parameters', 'my_code_specific_parameters')
        call hdc_set(distsource_i, 'codeparam/output_diag', 'Made in Python')
        call hdc_set(distsource_i, 'codeparam/output_flag', 0)

        call hdc_dump(distsource_i)

! ! !             !HDC User (code-specific) data (in JSON document)
! ! !             hdc_set_json(distsource_i, 'userspace', '{"params": {"string": "my params", "date": "2016-04-14T17:13:37.101511", "numbers": [1, 2, 3.5]}}')

    ! enddo

    write(*,*)'=== END test_cpos ==='

end subroutine test_cpos


subroutine test_test_cpo_hdc()
    use hdc_fortran
    use iso_c_binding
    implicit none

    real(8), pointer, dimension(:) :: time

    type(hdc_t) :: equilibrium, distsource

    equilibrium = hdc_new_empty()
    call hdc_set(equilibrium, 'time', 1.1d0)
    write(*,*)'hdc_get'
    ! call hdc_get(equilibrium, 'time', time)
    write(*,*)hdc_as_double(equilibrium, 'time')
    call hdc_set(equilibrium, 'profiles_1d_psi', (/0.1d0, 0.2d0, 0.3d0/))
    write(*,*)'call test_cpos'
    call test_cpos(equilibrium, distsource)
end subroutine

    
! end module
