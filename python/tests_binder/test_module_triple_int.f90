module test_module_triple_int
contains

subroutine triple_int(int_in,int_out)
    use iso_c_binding
    implicit none
    integer(kind=c_int64_t), intent(in) :: int_in
    integer(kind=c_int64_t), intent(out)  :: int_out
    int_out = 3 * int_in
end subroutine triple_int

end module test_module_triple_int
