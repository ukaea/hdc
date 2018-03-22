subroutine test_cpos_hdc(data_in, data_out) bind(c, name="test_cpos_hdc")
    use hdc_fortran
    use iso_c_binding
    
    use hdc_fortran_test_module
    implicit none
    ! declare input and output containers
    type(hdc_t), value :: data_in, data_out
    ! declare all input and output arguments
    
    type(hdc_t) :: equilibriumin
    integer :: int_in
    type(hdc_t) :: distsourceout
    real(8) :: float_out
    
    write(*,*)'=== test_cpos_hdc ==='
    call hdc_dump(data_in)
    
    call hdc_get(data_in, 'equilibriumin', equilibriumin)
    call hdc_get(data_in, 'int_in', int_in)

    call test_cpos(equilibriumin, int_in, distsourceout, float_out)

    
    call hdc_add_child(data_out, 'equilibriumin', equilibriumin)
    call hdc_add_child(data_out, 'distsourceout', distsourceout)
    call hdc_add_child(data_out, 'float_out', float_out)

    write(*,*)'output tree:'
    call hdc_dump(data_out)

    write(*,*)'=== END test_cpos ==='
end subroutine