module types
    use iso_c_binding
    implicit none
    type, bind(c) :: hdc_t
        type(c_ptr) :: obj
    end type hdc_t
end module types

module hdc_fortran
    use types
    use iso_c_binding
    implicit none
    private
    interface
        subroutine hello() bind(c,name="hello")
            use iso_c_binding
        end subroutine hello
        
        function hdc_new_empty() result(obj) bind(c,name="hdc_new_empty")
            import
            type(hdc_t) :: obj
        end function hdc_new_empty
    
        subroutine hdc_delete(obj) bind(c,name="hdc_delete")
            import
            type(hdc_t), value :: obj
        end subroutine hdc_delete
    
        subroutine c_hdc_add_child(obj, path, node) bind(c,name="hdc_add_child")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(hdc_t), value:: node
        end subroutine c_hdc_add_child

        function c_hdc_get_child(obj, path) result(res) bind(c,name="hdc_get_child")
            import
            type(hdc_t), value :: obj
            character(kind=c_char) :: path(*)
            type(hdc_t) :: res
        end function c_hdc_get_child
        
        subroutine c_hdc_set_child(obj, path, node) bind(c,name="hdc_set_child")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(hdc_t), value:: node
        end subroutine c_hdc_set_child  
        
        
        subroutine c_hdc_delete_child(obj, path) bind(c,name="hdc_delete_child")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
        end subroutine c_hdc_delete_child  
        
        function c_hdc_has_child(obj, path) result(res) bind(c,name="hdc_has_child")
            import
            type(hdc_t), value :: obj
            character(kind=c_char), intent(in) :: path(*)
            logical(kind=c_bool) :: res ! change this to c_bool later
        end function c_hdc_has_child
        
        subroutine c_hdc_set_data_int8(obj, ndim, shape_, data) bind(c,name="hdc_set_data_int8")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
!             integer(kind=c_long), dimension(:) :: shape_
!             integer(kind=c_int8_t), dimension(:) :: data
        end subroutine c_hdc_set_data_int8
        
        function c_hdc_as_voidptr(obj) result(res) bind(c,name="hdc_as_voidptr")
            import
            type(hdc_t), value:: obj
            type(c_ptr) :: res
        end function c_hdc_as_voidptr
        
        function c_hdc_get_ndim(obj) result(res) bind(c,name="hdc_get_ndim")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t) :: res
        end function c_hdc_get_ndim
        
        function c_hdc_get_shape(obj) result(res) bind(c,name="hdc_get_shape")
            import
            type(hdc_t), value:: obj
            type(c_ptr) :: res
        end function c_hdc_get_shape
        
    end interface 
     
    interface hdc_set_data
        module procedure hdc_set_data_int8_1d
    end interface hdc_set_data
     

    public :: hello, hdc_new_empty, hdc_delete, hdc_add_child, hdc_get_child, hdc_set_child, hdc_has_child, hdc_as_int8, hdc_delete_child, hdc_set_data
contains
 
    subroutine hdc_add_child(this, path, node)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        type(hdc_t) :: node
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
       call c_hdc_add_child(this, c_path, node)
    end subroutine hdc_add_child
    
    subroutine hdc_delete_child(this, path)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
       call c_hdc_delete_child(this, c_path)
    end subroutine hdc_delete_child
    
    function hdc_has_child(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        logical(kind=c_bool) :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        res = c_hdc_has_child(this, c_path)
    end function hdc_has_child
    
    subroutine hdc_set_child(this, path, node)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        type(hdc_t) :: node
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
       call c_hdc_set_child(this, c_path, node)
    end subroutine hdc_set_child

    subroutine hdc_set_data_int8_1d(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        print *, shape_,"A", data
        call c_hdc_set_data_int8(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_int8_1d

    
    function hdc_get_child(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        type(hdc_t) :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        res = c_hdc_get_child(this, c_path)
    end function hdc_get_child
    
    subroutine hdc_as_int8(this, res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int8_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
        print *,"RES",res
    end subroutine hdc_as_int8
    
 
end module hdc_fortran
! http://fortranwiki.org/fortran/show/Fortran+and+Cpp+objs
! https://gcc.gnu.org/onlinedocs/gfortran/Derived-Types-and-struct.html