! gfortran  -fPIC -g -O0 -ffree-line-length-none -pedantic -fbacktrace -std=f2008 -shared -o hdc_fortran_module.so hdc_fortran_module.f90 -I$PWD/build -L$PWD/build -lfhdc
! module hdc_fortran_module
! contains

subroutine hello_f() bind(c, name="hello_f")
    use iso_c_binding
    print *, "--- Hello from FORTRAN"
end subroutine hello_f

! TODO: make regular tests from this!

subroutine test_hdc_modify(tree_out) bind(c, name="test_hdc_modify")
    use hdc_fortran
    use iso_c_binding
    implicit none
    type(hdc_t), value :: tree_out
    integer, parameter :: nx = 2, ny = 3
    real(kind=DP) :: array(nx,ny)
    ! init array first
    array(:,:) = 9.99_dp
    array(2,2) = 1.000001_dp
    call hdc_set(tree_out,array)
    call hdc_print_info(tree_out)
    call hdc_dump(tree_out)
end subroutine test_hdc_modify

subroutine test_hdc_dump(tree_in) bind(c, name="test_hdc_dump")
    use hdc_fortran
    use iso_c_binding
    implicit none
    type(hdc_t), value :: tree_in
    call hdc_print_info(tree_in)
    call hdc_dump(tree_in)
end subroutine test_hdc_dump

function test_hdc_create() bind(c, name="test_hdc_create") result(res)
    use hdc_fortran
    use iso_c_binding
    implicit none
    type(hdc_t) :: res
    integer, parameter :: nx = 2, ny = 3
    real(kind=DP) :: array(nx,ny)
    ! init array first
    array(:,:) = 2.22_dp
    array(2,2) = 3.14159_dp
    res = hdc_new()
    call hdc_set(res,array)
end function test_hdc_create


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
    !HDC we assume here that rank = 1

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
! !             call hdc_set(distsource_i, 'source/profiles_1d/psi', 2 * hdc_get_child(equilibrium_i, 'profiles_1d/psi'))
        !HDC The alternative, more explicit solutions are
        !HDC it seems that hdc_get_<type> functions will be handy in Fortran
! read *
! print *,hdc_as_double_1d(equilibrium_i, 'profiles_1d/psi')
! read *

        write(*,*)'set distsource_i/source/profiles_1d/psi'
        ! or
        write(*,*)'get equilibrium_i/profiles_1d/psi'
        call hdc_get(equilibrium_i, 'profiles_1d/psi', psi)
        ! psi = hdc_as_double(equilibrium_i, 'profiles_1d/psi')
        write(*,*)'psi=', psi
        write(*,*)'set distsource_i/source/profiles_1d/psi'

        ! call hdc_set(distsource_i, 'source/profiles_1d/psi', 2 * hdc_as_double_1d(equilibrium_i, 'profiles_1d/psi'))
        psi_test = (/-1.d0, -2.d0, 3.d0, 4.4d0/)
        call hdc_set(distsource_i, 'source/profiles_1d/psi', -3 * psi(:))
        ! call hdc_set(distsource_i, 'source/profiles_1d/psi', psi_test)
        ! call hdc_set(distsource_i, 'source/profiles_1d/psi', psi)

        call hdc_get(distsource_i, 'source/profiles_1d/psi', psi)
        ! psi = hdc_as_double(equilibrium_i, 'profiles_1d/psi')
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


! end module
