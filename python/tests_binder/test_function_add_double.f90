subroutine function_add_double(x1,x2,y)
    use iso_c_binding
    implicit none
    real(kind=c_double), intent(in) :: x1,x2
    real(kind=c_double), intent(out)  :: y
    y = x1 + x2
end subroutine function_add_double

