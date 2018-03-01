module c_perf
  use iso_c_binding
  implicit none

  interface
     subroutine c_getMillis(t) bind(c,name='getMillis')
       use iso_c_binding
       integer(kind=c_long_long), intent(out) :: t
     end subroutine c_getMillis
  end interface

end module c_perf


program hdc_benchmark_put_all
    use hdc_fortran
    use c_perf
    implicit none
    call f_main()
contains
    subroutine f_main()

        type(hdc_t), pointer :: magnetics
        integer              :: idx, shot, run, refshot, refrun, status, i
        integer              :: j, k, n, k2, SIGNAL_LENGTH, NSTATS
        integer(kind=8)      :: t1,t2
        real(4), pointer     :: data_in(:), data_out(:)
        CHARACTER(len=132)   :: string_in
        CHARACTER(len=255)   :: string_out
        integer, parameter   :: out_unit=20
        CHARACTER(LEN=30)    :: Format
        integer(kind=c_int32_t), dimension(1:1) :: int0array_in
        real(8), dimension(1:1) :: float0array_in
        complex(8), dimension(1:1) :: complex0array_in
        integer(kind=c_int32_t), pointer :: int0array_out(:)
        real(8), pointer :: float0array_out(:)
        complex(8), pointer :: complex0array_out(:)
        NSTATS = 1000

        do k = 10, 100001, 1000
            SIGNAL_LENGTH = k
            allocate(data_in(SIGNAL_LENGTH),data_out(SIGNAL_LENGTH))
            do i = 1,SIGNAL_LENGTH
                data_in(i) = float(2*i)
            end do
            allocate(magnetics)
            magnetics = hdc_new_empty()

            call c_getMillis(t1)
            do k2 = 1,NSTATS
                call hdc_set(magnetics,"ids_properties/comment",string_in)
                call hdc_set(magnetics,"ids_properties/homogeneous_time",int0array_in)
                call hdc_set(magnetics,"flux_loop[0]/name",string_in)
                call hdc_set(magnetics,"flux_loop[0]/identifier",string_in)
                call hdc_set(magnetics,"flux_loop[0]/position[0]/r",float0array_in)
                call hdc_set(magnetics,"flux_loop[0]/position[0]/r_error_upper",float0array_in)
                call hdc_set(magnetics,"flux_loop[0]/position[0]/r_error_lower",float0array_in)
                call hdc_set(magnetics,"flux_loop[0]/position[0]/z",float0array_in)
                call hdc_set(magnetics,"flux_loop[0]/position[0]/z_error_upper",float0array_in)
                call hdc_set(magnetics,"flux_loop[0]/position[0]/z_error_lower",float0array_in)
                call hdc_set(magnetics,"flux_loop[0]/position[0]/phi",float0array_in)
                call hdc_set(magnetics,"flux_loop[0]/position[0]/phi_error_upper",float0array_in)
                call hdc_set(magnetics,"flux_loop[0]/position[0]/phi_error_lower",float0array_in)
                call hdc_set(magnetics,"flux_loop[0]/flux/data",data_in)
                call hdc_set(magnetics,"flux_loop[0]/flux/data_error_upper",data_in)
                call hdc_set(magnetics,"flux_loop[0]/flux/data_error_lower",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/name",string_in)
                call hdc_set(magnetics,"bpol_probe[0]/identifier",string_in)
                call hdc_set(magnetics,"bpol_probe[0]/position/r",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/position/r_error_upper",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/position/r_error_lower",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/position/z",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/position/z_error_upper",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/position/z_error_lower",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/position/phi",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/position/phi_error_upper",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/position/phi_error_lower",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/poloidal_angle",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/poloidal_angle_error_upper",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/poloidal_angle_error_lower",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/toroidal_angle",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/toroidal_angle_error_upper",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/toroidal_angle_error_lower",float0array_in)
                call hdc_set(magnetics,"bpol_probe[0]/area",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/area_error_upper",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/area_error_lower",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/lenght",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/lenght_error_upper",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/lenght_error_lower",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/lenght_error_turns",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/field/data",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/field/data_error_upper",data_in)
                call hdc_set(magnetics,"bpol_probe[0]/field/data_error_lower",data_in)
                call hdc_set(magnetics,"method[0]/name",string_in)
                call hdc_set(magnetics,"method[0]/ip/data",data_in)
                call hdc_set(magnetics,"method[0]/ip/data_error_upper",data_in)
                call hdc_set(magnetics,"method[0]/ip/data_error_lower",data_in)
                call hdc_set(magnetics,"method[0]/diamagnetic_flux/data",data_in)
                call hdc_set(magnetics,"method[0]/diamagnetic_flux/data_error_upper",data_in)
                call hdc_set(magnetics,"method[0]/diamagnetic_flux/data_error_lower",data_in)
                call hdc_set(magnetics,"code/name",string_in)
                call hdc_set(magnetics,"code/commit",string_in)
                call hdc_set(magnetics,"code/version",string_in)
                call hdc_set(magnetics,"code/repository",string_in)
                call hdc_set(magnetics,"code/parameters",string_in)
                call hdc_set(magnetics,"code/output_flag",float0array_in)
                call hdc_set(magnetics,"time",10.0)
            end do
            call c_getMillis(t2)
            Format = "(I8.8, EN15.5)"
    ! 	    write (out_unit,Format) SIGNAL_LENGTH, (t2-t1)*1e-3/float(NSTATS)
            write (*,Format) SIGNAL_LENGTH, (t2-t1)*1e-3/float(NSTATS)
            call hdc_delete(magnetics)
            deallocate(data_in)

	    end do
    end subroutine f_main
end program hdc_benchmark_put_all
