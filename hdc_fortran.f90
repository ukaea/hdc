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
            double precision, value :: data
        end subroutine c_hdc_set_data_double_scalar_path

        subroutine c_hdc_set_data_int32_scalar(obj, data) bind(c,name="hdc_set_data_int32_scalar")
            import
            type(hdc_t), value:: obj
            integer, value :: data
        end subroutine c_hdc_set_data_int32_scalar

        subroutine c_hdc_set_data_int32_scalar_path(obj, path, data) bind(c,name="hdc_set_data_int32_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer, value :: data
        end subroutine c_hdc_set_data_int32_scalar_path

        subroutine c_hdc_set_data_int8_scalar(obj, data) bind(c,name="hdc_set_data_int8_scalar")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t), value :: data
        end subroutine c_hdc_set_data_int8_scalar

        subroutine c_hdc_set_data_int8_scalar_path(obj, path, data) bind(c,name="hdc_set_data_int8_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t), value :: data
        end subroutine c_hdc_set_data_int8_scalar_path

        subroutine c_hdc_set_data_string_path(obj, path, str) bind(c,name="hdc_set_data_string_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            character(kind=c_char), intent(in) :: str(*)
        end subroutine c_hdc_set_data_string_path

        subroutine c_hdc_set_data_string(obj, str) bind(c,name="hdc_set_data_string")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: str(*)
        end subroutine c_hdc_set_data_string

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

        function c_hdc_as_voidptr_path(obj, path) result(res) bind(c,name="hdc_as_voidptr_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr) :: res
        end function c_hdc_as_voidptr_path

        function c_hdc_get_ndim(obj) result(res) bind(c,name="hdc_get_ndim")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t) :: res
        end function c_hdc_get_ndim

        function c_hdc_get_ndim_path(obj, path) result(res) bind(c,name="hdc_get_ndim_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t) :: res
        end function c_hdc_get_ndim_path

        function c_hdc_get_shape(obj) result(res) bind(c,name="hdc_get_shape")
            import
            type(hdc_t), value:: obj
            type(c_ptr) :: res
        end function c_hdc_get_shape

        function c_hdc_get_shape_path(obj, path) result(res) bind(c,name="hdc_get_shape_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr) :: res
        end function c_hdc_get_shape_path

        function c_hdc_as_double_scalar(obj) result(res) bind(c,name="hdc_as_double_scalar")
            import
            type(hdc_t), value:: obj
            double precision :: res
        end function c_hdc_as_double_scalar

        function c_hdc_as_double_scalar_path(obj, path) result(res) bind(c,name="hdc_as_double_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            double precision :: res
        end function c_hdc_as_double_scalar_path

        function c_hdc_as_int8_scalar(obj) result(res) bind(c,name="hdc_as_int8_scalar")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t) :: res
        end function c_hdc_as_int8_scalar

        function c_hdc_as_int8_scalar_path(obj, path) result(res) bind(c,name="hdc_as_int8_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t) :: res
        end function c_hdc_as_int8_scalar_path

        function c_hdc_as_int32_scalar(obj) result(res) bind(c,name="hdc_as_int32_scalar")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int32_t) :: res
        end function c_hdc_as_int32_scalar

        function c_hdc_as_int32_scalar_path(obj, path) result(res) bind(c,name="hdc_as_int32_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int32_t) :: res
        end function c_hdc_as_int32_scalar_path

    end interface

    interface hdc_set_data
        module procedure hdc_set_data_int8_1d
        module procedure hdc_set_data_int8_scalar
        module procedure hdc_set_data_double_1d
        module procedure hdc_set_data_double_2d
        module procedure hdc_set_data_double_ad
        module procedure hdc_set_data_string
        module procedure hdc_set_data_string_path
    end interface hdc_set_data

     interface hdc_set
        module procedure hdc_set_data_double_1d
        module procedure hdc_set_data_double_1d_path
        module procedure hdc_set_data_double_scalar_path
        module procedure hdc_set_data_int32_scalar
        module procedure hdc_set_data_int32_scalar_path
        module procedure hdc_set_data_double_2d
        module procedure hdc_set_child
        module procedure hdc_set_data_string
        module procedure hdc_set_data_string_path
     end interface hdc_set

     interface hdc_get_slice
        module procedure hdc_get_slice_
        module procedure hdc_get_slice_path
     end interface hdc_get_slice

     interface hdc_get_double
        module procedure hdc_get_double_
        module procedure hdc_get_double_path
     end interface hdc_get_double

     interface hdc_get_double_1d
        module procedure hdc_get_double_1d_
        module procedure hdc_get_double_1d_path
     end interface hdc_get_double_1d

     interface hdc_get_double_2d
        module procedure hdc_get_double_2d_
        module procedure hdc_get_double_2d_path
     end interface hdc_get_double_2d

     interface hdc_get_int8
        module procedure hdc_get_int8_
        module procedure hdc_get_int8_path
     end interface hdc_get_int8

     interface hdc_get_int8_1d
        module procedure hdc_get_int8_1d_
        module procedure hdc_get_int8_1d_path
     end interface hdc_get_int8_1d

     interface hdc_get_int8_2d
        module procedure hdc_get_int8_2d_
        module procedure hdc_get_int8_2d_path
     end interface hdc_get_int8_2d

     interface hdc_get_int32
        module procedure hdc_get_int32_
        module procedure hdc_get_int32_path
     end interface hdc_get_int32

     interface hdc_get_int32_1d
        module procedure hdc_get_int32_1d_
        module procedure hdc_get_int32_1d_path
     end interface hdc_get_int32_1d

     interface hdc_get_int32_2d
        module procedure hdc_get_int32_2d_
        module procedure hdc_get_int32_2d_path
     end interface hdc_get_int32_2d


!      ! Getter interfaces are ambiguous
!      interface hdc_get_double
!         module procedure hdc_get_double_1d
! !         module procedure hdc_get_double_2d
!      end interface hdc_get_double

    interface hdc_get
        module procedure hdc_get_double_1d_sub
        module procedure hdc_get_double_2d_sub
        module procedure hdc_get_child_sub
        module procedure hdc_get_int8_1d_sub
        module procedure hdc_get_int8_2d_sub
        module procedure hdc_get_int32_1d_sub
        module procedure hdc_get_int32_2d_sub
        module procedure hdc_get_double_sub

        module procedure hdc_get_double_1d_path_sub
        module procedure hdc_get_double_2d_path_sub
        module procedure hdc_get_int8_1d_path_sub
        module procedure hdc_get_int8_2d_path_sub
        module procedure hdc_get_int32_1d_path_sub
        module procedure hdc_get_int32_2d_path_sub
        module procedure hdc_get_double_path_sub
    end interface hdc_get


    public :: hello, hdc_new_empty, hdc_delete, hdc_add_child, hdc_get_child, hdc_set_child, hdc_has_child, hdc_set_data_double_ad, &
    hdc_delete_child, hdc_get_int8_1d, hdc_get_int8_2d, hdc_set_data, hdc_get_double_1d, hdc_get_double_2d, hdc_get_shape, hdc_set, hdc_copy, hdc_get_slice, hdc_get, hdc_get_double
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
        call c_hdc_set_data_int8(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_data_int8_1d

    subroutine hdc_set_data_int8_scalar(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: data
        call c_hdc_set_data_int8_scalar(this, data)
    end subroutine hdc_set_data_int8_scalar

    subroutine hdc_set_data_int8_scalar_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: data
        character(len=*) :: path
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        type(hdc_t) :: node
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        call c_hdc_set_data_int8_scalar_path(this, path, data)
    end subroutine hdc_set_data_int8_scalar_path

    subroutine hdc_set_data_int32_scalar(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t) :: data
        call c_hdc_set_data_int32_scalar(this, data)
    end subroutine hdc_set_data_int32_scalar
    
    subroutine hdc_set_data_int32_scalar_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t) :: data
        character(len=*) :: path
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        type(hdc_t) :: node
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        call c_hdc_set_data_int32_scalar_path(this, path, data)
    end subroutine hdc_set_data_int32_scalar_path
    
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
        call c_hdc_set_data_double_scalar_path(this, c_path, data)
    end subroutine hdc_set_data_double_scalar_path
    
    subroutine hdc_set_data_string_path(this, path, str)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path, str
        ! path stuff
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        character(len=1, kind=c_char) :: c_str(len_trim(str) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ! end path stuff
        N = len_trim(str)
        do i = 1, N
            c_str(i) = str(i:i)
        end do
        c_str(N+1) = c_null_char
        call c_hdc_set_data_string_path(this, c_path, c_str)
    end subroutine hdc_set_data_string_path
    
    subroutine hdc_set_data_string(this, str)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: str
        character(len=1, kind=c_char) :: c_str(len_trim(str) + 1)
        integer :: N, i
        N = len_trim(str)
        do i = 1, N
            c_str(i) = str(i:i)
        end do
        c_str(N+1) = c_null_char
        call c_hdc_set_data_string(this, c_str)
    end subroutine hdc_set_data_string
    
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
        real(kind=dp), target, dimension(*) :: data
        integer(kind=c_long), dimension(:), target :: shape_
        integer(kind=c_long), dimension(1) :: s
        type(c_ptr) :: data_ptr, shape_ptr
!         integer(kind=c_long)
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
    
    subroutine hdc_get_child_sub(this, path, res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        type(hdc_t) :: res
        res = hdc_get_child(this, path)
    end subroutine hdc_get_child_sub
    
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
    
    subroutine hdc_get_slice_path_sub(this, path, ii, res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_size_t) :: ii
        type(hdc_t) :: res
        res = hdc_get_slice_path(this, path, ii)
    end subroutine hdc_get_slice_path_sub
    
    function hdc_get_slice_(this, ii) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer :: ii
        type(hdc_t) :: res
        res = c_hdc_get_slice(this, int(ii,c_long))
    end function hdc_get_slice_
    
    subroutine hdc_get_slice_sub(this, ii, res)
        type(hdc_t) :: this
        integer :: ii
        type(hdc_t) :: res
        res = hdc_get_slice_(this, ii)
    end subroutine hdc_get_slice_sub
    
    function hdc_get_int8_1d_(this) result(res)
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
    end function hdc_get_int8_1d_
    
    subroutine hdc_get_int8_1d_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int8_t), dimension(:), pointer :: res
        res = hdc_get_int8_1d(this)
    end subroutine hdc_get_int8_1d_sub
    
        function hdc_get_int8_2d_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int8_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_int8_2d_
    
    subroutine hdc_get_int8_2d_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int8_t), dimension(:,:), pointer :: res
        res = hdc_get_int8_2d(this)
    end subroutine hdc_get_int8_2d_sub
    
        function hdc_get_int32_1d_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int32_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_int32_1d_
    
    subroutine hdc_get_int32_1d_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int32_t), dimension(:), pointer :: res
        res = hdc_get_int32_1d(this)
    end subroutine hdc_get_int32_1d_sub
    
        function hdc_get_int32_2d_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int32_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_int32_2d_
    
    subroutine hdc_get_int32_2d_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int32_t), dimension(:,:), pointer :: res
        res = hdc_get_int32_2d(this)
    end subroutine hdc_get_int32_2d_sub

    function hdc_get_double_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        type(c_ptr) :: data_ptr
        real(kind=dp) :: res
        res = c_hdc_as_double_scalar(this)
    end function hdc_get_double_
    
    subroutine hdc_get_double_sub(this, res)
        type(hdc_t) :: this
        real(kind=dp) :: res
        res = hdc_get_double_(this)
    end subroutine hdc_get_double_sub
    
    function hdc_get_double_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        type(c_ptr) :: data_ptr
        real(kind=dp) :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        res = c_hdc_as_double_scalar_path(this, c_path)
    end function hdc_get_double_path
    
    subroutine hdc_get_double_path_sub(this, path, res)
        type(hdc_t) :: this
        character(len=*) :: path
        real(kind=dp) :: res
        res = hdc_get_double_(this)
    end subroutine hdc_get_double_path_sub
    
    function hdc_get_int8_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        type(c_ptr) :: data_ptr
        integer(kind=c_int8_t) :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        res = c_hdc_as_int8_scalar_path(this, c_path)
    end function hdc_get_int8_path
    
    subroutine hdc_get_int8_path_sub(this, path, res)
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int8_t) :: res
        res = hdc_get_int8_(this)
    end subroutine hdc_get_int8_path_sub
    
    function hdc_get_int32_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        type(c_ptr) :: data_ptr
        integer(kind=c_int32_t) :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        res = c_hdc_as_int32_scalar_path(this, c_path)
    end function hdc_get_int32_path
    
    subroutine hdc_get_int32_path_sub(this, path, res)
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int32_t) :: res
        res = hdc_get_int32_(this)
    end subroutine hdc_get_int32_path_sub
    
    function hdc_get_int8_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        type(c_ptr) :: data_ptr
        integer(kind=c_int8_t) :: res
        res = c_hdc_as_int8_scalar(this)
    end function hdc_get_int8_
    
    subroutine hdc_get_int8_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: res
        res = hdc_get_int8_(this)
    end subroutine hdc_get_int8_sub
    
    function hdc_get_int32_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        type(c_ptr) :: data_ptr
        integer(kind=c_int32_t) :: res
        res = c_hdc_as_int32_scalar(this)
    end function hdc_get_int32_
    
    subroutine hdc_get_int32_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: res
        res = hdc_get_int32_(this)
    end subroutine hdc_get_int32_sub
    
    function hdc_get_double_1d_(this) result(res)
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
    end function hdc_get_double_1d_
    
    subroutine hdc_get_double_1d_sub(this, res)
        type(hdc_t) :: this
        real(kind=dp), dimension(:), pointer :: res
        res = hdc_get_double_1d_(this)
    end subroutine hdc_get_double_1d_sub
    
! ! !     function hdc_get_double_ad(this) result(res)
! ! !         use iso_c_binding
! ! !         type(hdc_t) :: this
! ! !         integer(kind=c_int8_t) :: ndim
! ! !         integer(kind=c_long), dimension(:), pointer :: shape_
! ! !         type(c_ptr) :: shape_ptr
! ! !         type(c_ptr) :: data_ptr
! ! !         real(kind=dp), pointer :: res
! ! !         ndim = c_hdc_get_ndim(this)
! ! !         print*,"ndim"
! ! !         shape_ptr = c_hdc_get_shape(this)
! ! !         print*,"shape_ptr"
! ! !         data_ptr = c_hdc_as_voidptr(this)
! ! !         print*,"data_ptr"
! ! !         call c_f_pointer(shape_ptr, shape_, (/ ndim /))
! ! !         print*,"c_f_pointer - shape"
! ! !         call c_f_pointer(data_ptr, res)
! ! ! !         print *,"RES",res
! ! !     end function hdc_get_double_ad
! ! !     
! ! !     subroutine hdc_get_double_ad_sub(this, res)
! ! !         type(hdc_t) :: this
! ! !         real(kind=dp), dimension(:,:), pointer :: res
! ! !         res = hdc_get_double_ad(this)
! ! !     end subroutine hdc_get_double_ad_sub
    
    function hdc_get_double_2d_(this) result(res)
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
    end function hdc_get_double_2d_

    subroutine hdc_get_double_2d_sub(this,res)
        type(hdc_t) :: this
        real(kind=dp), dimension(:,:), pointer :: res
        res = hdc_get_double_2d_(this)
    end subroutine hdc_get_double_2d_sub

    function hdc_get_double_2d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=dp), dimension(:,:), pointer :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape_path(this,path)
        data_ptr = c_hdc_as_voidptr_path(this,path)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_double_2d_path

    subroutine hdc_get_double_2d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*) :: path
        real(kind=dp), dimension(:,:), pointer :: res
        res = hdc_get_double_2d_path(this, path)
    end subroutine hdc_get_double_2d_path_sub
    
    function hdc_get_double_1d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=dp), dimension(:), pointer :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape_path(this,path)
        data_ptr = c_hdc_as_voidptr_path(this,path)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_double_1d_path

    subroutine hdc_get_double_1d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*) :: path
        real(kind=dp), dimension(:), pointer :: res
        res = hdc_get_double_1d_path(this, path)
    end subroutine hdc_get_double_1d_path_sub
    
        function hdc_get_int8_2d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int8_t), dimension(:,:), pointer :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape_path(this,path)
        data_ptr = c_hdc_as_voidptr_path(this,path)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_int8_2d_path

    subroutine hdc_get_int8_2d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int8_t), dimension(:,:), pointer :: res
        res = hdc_get_int8_2d_path(this, path)
    end subroutine hdc_get_int8_2d_path_sub

    function hdc_get_int8_1d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int8_t), dimension(:), pointer :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape_path(this,path)
        data_ptr = c_hdc_as_voidptr_path(this,path)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_int8_1d_path

    subroutine hdc_get_int8_1d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int8_t), dimension(:), pointer :: res
        res = hdc_get_int8_1d_path(this, path)
    end subroutine hdc_get_int8_1d_path_sub
    
        function hdc_get_int32_2d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int32_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int32_t), dimension(:,:), pointer :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape_path(this,path)
        data_ptr = c_hdc_as_voidptr_path(this,path)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_int32_2d_path

    subroutine hdc_get_int32_2d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int32_t), dimension(:,:), pointer :: res
        res = hdc_get_int32_2d_path(this, path)
    end subroutine hdc_get_int32_2d_path_sub
    
    function hdc_get_int32_1d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int32_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int32_t), dimension(:), pointer :: res
        character(len=1, kind=c_char) :: c_path(len_trim(path) + 1)
        integer :: N, i
        N = len_trim(path)
        do i = 1, N
            c_path(i) = path(i:i)
        end do
        c_path(N+1) = c_null_char
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape_path(this,path)
        data_ptr = c_hdc_as_voidptr_path(this,path)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
!         print *,"RES",res
    end function hdc_get_int32_1d_path

    subroutine hdc_get_int32_1d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*) :: path
        integer(kind=c_int32_t), dimension(:), pointer :: res
        res = hdc_get_int32_1d_path(this, path)
    end subroutine hdc_get_int32_1d_path_sub
    
end module hdc_fortran
! http://fortranwiki.org/fortran/show/Fortran+and+Cpp+objs
! https://gcc.gnu.org/onlinedocs/gfortran/Derived-Types-and-struct.html