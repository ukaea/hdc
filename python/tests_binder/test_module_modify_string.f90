module test_module_modify_string
contains

subroutine modify_string(str_in, str_out)
    use iso_c_binding
    implicit none
    character(kind=c_char,len=100),intent(in) :: str_in
    character(kind=c_char,len=201),intent(out) :: str_out
    str_out(:) = c_null_char
    str_out(1:len_trim(str_in)) = str_in(1:len_trim(str_in))
    str_out(len_trim(str_in)+1:len_trim(str_in)+2) = '_'
    str_out(len_trim(str_in)+2:2*len_trim(str_in)+2) = str_in(1:len_trim(str_in))

end subroutine modify_string

end module test_module_modify_string
