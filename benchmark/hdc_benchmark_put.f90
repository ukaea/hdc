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


program test_hdc_put
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
        real(4), pointer     :: data(:), flux_data(:), field_data(:)
        real(4)              :: time
        integer, parameter   :: out_unit=20
        CHARACTER(LEN=30)    :: Format
        
        NSTATS = 10000
        
        do k = 10, 100001, 1000
            SIGNAL_LENGTH = k
            allocate(data(SIGNAL_LENGTH),flux_data(SIGNAL_LENGTH),field_data(SIGNAL_LENGTH))
            do i = 1,SIGNAL_LENGTH
                data(i) = float(2*i)
            end do
            allocate(magnetics)
            magnetics = hdc_new_empty()
            call c_getMillis(t1)
            do k2 = 1,NSTATS
                call hdc_set(magnetics,"flux_loop(1)/flux/data",data)
                call hdc_set(magnetics,"bpol_probe(1)/field",data)
                call hdc_set(magnetics,"time",10.0)
            end do
            call c_getMillis(t2)
            Format = "(I8.8, EN15.5)"
    ! 	    write (out_unit,Format) SIGNAL_LENGTH, (t2-t1)*1e-3/float(NSTATS)
            write (*,Format) SIGNAL_LENGTH, (t2-t1)*1e-3/float(NSTATS)
            call hdc_delete(magnetics)
            deallocate(data)
        end do
    end subroutine f_main
end program test_hdc_put
