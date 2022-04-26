subroutine process_data(ids_bundle, tree_out) bind(c, name="process_data")
    use hdc_fortran
    use iso_c_binding

    implicit none

    type(hdc_t), value :: ids_bundle
    type(hdc_t), value :: tree_out

    real(8), dimension(:), pointer :: ti, te, time
    type(hdc_t) :: prof

    call hdc_get(ids_bundle, 'coreprof/time', time)
    call hdc_get(ids_bundle, 'coreprof/profiles_1d[30]', prof)
    call hdc_get(prof, 't_i_average', ti)
    call hdc_get(prof, 'electrons/temperature', te)

    call hdc_set(tree_out, 'output/tite', ti / te)

end subroutine
