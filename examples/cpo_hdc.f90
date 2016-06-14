program cpo_hdc
    use hdc_fortran
    use iso_c_binding
    implicit none

    type(hdc_t) :: equilibrium, distsource
    equilibrium = hdc_new_empty()
    call hdc_set(equilibrium, 'time', 1.1d0)
    call hdc_set(equilibrium, 'profiles_1d/psi', (/0.1d0, 0.2d0, 0.3d0/))
    call test_cpos(equilibrium, distsource)
contains

    subroutine test_cpos(equilibriumin, distsourceout)
        integer :: i
        !UAL ! Always describe cpo as array 
        !UAL ! In case of time slice, the size of the input cpo is 1
        !UAL type (type_equilibrium),pointer :: equilibriumin(:)
        !UAL type (type_distsource),pointer :: distsourceout(:)

        !HDC arrays (of CPO's) are internal in HDC containers
        type(hdc_t), intent(in) :: equilibriumin
        type(hdc_t), intent(out) :: distsourceout
        type(hdc_t) :: distsource_i, equilibrium_i
        real(kind=DP), dimension(:), pointer :: psi

        !UAL write(0,*) 'size of input CPO = ',size(equilibriumin)
        write(0,*) 'size of input CPO = ', hdc_get_shape(equilibriumin)

        !UAL allocate(distsourceout(size(equilibriumin)))
        !HDC we explicitely create a new container
        distsourceout = hdc_new_empty()
        !HDC resize will create empty containers
        !call hdc_resize(distsourceout, source=equilibriumin) resize is not in dynd

        call hdc_copy(equilibriumin, distsourceout)

        !UAL do i=1,size(equilibriumin)
        !HDC we assume here that ndim = 1

        equilibrium_i = equilibriumin
        distsource_i = distsourceout
        do i=1,hdc_get_shape(equilibriumin,1)
            !UAL write(0,*) 'Received input time from equilibrium : ', equilibriumin(i)%time
            !HDC We probably need get_slice to avoid complicated string operations in fortran
            equilibrium_i = hdc_get_slice(equilibriumin, i)
            distsource_i = hdc_get_slice(distsourceout, i)
            !HDC this needs hdc_t write / format support -- HOW?
            write(0,*) 'Received input time from equilibrium: ', hdc_as_double(equilibrium_i, 'time')
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

            call hdc_set(distsource_i, 'source/profiles_1d/psi', 2 * hdc_as_double_1d(equilibrium_i, 'profiles_1d/psi'))
            ! or
            call hdc_get(equilibrium_i, 'profiles_1d/psi',psi)
            call hdc_set(distsource_i, 'source/profiles_1d/psi', 2 * psi)

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

            write(*,*)'DUMP: '
            call hdc_dump(distsource_i)


! ! !             !HDC User (code-specific) data (in JSON document)
! ! !             hdc_set_json(distsource_i, 'userspace', '{"params": {"string": "my params", "date": "2016-04-14T17:13:37.101511", "numbers": [1, 2, 3.5]}}')

        enddo
    end subroutine test_cpos

end program cpo_hdc