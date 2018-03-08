module hdc_fortran
    use iso_c_binding
    implicit none
    type, bind(c) :: hdc_t
        type(c_ptr) :: obj !< void pointer to HDC container
    end type hdc_t

    integer, parameter :: dp=kind(1.0D0)
    integer, parameter :: sp=kind(1.0)

    integer(kind=c_int64_t), parameter ::  HDCDefault             = 0_8
    integer(kind=c_int64_t), parameter ::  HDCFortranOrder        = 1_8
    integer(kind=c_int64_t), parameter ::  HDCReadOnly            = 2_8
    integer(kind=c_int64_t), parameter ::  HDCExternal            = 4_8
    integer(kind=c_int64_t), parameter ::  HDCChildrenInitialized = 8_8

    private
    interface
        subroutine hello() bind(c,name="hello")
            use iso_c_binding
        end subroutine hello

        !> Default constructor. This is interface to C.
        function hdc_new_empty() result(obj) bind(c,name="hdc_new_empty")
            import
            type(hdc_t) :: obj
        end function hdc_new_empty

        !> Constructs HDC from string/uri
        function c_hdc_new_string(str) result(obj) bind(c,name="hdc_new_string")
            import
            type(hdc_t) :: obj
            character(kind=c_char), intent(in) :: str(*)
        end function c_hdc_new_string

        !> Default constructor. This is interface to C.
        function hdc_new_size(s) result(obj) bind(c,name="hdc_new_size")
            import
            integer(kind=c_size_t),value :: s
            type(hdc_t) :: obj
        end function hdc_new_size

        !> Construct empty arra of type given by string.
        function c_hdc_new_dtype(ndim, shape_, type_str) result(obj) bind(c,name="hdc_new_dtype")
            import
            type(hdc_t) :: obj
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            character(kind=c_char), intent(in) :: type_str(*)
        end function c_hdc_new_dtype

        function c_hdc_new_void_ptr() result(obj) bind(c, name="hdc_new_void_ptr")
            import
            type(c_ptr) :: obj
        end function c_hdc_new_void_ptr

        function hdc_get_ptr(tree) result(ptr) bind(c, name="hdc_get_ptr")
            import
            type(hdc_t) :: tree
            type(c_ptr) :: ptr
        end function hdc_get_ptr

        subroutine hdc_delete_ptr(obj) bind(c,name="hdc_delete_ptr")
            import
            type(c_ptr), value :: obj
        end subroutine hdc_delete_ptr

        function hdc_new_int8() result(obj) bind(c,name="hdc_new_int8")
            import
            type(hdc_t) :: obj
        end function hdc_new_int8

        function hdc_new_int32() result(obj) bind(c,name="hdc_new_int32")
            import
            type(hdc_t) :: obj
        end function hdc_new_int32

        function hdc_new_double() result(obj) bind(c,name="hdc_new_double")
            import
            type(hdc_t) :: obj
        end function hdc_new_double

        !> Desctructor. This is interface to C.
        subroutine hdc_delete(obj) bind(c,name="hdc_delete")
            import
           type(hdc_t), value :: obj
!             type(hdc_t), pointer :: obj
        end subroutine hdc_delete

        !> Performs deep copy of current node. This is interface to C.
        function c_hdc_copy(src) result(obj) bind(c,name="hdc_copy")
            import
            type(hdc_t), value :: src
            type(hdc_t) :: obj
        end function c_hdc_copy

        !> Adds HDC subtree as child with given path. If neccessary, recursively creates subnodes. This is interface to C.
        subroutine c_hdc_add_child(obj, path, node) bind(c,name="hdc_add_child")
            import
            type(hdc_t), value :: obj
            character(kind=c_char), intent(in) :: path(*)
            type(hdc_t), value :: node
        end subroutine c_hdc_add_child

        !> Initializations of HDC library
        subroutine c_hdc_init(pluginFileName,pluginSettingsString) bind(c,name="HDC_init_c")
            import
            character(kind=c_char), intent(in) :: pluginFileName(*)
            character(kind=c_char), intent(in) :: pluginSettingsString(*)
        end subroutine c_hdc_init

        !> Sets slice to a given position. This is interface to C.
        subroutine hdc_set_slice(obj, pos, node) bind(c,name="hdc_set_slice")
            import
            type(hdc_t), value :: obj
            integer(kind=c_int32_t), value     :: pos
            type(hdc_t), value :: node
        end subroutine hdc_set_slice

        !> Inserts slice to a given position. This is interface to C.
        subroutine hdc_insert_slice(obj, pos, node) bind(c,name="hdc_insert_slice")
            import
            type(hdc_t), value :: obj
            integer(kind=c_int32_t), value     :: pos
            type(hdc_t), value :: node
        end subroutine hdc_insert_slice

        !> Appends slice to the end of list. This is interface to C.
        subroutine hdc_append_slice(obj, node) bind(c,name="hdc_append_slice")
            import
            type(hdc_t), value :: obj
            type(hdc_t), value :: node
        end subroutine hdc_append_slice

        subroutine hdc_print_type_str(obj) bind(c,name="hdc_print_type_str")
            import
            type(hdc_t), value :: obj
        end subroutine hdc_print_type_str

