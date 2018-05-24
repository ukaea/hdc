subroutine process_data(tree_in, tree_out) bind(c, name="process_data")
    use hdc_fortran
    use iso_c_binding

    implicit none

    type(hdc_t), value :: tree_in
    type(hdc_t), value :: tree_out

    real(4), dimension(:), pointer :: Ip

    call hdc_get(tree_in, "Ip/data", Ip)
    call hdc_set(tree_out, 'Ip/max', maxval(Ip))
    call hdc_set(tree_out, 'Ip/min', minval(Ip))

end subroutine


subroutine process_data_inplace(tree_in) bind(c, name="process_data_inplace")
    use hdc_fortran
    use iso_c_binding

    implicit none

    type(hdc_t), value :: tree_in

    real(4), dimension(:), pointer :: Ip

    call hdc_get(tree_in, "Ip/data", Ip)
    call hdc_set(tree_in, 'Ip/max', maxval(Ip))
    call hdc_set(tree_in, 'Ip/min', minval(Ip))
    ! normalize Ip
    Ip = (Ip - minval(Ip)) / (maxval(Ip) - minval(Ip))

end subroutine
