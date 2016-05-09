module types
    use iso_c_binding
    implicit none
    type, bind(c) :: hdc_t
        type(c_ptr) :: obj
    end type hdc_t
    integer, parameter :: DP=kind(1.0D0)
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
    
    subroutine hdc_copy(src, dest) bind(c,name="hdc_copy")
        import
        type(hdc_t), value :: src,dest
    end subroutine hdc_copy
    
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
        
        function c_hdc_get_slice_path(obj, path, i) result(res) bind(c,name="hdc_get_slice_path")
            import
            type(hdc_t), value :: obj
            character(kind=c_char) :: path(*)
            integer(kind=c_size_t) :: i
            type(hdc_t) :: res
        end function c_hdc_get_slice_path
        
        function c_hdc_get_slice(obj, i) result(res) bind(c,name="hdc_get_slice")
            import
            type(hdc_t), value :: obj
            integer(kind=c_size_t) :: i
            type(hdc_t) :: res
        end function c_hdc_get_slice
        
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
        end subroutine c_hdc_set_data_int8
        
        subroutine c_hdc_set_data_double(obj, ndim, shape_, data) bind(c,name="hdc_set_data_double")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_data_double
        
        subroutine c_hdc_set_data_double_scalar(obj, data) bind(c,name="hdc_set_data_double_scalar")
            import
            type(hdc_t), value:: obj
            type(c_ptr), value :: data
        end subroutine c_hdc_set_data_double_scalar
        
        subroutine c_hdc_set_data_double_scalar_path(obj, path, data) bind(c,name="hdc_set_data_double_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr), value :: data
        end subroutine c_hdc_set_data_double_scalar_path
        
        subroutine c_hdc_set_data_double_path(obj, path, ndim, shape_, data) bind(c,name="hdc_set_data_double_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_data_double_path
        
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
        module procedure hdc_set_data_int8_scalar
        module procedure hdc_set_data_double_1d
        module procedure hdc_set_data_double_2d
        module procedure hdc_set_data_double_ad
    end interface hdc_set_data
    
     interface hdc_set
        module procedure hdc_set_data_double_1d
        module procedure hdc_set_data_double_1d_path
        module procedure hdc_set_data_double_scalar_path
        module procedure hdc_set_data_double_2d
        module procedure hdc_set_child
     end interface hdc_set
     
     interface hdc_get
        
     end interface hdc_get

    public :: hello, hdc_new_empty, hdc_delete, hdc_add_child, hdc_get_child, hdc_set_child, hdc_has_child, hdc_as_int8, hdc_delete_child, hdc_get_int8, hdc_set_data, hdc_get_double_1d, hdc_get_double_2d, hdc_get_shape, hdc_set, hdc_copy
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
    
    
    function hdc_get_ndim(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: res
        res = c_hdc_get_ndim(this)
    end function hdc_get_ndim
    
    function hdc_get_shape(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:),pointer :: res
        type(c_ptr) :: shape_ptr
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        call c_f_pointer(shape_ptr, res, (/ ndim /))
    end function hdc_get_shape

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
!         print *, shape_,"A", data
        call c_hdc_set_data_int8(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_int8_1d
    
    subroutine hdc_set_data_int8_scalar(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        shape_ = (/1/)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
!         print *, shape_,"A", data
        call c_hdc_set_data_int8(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_int8_scalar
    
    subroutine hdc_set_data_double_1d(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
!         print *, shape_,"A", data
        call c_hdc_set_data_double(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_double_1d
    
    subroutine hdc_set_data_double_1d_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        ! path stuff
        character(len=*) :: path
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ! end path stuff
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
!         print *, shape_,"A", data
        call c_hdc_set_data_double_path(this, c_path, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_double_1d_path
    
    subroutine hdc_set_data_double_scalar_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp) :: data
        ! path stuff
        character(len=*) :: path
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ! end path stuff
!         print *, shape_,"A", data
!         call c_hdc_set_data_double_scalar_path(this, c_path, data)
    end subroutine hdc_set_data_double_scalar_path
    
    
    subroutine hdc_set_data_double_sc_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        ! path stuff
        character(len=*) :: path
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ! end path stuff
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
!         print *, shape_,"A", data
        call c_hdc_set_data_double_path(this, c_path, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_double_sc_path
    
    
    
    subroutine hdc_set_data_int8_1d_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        ! path stuff
        character(len=*) :: path
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ! end path stuff
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
!         print *, shape_,"A", data
        call c_hdc_set_data_double_path(this, c_path, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_int8_1d_path

    subroutine hdc_set_data_double_2d(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp), dimension(:,:), target :: data
        integer(kind=c_long), dimension(1:2), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 2
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
!         print *, shape_,"A", data
        call c_hdc_set_data_double(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_double_2d
    
    subroutine hdc_set_data_double_ad(this, data, shape_)
    !Arbitrary dimension array as argument - shape has to be provided
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp), target :: data(*)
        integer, dimension(:), target :: shape_
        integer(kind=c_long), dimension(1) :: s
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim
!         shape_ = shape(data)
        s = shape(shape_)
        ndim = int(s(1),1)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
!         print *, shape_,"A", data
        call c_hdc_set_data_double(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_double_ad
    
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
    
    function hdc_get_slice_path(this, path, ii) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_size_t) :: ii
        type(hdc_t) :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        res = c_hdc_get_slice_path(this, c_path, ii)
    end function hdc_get_slice_path
    
!     function hdc_get_slice(this, ii) result(res)
!         use iso_c_binding
!         type(hdc_t) :: this
!         integer(kind=c_size_t) :: ii
!         type(hdc_t) :: res
!         res = c_hdc_get_slice(this, ii)
!     end function hdc_get_slice
    
    
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
!         print *,"RES",res
    end subroutine hdc_as_int8
    
    function hdc_get_int8(this) result(res)
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
!         print *,"RES",res
    end function hdc_get_int8
    
    function hdc_get_double_1d(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=dp), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_double_1d
    
    function hdc_get_double_2d(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=dp), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_double_2d
    
    subroutine hdc_as_double(this, res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=dp), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end subroutine hdc_as_double
 
end module hdc_fortran
! http://fortranwiki.org/fortran/show/Fortran+and+Cpp+objs
! https://gcc.gnu.org/onlinedocs/gfortran/Derived-Types-and-struct.html