! ! !         function c_hdc_get_type_str(obj) result(res) bind(c,name="hdc_get_type_str")
! ! !             import
! ! !             type(hdc_t), value :: obj
! ! !             character(len=:), allocatable :: res
! ! !         end function c_hdc_get_type_str

        subroutine c_hdc_to_json(obj,path,mode) bind(c,name="hdc_to_json")
            import
            type(hdc_t), value :: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int32_t), value :: mode
        end subroutine c_hdc_to_json

        !> Returns HDC subtree by given path. This is interface to C.
        function c_hdc_get_child(obj, path) result(res) bind(c,name="hdc_get")
            import
            type(hdc_t), value :: obj
            character(kind=c_char) :: path(*)
            type(hdc_t) :: res
        end function c_hdc_get_child

        !> Returns HDC subtree by given path and slice index. This is interface to C.
        function c_hdc_get_slice_path(obj, path, i) result(res) bind(c,name="hdc_get_slice_path")
            import
            type(hdc_t), value :: obj
            character(kind=c_char) :: path(*)
            integer(kind=c_size_t) :: i
            type(hdc_t) :: res
        end function c_hdc_get_slice_path
        !> Returns HDC subtree by given slice index.  This is interface to C.
        function c_hdc_get_slice(obj, i) result(res) bind(c,name="hdc_get_slice")
            import
            type(hdc_t), value :: obj
            integer(kind=c_size_t) :: i
            type(hdc_t) :: res
        end function c_hdc_get_slice
        !> Sets HDC subtree to given path.
        subroutine c_hdc_set_child(obj, path, node) bind(c,name="hdc_set_child")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(hdc_t), value:: node
        end subroutine c_hdc_set_child
        !> Recursively deletes subtree at given path. This is interface to C.
        subroutine c_hdc_delete_child(obj, path) bind(c,name="hdc_delete_child")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
        end subroutine c_hdc_delete_child
        !> Returns true if subtree with given path exists. This is interface to C.
        function c_hdc_has_child(obj, path) result(res) bind(c,name="hdc_has_child")
            import
            type(hdc_t), value :: obj
            character(kind=c_char), intent(in) :: path(*)
            logical(kind=c_bool) :: res ! change this to c_bool later
        end function c_hdc_has_child

        !> Sets array of int8. This is interface to C.
        subroutine c_hdc_set_int8(obj, ndim, shape_, data) bind(c,name="hdc_set_int8")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_int8

        !> Sets array of int32. This is interface to C.
        subroutine c_hdc_set_int32(obj, ndim, shape_, data, flags) bind(c,name="hdc_set_int32")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
            integer(kind=c_int64_t), value :: flags
        end subroutine c_hdc_set_int32

        !> Sets array of int32. This is interface to C.
        subroutine c_hdc_set_int32_path(obj, path, ndim, shape_, data) bind(c,name="hdc_set_int32_path")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_int32_path

        !> Sets array of int8. This is interface to C.
        subroutine c_hdc_set_int8_path(obj, path, ndim, shape_, data) bind(c,name="hdc_set_int8_path")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_int8_path


        !> Sets array of double. This is interface to C.
        subroutine c_hdc_set_double(obj, ndim, shape_, data) bind(c,name="hdc_set_double")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_double
        !> Sets array of double. This is interface to C.
        subroutine c_hdc_set_float(obj, ndim, shape_, data) bind(c,name="hdc_set_float")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_float
        !> Stes scalar double.This is interface to C.
        subroutine c_hdc_set_double_scalar(obj, data) bind(c,name="hdc_set_double_scalar")
            import
            type(hdc_t), value:: obj
            real(kind=c_double), value :: data
        end subroutine c_hdc_set_double_scalar
        !> Sets scalar float.This is interface to C.
        subroutine c_hdc_set_float_scalar(obj, data) bind(c,name="hdc_set_float_scalar")
            import
            type(hdc_t), value:: obj
            real(kind=c_float), value :: data
        end subroutine c_hdc_set_float_scalar
        !> Sets scalar double to given path. This is interface to C.
        subroutine c_hdc_set_double_scalar_path(obj, path, data) bind(c,name="hdc_set_double_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            real(kind=c_double), value :: data
        end subroutine c_hdc_set_double_scalar_path
        !> Sets scalar float to given path. This is interface to C.
        subroutine c_hdc_set_float_scalar_path(obj, path, data) bind(c,name="hdc_set_float_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            real(kind=c_float), value :: data
        end subroutine c_hdc_set_float_scalar_path
        !> Sets scalar int32.This is interface to C.
        subroutine c_hdc_set_int32_scalar(obj, data) bind(c,name="hdc_set_int32_scalar")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int32_t), value :: data
        end subroutine c_hdc_set_int32_scalar
        !> Sets scalar int32 to given path. This is interface to C.
        subroutine c_hdc_set_int32_scalar_path(obj, path, data) bind(c,name="hdc_set_int32_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int32_t), value :: data
        end subroutine c_hdc_set_int32_scalar_path
        !> Sets scalar int8. This is interface to C.
        subroutine c_hdc_set_int8_scalar(obj, data) bind(c,name="hdc_set_int8_scalar")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t), value :: data
        end subroutine c_hdc_set_int8_scalar
        !>  Sets scalar int8 to given path. This is interface to C.
        subroutine c_hdc_set_int8_scalar_path(obj, path, data) bind(c,name="hdc_set_int8_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t), value :: data
        end subroutine c_hdc_set_int8_scalar_path
        !> Sets string to given path. This is interface to C.
        subroutine c_hdc_set_string_path(obj, path, str) bind(c,name="hdc_set_string_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            character(kind=c_char), intent(in) :: str(*)
        end subroutine c_hdc_set_string_path
        !> Sets string. This is interface to C.
        subroutine c_hdc_set_string(obj, str) bind(c,name="hdc_set_string")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: str(*)
        end subroutine c_hdc_set_string
        !> Sets double array to given path. This is interface to C.
        subroutine c_hdc_set_double_path(obj, path, ndim, shape_, data) bind(c,name="hdc_set_double_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_double_path
        !> Sets double array to given path. This is interface to C.
        subroutine c_hdc_set_float_path(obj, path, ndim, shape_, data) bind(c,name="hdc_set_float_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_float_path
        !> Sets arbitrary data casted to void pointer. This is interface to C.
        function c_hdc_as_voidptr(obj) result(res) bind(c,name="hdc_as_voidptr")
            import
            type(hdc_t), value:: obj
            type(c_ptr) :: res
        end function c_hdc_as_voidptr
        !> Sets arbitrary data casted to void pointer. This is interface to C.
        subroutine c_hdc_as_string_fortran(obj,str,strlen) bind(c,name="hdc_as_string_fortran")
            import
            type(hdc_t), value:: obj
            integer(c_int) :: strlen
            character(kind=c_char) :: str(*)
        end subroutine c_hdc_as_string_fortran
        !> Sets arbitrary data casted to void pointer. This is interface to C.
        subroutine c_hdc_as_string_path_fortran(obj,path,str,strlen)  bind(c,name="hdc_as_string_path_fortran")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(c_int) :: strlen
            character(kind=c_char) :: str(*)
        end subroutine c_hdc_as_string_path_fortran
        !> Sets arbitrary data casted to void pointer to given path. This is interface to C.
        function c_hdc_as_voidptr_path(obj, path) result(res) bind(c,name="hdc_as_voidptr_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr) :: res
        end function c_hdc_as_voidptr_path
        !> Returns array dimension. This is interface to C.
        function c_hdc_get_ndim(obj) result(res) bind(c,name="hdc_get_ndim")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t) :: res
        end function c_hdc_get_ndim
        !> Returns array dimension at give path. This is interface to C.
        function c_hdc_get_ndim_path(obj, path) result(res) bind(c,name="hdc_get_ndim_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t) :: res
        end function c_hdc_get_ndim_path
        !> Returns array shape. This is interface to C.
        function c_hdc_get_shape(obj) result(res) bind(c,name="hdc_get_shape")
            import
            type(hdc_t), value:: obj
            type(c_ptr) :: res
        end function c_hdc_get_shape
        !> Returns array shape at given path.This is interface to C.
        function c_hdc_get_shape_path(obj, path) result(res) bind(c,name="hdc_get_shape_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr) :: res
        end function c_hdc_get_shape_path
        !> Returns scalar double. This is interface to C.
        function c_hdc_as_double_scalar(obj) result(res) bind(c,name="hdc_as_double_scalar")
            import
            type(hdc_t), value:: obj
            real(kind=c_double) :: res
        end function c_hdc_as_double_scalar
        !> Returns scalar double from given path. This is interface to C.
        function c_hdc_as_double_scalar_path(obj, path) result(res) bind(c,name="hdc_as_double_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            real(kind=c_double) :: res
        end function c_hdc_as_double_scalar_path
        !> Returns scalar int8.
        function c_hdc_as_int8_scalar(obj) result(res) bind(c,name="hdc_as_int8_scalar")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t) :: res
        end function c_hdc_as_int8_scalar
        !> Returns scalar int8 from given path. This is interface to C.
        function c_hdc_as_int8_scalar_path(obj, path) result(res) bind(c,name="hdc_as_int8_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t) :: res
        end function c_hdc_as_int8_scalar_path
        !> Returns scalar int32. This is interface to C.
        function c_hdc_as_int32_scalar(obj) result(res) bind(c,name="hdc_as_int32_scalar")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int32_t) :: res
        end function c_hdc_as_int32_scalar
        !> Returns scalar int32 from given path. This is interface to C.
        function c_hdc_as_int32_scalar_path(obj, path) result(res) bind(c,name="hdc_as_int32_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int32_t) :: res
        end function c_hdc_as_int32_scalar_path

        subroutine hdc_dump(obj) bind(c,name="hdc_dump")
            import
            type(hdc_t), value:: obj
        end subroutine hdc_dump

        function hdc_get_type(obj) bind(c,name="hdc_get_type") result(res)
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t) :: res
        end function hdc_get_type

        !> Returns scalar float. This is interface to C.
        function c_hdc_as_float_scalar(obj) result(res) bind(c,name="hdc_as_float_scalar")
            import
            type(hdc_t), value:: obj
            real(kind=c_float) :: res
        end function c_hdc_as_float_scalar
        !> Returns scalar float from given path. This is interface to C.
        function c_hdc_as_float_scalar_path(obj, path) result(res) bind(c,name="hdc_as_float_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            real(kind=c_float) :: res
        end function c_hdc_as_float_scalar_path


        function hdc_new_int64() result(obj) bind(c,name="hdc_new_int64")
            import
            type(hdc_t) :: obj
        end function hdc_new_int64

        !> Sets array of int64. This is interface to C.
        subroutine c_hdc_set_int64(obj, ndim, shape_, data) bind(c,name="hdc_set_int64")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_int64

        !> Sets array of int64. This is interface to C.
        subroutine c_hdc_set_int64_path(obj, path, ndim, shape_, data) bind(c,name="hdc_set_int64_path")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int8_t),value :: ndim
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr), value :: shape_
            type(c_ptr), value :: data
        end subroutine c_hdc_set_int64_path

        !> Sets scalar int64.This is interface to C.
        subroutine c_hdc_set_int64_scalar(obj, data) bind(c,name="hdc_set_int64_scalar")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int64_t), value :: data
        end subroutine c_hdc_set_int64_scalar
        !> Sets scalar int64 to given path. This is interface to C.
        subroutine c_hdc_set_int64_scalar_path(obj, path, data) bind(c,name="hdc_set_int64_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int64_t), value :: data
        end subroutine c_hdc_set_int64_scalar_path

        !> Returns scalar int64. This is interface to C.
        function c_hdc_as_int64_scalar(obj) result(res) bind(c,name="hdc_as_int64_scalar")
            import
            type(hdc_t), value:: obj
            integer(kind=c_int64_t) :: res
        end function c_hdc_as_int64_scalar
        !> Returns scalar int64 from given path. This is interface to C.
        function c_hdc_as_int64_scalar_path(obj, path) result(res) bind(c,name="hdc_as_int64_scalar_path")
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int64_t) :: res
        end function c_hdc_as_int64_scalar_path
    end interface

    !> Generic set interface.
    interface hdc_set_data
        module procedure hdc_set_int8_1d
        module procedure hdc_set_int8_1d_path
        module procedure hdc_set_int8_scalar
        module procedure hdc_set_int32_1d
!         module procedure hdc_set_int32_2d
!         module procedure hdc_set_int32_3d
!         module procedure hdc_set_int32_4d
        module procedure hdc_set_int32_1d_path
        module procedure hdc_set_int32_scalar
        module procedure hdc_set_int64_1d
!         module procedure hdc_set_int64_2d
!         module procedure hdc_set_int64_3d
!         module procedure hdc_set_int64_4d
        module procedure hdc_set_int64_1d_path
        module procedure hdc_set_int64_scalar
        module procedure hdc_set_double_1d
        module procedure hdc_set_double_1d_path
        module procedure hdc_set_double_2d
        module procedure hdc_set_double_ad
        module procedure hdc_set_string
        module procedure hdc_set_string_path
    end interface hdc_set_data

    !> Generic set interface.
    interface hdc_set
        module procedure hdc_set_double_1d
        module procedure hdc_set_double_1d_path
        module procedure hdc_set_double_2d
        module procedure hdc_set_float_1d
        module procedure hdc_set_float_1d_path
        module procedure hdc_set_int32_1d
        module procedure hdc_set_int32_1d_path
        module procedure hdc_set_int64_1d
        module procedure hdc_set_int64_1d_path
!         module procedure hdc_set_double_2d_path
        module procedure hdc_set_double_scalar
        module procedure hdc_set_double_scalar_path
        module procedure hdc_set_float_scalar
        module procedure hdc_set_float_scalar_path
        module procedure hdc_set_int32_scalar
        module procedure hdc_set_int32_scalar_path
        module procedure hdc_set_int64_scalar
        module procedure hdc_set_int64_scalar_path
        module procedure hdc_set_child
        module procedure hdc_set_string
        module procedure hdc_set_string_path
     end interface hdc_set

     interface hdc_get_slice
        module procedure hdc_get_slice_
        module procedure hdc_get_slice_l
        module procedure hdc_get_slice_path
     end interface hdc_get_slice

     interface hdc_as_double
        module procedure hdc_as_double_
        module procedure hdc_as_double_path
     end interface hdc_as_double

     interface hdc_as_double_1d
        module procedure hdc_as_double_1d_
        module procedure hdc_as_double_1d_path
     end interface hdc_as_double_1d

     interface hdc_as_double_2d
        module procedure hdc_as_double_2d_
        module procedure hdc_as_double_2d_path
     end interface hdc_as_double_2d

     interface hdc_as_string
        module procedure hdc_as_string_
        module procedure hdc_as_string_path
    end interface hdc_as_string

     interface hdc_as_int8
        module procedure hdc_as_int8_
        module procedure hdc_as_int8_path
     end interface hdc_as_int8

     interface hdc_as_int8_1d
        module procedure hdc_as_int8_1d_
        module procedure hdc_as_int8_1d_path
     end interface hdc_as_int8_1d

     interface hdc_as_int8_2d
        module procedure hdc_as_int8_2d_
        module procedure hdc_as_int8_2d_path
     end interface hdc_as_int8_2d

     interface hdc_as_int32
        module procedure hdc_as_int32_
        module procedure hdc_as_int32_path
     end interface hdc_as_int32

     interface hdc_as_int32_1d
        module procedure hdc_as_int32_1d_
        module procedure hdc_as_int32_1d_path
     end interface hdc_as_int32_1d

     interface hdc_as_int32_2d
        module procedure hdc_as_int32_2d_
        module procedure hdc_as_int32_2d_path
     end interface hdc_as_int32_2d

     interface hdc_as_int64
        module procedure hdc_as_int64_
        module procedure hdc_as_int64_path
     end interface hdc_as_int64

     interface hdc_as_int64_1d
        module procedure hdc_as_int64_1d_
        module procedure hdc_as_int64_1d_path
     end interface hdc_as_int64_1d

     interface hdc_as_int64_2d
        module procedure hdc_as_int64_2d_
        module procedure hdc_as_int64_2d_path
     end interface hdc_as_int64_2d

    !> Generic hdc_get interface
    interface hdc_get
        module procedure hdc_as_double_sub
        module procedure hdc_as_double_1d_sub
        module procedure hdc_as_double_2d_sub
        module procedure hdc_get_child_sub
        module procedure hdc_as_int8_1d_sub
        module procedure hdc_as_int8_2d_sub
        module procedure hdc_as_int32_1d_sub
        module procedure hdc_as_int32_2d_sub
        module procedure hdc_as_int32_sub
        module procedure hdc_as_int64_1d_sub
        module procedure hdc_as_int64_2d_sub
        module procedure hdc_as_int64_sub
        module procedure hdc_as_float_sub
        module procedure hdc_as_string_sub

        module procedure hdc_as_double_1d_path_sub
        module procedure hdc_as_double_2d_path_sub
        module procedure hdc_as_float_1d_path_sub
        module procedure hdc_as_float_2d_path_sub
        module procedure hdc_as_int8_1d_path_sub
        module procedure hdc_as_int8_2d_path_sub
        module procedure hdc_as_int32_1d_path_sub
        module procedure hdc_as_int32_2d_path_sub
        module procedure hdc_as_int32_path_sub
        module procedure hdc_as_int64_1d_path_sub
        module procedure hdc_as_int64_2d_path_sub
        module procedure hdc_as_int64_path_sub
        module procedure hdc_as_double_path_sub
        module procedure hdc_as_float_path_sub
        module procedure hdc_as_string_path_sub
    end interface hdc_get

    interface hdc_get_shape
        module procedure hdc_get_shape_
        module procedure hdc_get_shape_pos
        module procedure hdc_get_shape_path
    end interface hdc_get_shape

    interface hdc_set_int8_scalar
        module procedure hdc_set_int8_scalar_path
        module procedure hdc_set_int8_scalar
    end interface hdc_set_int8_scalar

    interface hdc_get_ptr_f
        module procedure hdc_get_ptr_f
    end interface hdc_get_ptr_f

    interface hdc_get_ndim
        module procedure hdc_get_ndim
        module procedure hdc_get_ndim_path
    end interface hdc_get_ndim

    interface hdc_init
        module procedure hdc_init_
    end interface hdc_init


! TODO:
!     interface hdc_new
! !         module procedure hdc_new_empty
!         module procedure hdc_new_size
!     end interface hdc_new
!
    public :: hello, hdc_new_empty, hdc_new_size, hdc_delete, hdc_add_child, hdc_get_child, hdc_set_child, hdc_has_child, hdc_set_double_ad, &
    hdc_delete_child, hdc_as_int8_1d, hdc_as_int8_2d, hdc_set, hdc_as_double_1d, hdc_as_double_2d, hdc_get_shape, hdc_set_data, &
    hdc_get_slice, hdc_get, hdc_as_double, hdc_copy, hdc_t, dp, hdc_dump, hdc_new_pokus, hello_fort, hdc_new_ptr, hdc_delete_ptr, hdc_get_ptr_f, &
    hdc_set_double_1d, hdc_set_double_1d_path, hdc_get_ndim, hdc_print_type_str, hdc_to_json, hdc_insert_slice, hdc_append_slice, hdc_set_slice, &
    hdc_set_int8_scalar, hdc_get_slice_path_sub, hdc_get_slice_sub, hdc_as_int32_1d_, hdc_as_int32_2d_, hdc_as_int8_path_sub, hdc_as_int32_path_sub, &
    hdc_as_int8_sub, hdc_as_int32_sub, hdc_as_int32_2d_path, hdc_as_int32_1d_path, hdc_new_dtype, hdc_get_type, hdc_as_float_1d_sub, hdc_as_float_2d_sub, &
    hdc_as_float_2d_path_sub, hdc_as_float_1d_path_sub, hdc_as_float_sub, hdc_as_float_path_sub, hdc_destroy, hdc_init, hdc_init_, hdc_as_string_sub, &
    hdc_as_string_, hdc_as_string_path_sub, hdc_as_string_path, hdc_as_int32, hdc_as_string, hdc_as_int64_1d_, hdc_as_int64_2d_, hdc_as_int64_path_sub, &
    hdc_as_int64_sub, hdc_as_int64_2d_path, hdc_as_int64_1d_path, hdc_as_int64, hdc_new_string
contains

    subroutine hdc_add_child(this, path, node)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        type(hdc_t) :: node
!         print *,trim(path)//c_null_char
        call c_hdc_add_child(this, trim(path)//c_null_char, node)
    end subroutine hdc_add_child

    subroutine hdc_delete_child(this, path)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        call c_hdc_delete_child(this, trim(path)//c_null_char)
    end subroutine hdc_delete_child

    function hdc_has_child(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        logical(kind=c_bool) :: res
        res = c_hdc_has_child(this, trim(path)//c_null_char)
    end function hdc_has_child

    function hdc_get_ndim(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: res
        res = c_hdc_get_ndim(this)
    end function hdc_get_ndim

    function hdc_get_ndim_path(this,path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: res
        res = c_hdc_get_ndim_path(this, trim(path)//c_null_char)
    end function hdc_get_ndim_path

    function hdc_get_shape_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: res
        type(c_ptr) :: shape_ptr
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        call c_f_pointer(shape_ptr, res, (/ ndim /))
    end function hdc_get_shape_

    function hdc_get_shape_path(this,path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        character(len=*), intent(in) :: path
        integer(kind=c_long), dimension(:),pointer :: res
        type(c_ptr) :: shape_ptr
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, res, (/ ndim /))
    end function hdc_get_shape_path

    function hdc_get_shape_pos(this, pos) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t) :: pos
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:),pointer :: shape_
        integer(kind=c_long) :: res
        type(c_ptr) :: shape_ptr
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        res = shape_(pos)
    end function hdc_get_shape_pos

    subroutine hdc_set_child(this, path, node)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        type(hdc_t) :: node
        call c_hdc_set_child(this, trim(path)//c_null_char, node)
    end subroutine hdc_set_child

    subroutine hdc_set_int8_1d(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_ ! Won't compile on gfortran-4.8
!        integer(kind=c_long), dimension(1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_int8(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_int8_1d

    subroutine hdc_set_int32_1d(this, data, flags_)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_ ! Won't compile on gfortran-4.8
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        integer(kind=c_int64_t), intent(in), optional :: flags_
        integer(kind=c_int64_t) :: flags
        if (.not. present(flags_)) then
            flags = HDCFortranOrder;
        else
            flags = flags_
        end if
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_int32(this, ndim, shape_ptr, data_ptr, flags)
    end subroutine hdc_set_int32_1d

    subroutine hdc_set_int8_scalar(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: data
        call c_hdc_set_int8_scalar(this, data)
    end subroutine hdc_set_int8_scalar

    subroutine hdc_set_int8_scalar_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: data
        character(len=*), intent(in) :: path
        call c_hdc_set_int8_scalar_path(this, trim(path)//c_null_char, data)
    end subroutine hdc_set_int8_scalar_path

    subroutine hdc_set_int32_scalar(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t) :: data
        call c_hdc_set_int32_scalar(this, data)
    end subroutine hdc_set_int32_scalar

    subroutine hdc_copy(src, dest)
        use iso_c_binding
        type(hdc_t) :: src, dest
        dest = c_hdc_copy(src)
    end subroutine hdc_copy

    subroutine hdc_set_int32_scalar_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t) :: data
        character(len=*), intent(in) :: path
        call c_hdc_set_int32_scalar_path(this, trim(path)//c_null_char, data)
    end subroutine hdc_set_int32_scalar_path

    subroutine hdc_set_double_1d(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_double(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_double_1d

    subroutine hdc_set_float_1d(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=sp), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_float(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_float_1d

    subroutine hdc_set_double_1d_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        character(len=*), intent(in) :: path
        integer(1) :: ndim = 1
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_double_path(this, trim(path)//c_null_char, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_double_1d_path

    subroutine hdc_set_float_1d_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=sp), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        character(len=*), intent(in) :: path
        integer(1) :: ndim = 1
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_float_path(this, trim(path)//c_null_char, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_float_1d_path



    subroutine hdc_set_double_scalar_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp) :: data
        ! path stuff
        character(len=*), intent(in) :: path
        call c_hdc_set_double_scalar_path(this, trim(path)//c_null_char, data)
    end subroutine hdc_set_double_scalar_path


    subroutine hdc_set_double_scalar(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp) :: data
        call c_hdc_set_double_scalar(this, data)
    end subroutine hdc_set_double_scalar

    subroutine hdc_set_float_scalar_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=sp) :: data
        ! path stuff
        character(len=*), intent(in) :: path
        call c_hdc_set_float_scalar_path(this, trim(path)//c_null_char, data)
    end subroutine hdc_set_float_scalar_path


    subroutine hdc_set_float_scalar(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=sp) :: data
        call c_hdc_set_float_scalar(this, data)
    end subroutine hdc_set_float_scalar


    subroutine hdc_set_string_path(this, path, str)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path, str
        call c_hdc_set_string_path(this, trim(path)//c_null_char, trim(str)//c_null_char)
    end subroutine hdc_set_string_path

    subroutine hdc_set_string(this, str)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*) :: str
        call c_hdc_set_string(this, trim(str)//c_null_char)
    end subroutine hdc_set_string

    subroutine hdc_set_int8_1d_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        character(len=*), intent(in) :: path
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_int8_path(this, trim(path)//c_null_char, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_int8_1d_path

    subroutine hdc_set_int32_1d_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        character(len=*), intent(in) :: path
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_int32_path(this, trim(path)//c_null_char, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_int32_1d_path


    subroutine hdc_set_double_2d(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp), dimension(:,:), target :: data
        integer(kind=c_long), dimension(1:2), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 2
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_double(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_double_2d

    subroutine hdc_set_double_ad(this, data, shape_)
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
        call c_hdc_set_double(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_double_ad

    function hdc_get_child(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        type(hdc_t) :: res
        res = c_hdc_get_child(this, trim(path)//c_null_char)
    end function hdc_get_child

    subroutine hdc_get_child_sub(this, path, res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        type(hdc_t) :: res
        res = hdc_get_child(this, trim(path)//c_null_char)
    end subroutine hdc_get_child_sub

    function hdc_get_slice_path(this, path, ii) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_size_t) :: ii
        type(hdc_t) :: res
        res = c_hdc_get_slice_path(this, trim(path)//c_null_char, ii)
    end function hdc_get_slice_path

!     function hdc_get_slice_l_path(this, path, ii) result(res)
!         use iso_c_binding
!         type(hdc_t) :: this
!         character(len=*), intent(in) :: path
!         integer(kind=8) :: ii
!         type(hdc_t) :: res
!         res = c_hdc_get_slice_path(this, trim(path)//c_null_char, ii)
!     end function hdc_get_slice_l_path

    subroutine hdc_get_slice_path_sub(this, path, ii, res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_size_t) :: ii
        type(hdc_t) :: res
        res = hdc_get_slice_path(this, trim(path)//c_null_char, ii)
    end subroutine hdc_get_slice_path_sub

    function hdc_get_slice_(this, ii) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t) :: ii
        type(hdc_t) :: res
        res = c_hdc_get_slice(this, int(ii,c_long))
    end function hdc_get_slice_

    function hdc_get_slice_l(this, ii) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=8) :: ii
        type(hdc_t) :: res
        res = c_hdc_get_slice(this, int(ii,c_long))
    end function hdc_get_slice_l

    subroutine hdc_get_slice_sub(this, ii, res)
        type(hdc_t) :: this
        integer :: ii
        type(hdc_t) :: res
        res = hdc_get_slice_(this, ii)
    end subroutine hdc_get_slice_sub

    function hdc_as_int8_1d_(this) result(res)
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
    end function hdc_as_int8_1d_

    subroutine hdc_as_int8_1d_sub(this, res)
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
    end subroutine hdc_as_int8_1d_sub

    function hdc_as_int8_2d_(this) result(res)
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
    end function hdc_as_int8_2d_

    subroutine hdc_as_int8_2d_sub(this, res)
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
    end subroutine hdc_as_int8_2d_sub

    function hdc_as_int32_1d_(this) result(res)
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
    end function hdc_as_int32_1d_

    subroutine hdc_as_int32_1d_sub(this, res)
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
    end subroutine hdc_as_int32_1d_sub


    function C_to_F_string(c_string_pointer) result(f_string)
        use, intrinsic :: iso_c_binding, only: c_ptr,c_f_pointer,c_char,c_null_char
        type(c_ptr), intent(in) :: c_string_pointer
        character(len=:), allocatable :: f_string
        character(kind=c_char), dimension(:), pointer :: char_array_pointer => null()
        character(len=255) :: aux_string
        integer :: i,length
        call c_f_pointer(c_string_pointer,char_array_pointer,[255])
        if (.not.associated(char_array_pointer)) then
        allocate(character(len=4)::f_string); f_string="NULL"; return
        end if
        aux_string=" "
        do i=1,255
        if (char_array_pointer(i)==c_null_char) then
            length=i-1; exit
        end if
        aux_string(i:i)=char_array_pointer(i)
        end do
        allocate(character(len=length)::f_string)
        f_string=aux_string(1:length)
    end function C_to_F_string

    function hdc_as_string_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        type(c_ptr) :: char_ptr
        character(len=:),allocatable :: res
        char_ptr = c_hdc_as_voidptr(this)
        res = C_to_F_string(char_ptr)
    end function hdc_as_string_

    subroutine hdc_as_string_sub(this, res)
        type(hdc_t) :: this
        type(c_ptr) :: char_ptr
        character(len=:),allocatable :: res
        char_ptr = c_hdc_as_voidptr(this)
        res = C_to_F_string(char_ptr)
    end subroutine hdc_as_string_sub

    function hdc_as_int32_2d_(this) result(res)
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
    end function hdc_as_int32_2d_

    subroutine hdc_as_int32_2d_sub(this, res)
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
    end subroutine hdc_as_int32_2d_sub

    function hdc_as_double_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=dp) :: res
        res = c_hdc_as_double_scalar(this)
    end function hdc_as_double_

    function hdc_as_float_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        real(kind=sp) :: res
        res = c_hdc_as_float_scalar(this)
    end function hdc_as_float_

    subroutine hdc_as_double_sub(this, res)
        type(hdc_t) :: this
        real(kind=dp) :: res
        res = c_hdc_as_double_scalar(this)
    end subroutine hdc_as_double_sub

    subroutine hdc_as_float_sub(this, res)
        type(hdc_t) :: this
        real(kind=sp) :: res
        res = hdc_as_float_(this)
    end subroutine hdc_as_float_sub

    function hdc_as_double_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        real(kind=dp) :: res
        res = c_hdc_as_double_scalar_path(this, trim(path)//c_null_char)
    end function hdc_as_double_path

    function hdc_as_float_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        real(kind=sp) :: res
        res = c_hdc_as_float_scalar_path(this, trim(path)//c_null_char)
    end function hdc_as_float_path

    subroutine hdc_as_double_path_sub(this, path, res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        real(kind=dp) :: res
        res = hdc_as_double_path(this,path)
    end subroutine hdc_as_double_path_sub

    subroutine hdc_as_float_path_sub(this, path, res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        real(kind=sp) :: res
        res = hdc_as_float_path(this,path)
    end subroutine hdc_as_float_path_sub

    function hdc_as_int8_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: res
        res = c_hdc_as_int8_scalar_path(this, trim(path)//c_null_char)
    end function hdc_as_int8_path

    subroutine hdc_as_int8_path_sub(this, path, res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: res
        res = hdc_as_int8_path(this,trim(path)//c_null_char)
    end subroutine hdc_as_int8_path_sub

    function hdc_as_int32_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int32_t) :: res
        res = c_hdc_as_int32_scalar_path(this, trim(path)//c_null_char)
    end function hdc_as_int32_path

    subroutine hdc_as_int32_path_sub(this, path, res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int32_t) :: res
        res = hdc_as_int32_path(this,trim(path)//c_null_char)
    end subroutine hdc_as_int32_path_sub

    function hdc_as_int8_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: res
        res = c_hdc_as_int8_scalar(this)
    end function hdc_as_int8_

    subroutine hdc_as_int8_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: res
        res = hdc_as_int8_(this)
    end subroutine hdc_as_int8_sub

    function hdc_as_int32_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t) :: res
        res = c_hdc_as_int32_scalar(this)
    end function hdc_as_int32_

    subroutine hdc_as_int32_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int32_t) :: res
        res = hdc_as_int32_(this)
    end subroutine hdc_as_int32_sub

    function hdc_new_dtype(ndim, shape_, type_str) result(res)
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), target :: shape_
        character(len=*), intent(in) :: type_str
        type(hdc_t) :: res
        res = c_hdc_new_dtype(ndim, c_loc(shape_), trim(type_str)//c_null_char)
    end function hdc_new_dtype

    function hdc_new_string(str) result(res)
        character(len=*), intent(in) :: str
        type(hdc_t) :: res
        res = c_hdc_new_string(trim(str)//c_null_char)
    end function hdc_new_string

    function hdc_as_double_1d_(this) result(res)
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
    end function hdc_as_double_1d_

    subroutine hdc_as_double_1d_sub(this, res)
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
    end subroutine hdc_as_double_1d_sub

    function hdc_as_double_2d_(this) result(res)
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
    end function hdc_as_double_2d_

    subroutine hdc_as_double_2d_sub(this,res)
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
    end subroutine hdc_as_double_2d_sub

    function hdc_as_double_2d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=dp), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_double_2d_path

    subroutine hdc_as_double_2d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=dp), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_double_2d_path_sub

    function hdc_as_double_1d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=dp), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_double_1d_path

    subroutine hdc_as_double_1d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=dp), dimension(:), pointer, intent(inout) :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_double_1d_path_sub

    function hdc_as_float_1d_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=sp), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_float_1d_

    subroutine hdc_as_float_1d_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=sp), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_float_1d_sub

    function hdc_as_float_2d_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=sp), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_float_2d_

    subroutine hdc_as_float_2d_sub(this,res)
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=sp), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_float_2d_sub

    function hdc_as_float_2d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=sp), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_float_2d_path

    subroutine hdc_as_float_2d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=sp), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_float_2d_path_sub

    function hdc_as_float_1d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=sp), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_float_1d_path

    subroutine hdc_as_float_1d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        real(kind=sp), dimension(:), pointer, intent(inout) :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_float_1d_path_sub

    function hdc_as_int8_2d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int8_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_int8_2d_path

    subroutine hdc_as_int8_2d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int8_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_int8_2d_path_sub

    function hdc_as_int8_1d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int8_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_int8_1d_path

    subroutine hdc_as_int8_1d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int8_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_int8_1d_path_sub

    function hdc_as_int32_2d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int32_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int32_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_int32_2d_path

    subroutine hdc_as_int32_2d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int32_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int32_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_int32_2d_path_sub

    function hdc_as_int32_1d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int32_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int32_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_int32_1d_path

    subroutine hdc_as_int32_1d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int32_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int32_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_int32_1d_path_sub

    function hdc_as_string_path(this,path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        type(c_ptr) :: char_ptr
        character(len=*), intent(in) :: path
        character(len=:), allocatable :: res
        char_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        res = C_to_F_string(char_ptr)
    end function hdc_as_string_path

    subroutine hdc_as_string_path_sub(this, path, res)
        type(hdc_t) :: this
        type(c_ptr) :: char_ptr
        character(len=*), intent(in) :: path
        character(len=*) :: res
        char_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        res = C_to_F_string(char_ptr)
    end subroutine hdc_as_string_path_sub

! !     subroutine hdc_as_string_path_sub(this, path, res)
! !         use iso_c_binding
! !         type(hdc_t) :: this
! !         type(c_ptr) :: char_ptr
! !         character(len=*), intent(in) :: path
! !         integer(c_int) :: strlen
! !         character(len=*) :: res
! !         call c_hdc_as_string_path_fortran(this,trim(path)//c_null_char,res,cloc(strlen))
! !         char_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
! !         res = C_to_F_string(char_ptr)
! !     end subroutine hdc_as_string_path_sub

    subroutine hdc_to_json(this,path,mode)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int32_t) :: mode
        call c_hdc_to_json(this,trim(path)//c_null_char, mode)
    end subroutine hdc_to_json


    subroutine hdc_new_pokus(res)
        use iso_c_binding
        type(hdc_t), target :: node
        type(c_ptr) :: res
        node = hdc_new_empty()
        res = c_loc(node)
    end subroutine hdc_new_pokus

    function hdc_new_ptr() result(res)
        use iso_c_binding
        type(hdc_t), pointer :: struct
        type(c_ptr) :: res
        allocate(struct)
        struct%obj = c_hdc_new_void_ptr()
        res = c_loc(struct)
    end function hdc_new_ptr

    subroutine hello_fort()
        print *, "Hello from fortran..."
    end subroutine hello_fort

    function hdc_get_ptr_f(tree) result(res)
        use iso_c_binding
        type(hdc_t), target :: tree
        type(c_ptr) :: res
        res = c_loc(tree)
    end function hdc_get_ptr_f


    !> Destroy HDC
    subroutine hdc_destroy() bind(c,name="HDC_destroy_c")
        use iso_c_binding
    end subroutine hdc_destroy

    !> Init HDC
    subroutine hdc_init_(pluginFileName, pluginSettingsString)
        use iso_c_binding
        character(kind=c_char,len=*), optional :: pluginFileName
        character(kind=c_char,len=*), optional :: pluginSettingsString
        if (.not.present(pluginFileName)) pluginFileName = ""
        if (.not.present(pluginSettingsString)) pluginSettingsString = ""
        call c_hdc_init(trim(pluginFileName)//c_null_char,trim(pluginSettingsString)//c_null_char)
    end subroutine hdc_init_



    subroutine hdc_set_int64_1d(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int64_t), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_ ! Won't compile on gfortran-4.8
!        integer(kind=c_long), dimension(1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_int64(this, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_int64_1d

    subroutine hdc_set_int64_scalar(this, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int64_t) :: data
        call c_hdc_set_int64_scalar(this, data)
    end subroutine hdc_set_int64_scalar

    subroutine hdc_set_int64_scalar_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int64_t) :: data
        character(len=*), intent(in) :: path
        call c_hdc_set_int64_scalar_path(this, trim(path)//c_null_char, data)
    end subroutine hdc_set_int64_scalar_path

    subroutine hdc_set_int64_1d_path(this, path, data)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int64_t), dimension(:), target :: data
        integer(kind=c_long), dimension(1:1), target :: shape_
        type(c_ptr) :: data_ptr, shape_ptr
        integer(1) :: ndim = 1
        character(len=*), intent(in) :: path
        shape_ = shape(data)
        data_ptr = c_loc(data)
        shape_ptr = c_loc(shape_)
        call c_hdc_set_int64_path(this, trim(path)//c_null_char, ndim, shape_ptr, data_ptr)
    end subroutine hdc_set_int64_1d_path

    function hdc_as_int64_1d_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int64_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_int64_1d_

    subroutine hdc_as_int64_1d_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int64_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_int64_1d_sub

    function hdc_as_int64_2d_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int64_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_int64_2d_

    subroutine hdc_as_int64_2d_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int8_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int64_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim(this)
        shape_ptr = c_hdc_get_shape(this)
        data_ptr = c_hdc_as_voidptr(this)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_int64_2d_sub

    function hdc_as_int64_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int64_t) :: res
        res = c_hdc_as_int64_scalar_path(this, trim(path)//c_null_char)
    end function hdc_as_int64_path

    subroutine hdc_as_int64_path_sub(this, path, res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int64_t) :: res
        res = hdc_as_int64_path(this,trim(path)//c_null_char)
    end subroutine hdc_as_int64_path_sub

    function hdc_as_int64_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int64_t) :: res
        res = c_hdc_as_int64_scalar(this)
    end function hdc_as_int64_

    subroutine hdc_as_int64_sub(this, res)
        type(hdc_t) :: this
        integer(kind=c_int64_t) :: res
        res = hdc_as_int64_(this)
    end subroutine hdc_as_int64_sub


    function hdc_as_int64_2d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int64_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int64_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_int64_2d_path

    subroutine hdc_as_int64_2d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int64_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int64_t), dimension(:,:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_int64_2d_path_sub

    function hdc_as_int64_1d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int64_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int64_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end function hdc_as_int64_1d_path

    subroutine hdc_as_int64_1d_path_sub(this,path,res)
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        integer(kind=c_int64_t) :: ndim
        integer(kind=c_long), dimension(:), pointer :: shape_
        type(c_ptr) :: shape_ptr, data_ptr
        integer(kind=c_int64_t), dimension(:), pointer :: res
        ndim = c_hdc_get_ndim_path(this,trim(path)//c_null_char)
        shape_ptr = c_hdc_get_shape_path(this,trim(path)//c_null_char)
        data_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        call c_f_pointer(shape_ptr, shape_, (/ ndim /))
        call c_f_pointer(data_ptr, res, shape_)
    end subroutine hdc_as_int64_1d_path_sub

!     function c_to_f_string(s) result(str)
!         use iso_c_binding
!         character(kind=c_char,len=1), intent(in) :: s(*)
!         character(len=:), allocatable :: str
!         integer i, nchars
!         i = 1
!         do
!             if (s(i) == c_null_char) exit
!             i = i + 1
!         end do
!         nchars = i - 1  ! Exclude null character from Fortran string
!         allocate(character(len=nchars) :: str)
!         str = transfer(s(1:nchars), str)
!     end function c_to_f_string

end module hdc_fortran
! http://fortranwiki.org/fortran/show/Fortran+and+Cpp+objs
! https://gcc.gnu.org/onlinedocs/gfortran/Derived-Types-and-struct.html
