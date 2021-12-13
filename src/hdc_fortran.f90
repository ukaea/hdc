
! This file was generated on 2021-07-26 13:29:59.076887 by generate_fortran_api.py
! Please, edit the hdc_fortran.f90.template file instead and run the python script.


module hdc_fortran
    use iso_c_binding
    implicit none
    type, bind(c) :: hdc_t
        character(kind=c_char) :: uuid(16)
        integer(kind=c_size_t) :: storage_id
    end type hdc_t

    integer, parameter :: HDC_MAX_DIMS = 10

    type, bind(c) :: hdc_data_t
        integer(kind=c_size_t) :: dtype
        integer(kind=c_size_t) :: flags
        integer(kind=c_size_t) :: rank
        integer(kind=c_size_t) :: dshape(HDC_MAX_DIMS)
        type(c_ptr) :: data !< data pointer
    end type hdc_data_t

    integer, parameter :: dp=kind(1.0D0)
    integer, parameter :: sp=kind(1.0)

    integer(kind=c_size_t), parameter ::  HDCDefault             = 0_8
    integer(kind=c_size_t), parameter ::  HDCFortranOrder        = 1_8
    integer(kind=c_size_t), parameter ::  HDCReadOnly            = 2_8
    integer(kind=c_size_t), parameter ::  HDCExternal            = 4_8
    integer(kind=c_size_t), parameter ::  HDCChildrenInitialized = 8_8

    integer(kind=c_size_t), parameter ::  HDC_EMPTY  =  0
    integer(kind=c_size_t), parameter ::  HDC_STRUCT =  1
    integer(kind=c_size_t), parameter ::  HDC_LIST   =  2
    integer(kind=c_size_t), parameter ::  HDC_INT8   =  3
    integer(kind=c_size_t), parameter ::  HDC_INT16  =  4
    integer(kind=c_size_t), parameter ::  HDC_INT32  =  5
    integer(kind=c_size_t), parameter ::  HDC_INT64  =  6
    integer(kind=c_size_t), parameter ::  HDC_UINT8  =  7
    integer(kind=c_size_t), parameter ::  HDC_UINT16 =  8
    integer(kind=c_size_t), parameter ::  HDC_UINT32 =  9
    integer(kind=c_size_t), parameter ::  HDC_UINT64 = 10
    integer(kind=c_size_t), parameter ::  HDC_FLOAT  = 11
    integer(kind=c_size_t), parameter ::  HDC_DOUBLE = 12
    integer(kind=c_size_t), parameter ::  HDC_STRING = 13
    integer(kind=c_size_t), parameter ::  HDC_BOOL   = 14
    integer(kind=c_size_t), parameter ::  HDC_ERROR  = 15

    private
    interface
        subroutine hello() bind(c,name="hello")
            use iso_c_binding
        end subroutine hello

        function c_hdc_new_empty() result(obj) bind(c,name="hdc_new_empty")
        ! Default constructor. This is interface to C.
            import
            type(hdc_t) :: obj
        end function c_hdc_new_empty

        function c_hdc_new_size(size) result(obj) bind(c,name="hdc_new_size")
        ! Constructor. This is interface to C.
            import
            integer(kind=c_size_t),value :: size ! size in bytes
            type(hdc_t) :: obj
        end function c_hdc_new_size

        function c_hdc_new_string(str) result(obj) bind(c,name="hdc_new_string")
        ! Constructs HDC from string/uri
            import
            type(hdc_t) :: obj
            character(kind=c_char), intent(in) :: str(*) ! URI/string
        end function c_hdc_new_string

        function c_hdc_new_array(rank, shape_, type_str) result(obj) bind(c,name="hdc_new_array")
        ! Construct empty array of type given by string.
            import
            type(hdc_t) :: obj
            integer(kind=c_size_t),value :: rank ! array rank
            type(c_ptr), value :: shape_ ! array shape
            character(kind=c_char), intent(in) :: type_str(*) ! type string - e.g. int8/uint16/float/double/bool/string
        end function c_hdc_new_array

        subroutine hdc_clean(obj) bind(c,name="hdc_clean")
        ! Desctructor. This is interface to C.
           import
           type(hdc_t), value :: obj ! HDC object
        end subroutine hdc_clean

        function c_hdc_copy(src, deep_copy) result(obj) bind(c,name="hdc_copy")
        ! Performs deep copy of current node. This is interface to C.
            import
            type(hdc_t), value :: src
            logical(kind=c_bool), value :: deep_copy
            type(hdc_t) :: obj
        end function c_hdc_copy

        subroutine c_hdc_add_child(obj, path, node) bind(c,name="hdc_add_child")
        ! Adds HDC subtree as child with given path. If neccessary, recursively creates subnodes. This is interface to C.
            import
            type(hdc_t), value :: obj ! HDC object
            character(kind=c_char), intent(in) :: path(*)
            type(hdc_t), value :: node
        end subroutine c_hdc_add_child

        subroutine c_hdc_init(storagePluginFileName,storagePluginSettingsFileName) bind(c,name="hdc_init")
        ! Initialization of HDC library
            import
            character(kind=c_char), intent(in) :: storagePluginFileName(*) ! Filename of HDC storage plugin to be used.
            character(kind=c_char), intent(in) :: storagePluginSettingsFileName(*) ! Filename of additional settings for provided storarage plugin. The file should be JSON encoded.
        end subroutine c_hdc_init

        subroutine hdc_set_slice(obj, pos, node) bind(c,name="hdc_set_slice")
        ! Sets slice to the given position. This is interface to C.
            import
            type(hdc_t), value :: obj ! HDC object
            integer(kind=c_int32_t), value     :: pos ! position/index
            type(hdc_t), value :: node ! HDC object to be stored
        end subroutine hdc_set_slice

        subroutine hdc_insert_slice(obj, pos, node) bind(c,name="hdc_insert_slice")
        ! Inserts slice to the given position. This is interface to C.
            import
            type(hdc_t), value :: obj ! HDC object
            integer(kind=c_int32_t), value     :: pos
            type(hdc_t), value :: node ! HDC object to be stored
        end subroutine hdc_insert_slice

        subroutine hdc_append_slice(obj, node) bind(c,name="hdc_append_slice")
        ! Appends slice to the end of list. This is interface to C.
            import
            type(hdc_t), value :: obj ! HDC object
            type(hdc_t), value :: node ! HDC object to be stored
        end subroutine hdc_append_slice

!        subroutine c_hdc_to_json(obj,path,mode) bind(c,name="hdc_to_json")
!            import
!            type(hdc_t), value :: obj ! HDC object
!            character(kind=c_char), intent(in) :: path(*)
!            integer(kind=c_int32_t), value :: mode
!       end subroutine c_hdc_to_json

        function c_hdc_get_child(obj, path) result(res) bind(c,name="hdc_get")
        ! Returns HDC subtree by given path. This is interface to C.
            import
            type(hdc_t), value :: obj ! HDC object
            character(kind=c_char) :: path(*)
            type(hdc_t) :: res
        end function c_hdc_get_child

        function c_hdc_get_slice_path(obj, path, i) result(res) bind(c,name="hdc_get_slice")
        ! Returns HDC subtree by given path and slice index. This is interface to C.
            import
            type(hdc_t), value :: obj ! HDC object
            character(kind=c_char) :: path(*)
            integer(kind=c_size_t) :: i
            type(hdc_t) :: res
        end function c_hdc_get_slice_path

        function c_hdc_get_slice(obj, i) result(res) bind(c,name="hdc_get_slice")
        ! Returns HDC subtree by given slice index.  This is interface to C.
            import
            type(hdc_t), value :: obj ! HDC object
            integer(kind=c_size_t) :: i
            type(hdc_t) :: res
        end function c_hdc_get_slice

        subroutine c_hdc_set_child(obj, path, node) bind(c,name="hdc_set_child")
        ! Sets HDC subtree to given path.
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(hdc_t), value:: node
        end subroutine c_hdc_set_child

        subroutine c_hdc_delete_child(obj, path) bind(c,name="hdc_delete_child")
        ! Recursively deletes subtree at given path. This is interface to C.
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
        end subroutine c_hdc_delete_child

        function c_hdc_exists(obj, path) result(res) bind(c,name="hdc_exists")
        ! Returns true if subtree with given path exists. This is interface to C.
            import
            type(hdc_t), value :: obj ! HDC object
            character(kind=c_char), intent(in) :: path(*)
            logical(kind=c_bool) :: res ! change this to c_bool later
        end function c_hdc_exists

        function c_hdc_is_scalar(obj, path) result(res) bind(c,name="hdc_is_scalar")
            ! Returns true if subtree with given path contains scala data. This is interface to C.
                import
                type(hdc_t), value :: obj ! HDC object
                character(kind=c_char), intent(in) :: path(*)
                logical(kind=c_bool) :: res ! change this to c_bool later
        end function c_hdc_is_scalar

        function c_hdc_is_external(obj, path) result(res) bind(c,name="hdc_is_external")
            ! Returns true if subtree with given path contains external data. This is interface to C.
                import
                type(hdc_t), value :: obj ! HDC object
                character(kind=c_char), intent(in) :: path(*)
                logical(kind=c_bool) :: res ! change this to c_bool later
        end function c_hdc_is_external

        subroutine c_hdc_set_string(obj, path, str) bind(c,name="hdc_set_string")
        ! Sets string to given path. This is interface to C.
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            character(kind=c_char), intent(in) :: str(*)
        end subroutine c_hdc_set_string

        function c_hdc_as_voidptr(obj) result(res) bind(c,name="hdc_as_voidptr")
        ! Sets arbitrary data casted to void pointer. This is interface to C.
            import
            type(hdc_t), value:: obj
            type(c_ptr) :: res
        end function c_hdc_as_voidptr

        function c_hdc_get_data(obj,path) result(res) bind(c,name="hdc_get_data")
        ! Gets hdc_data_t object. This is interface to C.
            import
            character(kind=c_char), intent(in) :: path(*)
            type(hdc_t), value:: obj
            type(hdc_data_t) :: res
        end function c_hdc_get_data

        subroutine c_hdc_set_data(obj,path,data) bind(c,name="hdc_set_data")
        ! Sets hdc_data_t object. This is interface to C.
            import
            character(kind=c_char), intent(in) :: path(*)
            type(hdc_t), value:: obj
            type(hdc_data_t),value :: data
        end subroutine c_hdc_set_data

        subroutine c_hdc_set_external(obj,path,data) bind(c,name="hdc_set_external")
        ! Sets hdc_data_t object. This is interface to C.
            import
            character(kind=c_char), intent(in) :: path(*)
            type(hdc_t), value:: obj
            type(hdc_data_t),value :: data
        end subroutine c_hdc_set_external

        subroutine c_hdc_as_string_fortran(obj,path,str,strlen)  bind(c,name="hdc_as_string_fortran")
        ! Sets arbitrary data casted to void pointer. This is interface to C.
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(c_int) :: strlen
            character(kind=c_char) :: str(*)
        end subroutine c_hdc_as_string_fortran

        function c_hdc_as_voidptr_path(obj, path) result(res) bind(c,name="hdc_as_voidptr")
        ! Sets arbitrary data casted to void pointer to given path. This is interface to C.
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr) :: res
        end function c_hdc_as_voidptr_path

        subroutine hdc_dump(obj) bind(c,name="hdc_dump")
        ! Dumps HDC(sub)tree into screen
            import
            type(hdc_t), value:: obj
        end subroutine hdc_dump

        subroutine hdc_print_info(obj) bind(c,name="hdc_print_info")
            import
            type(hdc_t), value:: obj
        end subroutine hdc_print_info

        subroutine c_hdc_set_scalar(obj, path, data, type_) bind(c,name="hdc_set_scalar")
        ! Sets scalar. This is interface to C.
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            type(c_ptr), value :: data
            integer(kind=c_size_t),value :: type_
        end subroutine c_hdc_set_scalar

        function c_hdc_as_int8_scalar(obj, path) result(res) bind(c,name="hdc_as_int8_scalar")
            ! Returns scalar int8
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int8_t) :: res
        end function c_hdc_as_int8_scalar
        function c_hdc_as_int16_scalar(obj, path) result(res) bind(c,name="hdc_as_int16_scalar")
            ! Returns scalar int16
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int16_t) :: res
        end function c_hdc_as_int16_scalar
        function c_hdc_as_int32_scalar(obj, path) result(res) bind(c,name="hdc_as_int32_scalar")
            ! Returns scalar int32
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int32_t) :: res
        end function c_hdc_as_int32_scalar
        function c_hdc_as_int64_scalar(obj, path) result(res) bind(c,name="hdc_as_int64_scalar")
            ! Returns scalar int64
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            integer(kind=c_int64_t) :: res
        end function c_hdc_as_int64_scalar
        function c_hdc_as_float_scalar(obj, path) result(res) bind(c,name="hdc_as_float_scalar")
            ! Returns scalar float
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            real(kind=sp) :: res
        end function c_hdc_as_float_scalar
        function c_hdc_as_double_scalar(obj, path) result(res) bind(c,name="hdc_as_double_scalar")
            ! Returns scalar double
            import
            type(hdc_t), value:: obj
            character(kind=c_char), intent(in) :: path(*)
            real(kind=dp) :: res
        end function c_hdc_as_double_scalar
    end interface

    interface hdc_get_shape
!         module procedure hdc_get_shape_
        module procedure hdc_get_shape_pos
        module procedure hdc_get_shape_path
    end interface hdc_get_shape

    interface hdc_new
        module procedure hdc_new_dt
        module procedure hdc_new_size
    end interface hdc_new


    interface hdc_set_data
    ! Generic set interface.
        module procedure hdc_set_string
        module procedure hdc_set_string_path
        module procedure hdc_set_data_
        module procedure hdc_set_data_path
        module procedure hdc_set_int8_scalar_path
        module procedure hdc_set_int8_scalar
        module procedure hdc_set_int8_1d
        module procedure hdc_set_int8_1d_path
        module procedure hdc_set_int8_2d
        module procedure hdc_set_int8_2d_path
        module procedure hdc_set_int8_3d
        module procedure hdc_set_int8_3d_path
        module procedure hdc_set_int8_4d
        module procedure hdc_set_int8_4d_path
        module procedure hdc_set_int8_5d
        module procedure hdc_set_int8_5d_path
        module procedure hdc_set_int8_6d
        module procedure hdc_set_int8_6d_path
        module procedure hdc_set_int8_7d
        module procedure hdc_set_int8_7d_path
        module procedure hdc_set_int16_scalar_path
        module procedure hdc_set_int16_scalar
        module procedure hdc_set_int16_1d
        module procedure hdc_set_int16_1d_path
        module procedure hdc_set_int16_2d
        module procedure hdc_set_int16_2d_path
        module procedure hdc_set_int16_3d
        module procedure hdc_set_int16_3d_path
        module procedure hdc_set_int16_4d
        module procedure hdc_set_int16_4d_path
        module procedure hdc_set_int16_5d
        module procedure hdc_set_int16_5d_path
        module procedure hdc_set_int16_6d
        module procedure hdc_set_int16_6d_path
        module procedure hdc_set_int16_7d
        module procedure hdc_set_int16_7d_path
        module procedure hdc_set_int32_scalar_path
        module procedure hdc_set_int32_scalar
        module procedure hdc_set_int32_1d
        module procedure hdc_set_int32_1d_path
        module procedure hdc_set_int32_2d
        module procedure hdc_set_int32_2d_path
        module procedure hdc_set_int32_3d
        module procedure hdc_set_int32_3d_path
        module procedure hdc_set_int32_4d
        module procedure hdc_set_int32_4d_path
        module procedure hdc_set_int32_5d
        module procedure hdc_set_int32_5d_path
        module procedure hdc_set_int32_6d
        module procedure hdc_set_int32_6d_path
        module procedure hdc_set_int32_7d
        module procedure hdc_set_int32_7d_path
        module procedure hdc_set_int64_scalar_path
        module procedure hdc_set_int64_scalar
        module procedure hdc_set_int64_1d
        module procedure hdc_set_int64_1d_path
        module procedure hdc_set_int64_2d
        module procedure hdc_set_int64_2d_path
        module procedure hdc_set_int64_3d
        module procedure hdc_set_int64_3d_path
        module procedure hdc_set_int64_4d
        module procedure hdc_set_int64_4d_path
        module procedure hdc_set_int64_5d
        module procedure hdc_set_int64_5d_path
        module procedure hdc_set_int64_6d
        module procedure hdc_set_int64_6d_path
        module procedure hdc_set_int64_7d
        module procedure hdc_set_int64_7d_path
        module procedure hdc_set_float_scalar_path
        module procedure hdc_set_float_scalar
        module procedure hdc_set_float_1d
        module procedure hdc_set_float_1d_path
        module procedure hdc_set_float_2d
        module procedure hdc_set_float_2d_path
        module procedure hdc_set_float_3d
        module procedure hdc_set_float_3d_path
        module procedure hdc_set_float_4d
        module procedure hdc_set_float_4d_path
        module procedure hdc_set_float_5d
        module procedure hdc_set_float_5d_path
        module procedure hdc_set_float_6d
        module procedure hdc_set_float_6d_path
        module procedure hdc_set_float_7d
        module procedure hdc_set_float_7d_path
        module procedure hdc_set_double_scalar_path
        module procedure hdc_set_double_scalar
        module procedure hdc_set_double_1d
        module procedure hdc_set_double_1d_path
        module procedure hdc_set_double_2d
        module procedure hdc_set_double_2d_path
        module procedure hdc_set_double_3d
        module procedure hdc_set_double_3d_path
        module procedure hdc_set_double_4d
        module procedure hdc_set_double_4d_path
        module procedure hdc_set_double_5d
        module procedure hdc_set_double_5d_path
        module procedure hdc_set_double_6d
        module procedure hdc_set_double_6d_path
        module procedure hdc_set_double_7d
        module procedure hdc_set_double_7d_path
    end interface hdc_set_data

    interface hdc_set_external
    ! Genneric set_external interface.
        module procedure hdc_set_external_path
        module procedure hdc_set_external_
    end interface hdc_set_external

    interface hdc_set
    ! Generic set interface.
        module procedure hdc_set_child
        module procedure hdc_set_string
        module procedure hdc_set_string_path
        module procedure hdc_set_data_
        module procedure hdc_set_data_path
        module procedure hdc_set_int8_scalar_path
        module procedure hdc_set_int8_scalar
        module procedure hdc_set_int8_1d
        module procedure hdc_set_int8_1d_path
        module procedure hdc_set_int8_2d
        module procedure hdc_set_int8_2d_path
        module procedure hdc_set_int8_3d
        module procedure hdc_set_int8_3d_path
        module procedure hdc_set_int8_4d
        module procedure hdc_set_int8_4d_path
        module procedure hdc_set_int8_5d
        module procedure hdc_set_int8_5d_path
        module procedure hdc_set_int8_6d
        module procedure hdc_set_int8_6d_path
        module procedure hdc_set_int8_7d
        module procedure hdc_set_int8_7d_path
        module procedure hdc_set_int16_scalar_path
        module procedure hdc_set_int16_scalar
        module procedure hdc_set_int16_1d
        module procedure hdc_set_int16_1d_path
        module procedure hdc_set_int16_2d
        module procedure hdc_set_int16_2d_path
        module procedure hdc_set_int16_3d
        module procedure hdc_set_int16_3d_path
        module procedure hdc_set_int16_4d
        module procedure hdc_set_int16_4d_path
        module procedure hdc_set_int16_5d
        module procedure hdc_set_int16_5d_path
        module procedure hdc_set_int16_6d
        module procedure hdc_set_int16_6d_path
        module procedure hdc_set_int16_7d
        module procedure hdc_set_int16_7d_path
        module procedure hdc_set_int32_scalar_path
        module procedure hdc_set_int32_scalar
        module procedure hdc_set_int32_1d
        module procedure hdc_set_int32_1d_path
        module procedure hdc_set_int32_2d
        module procedure hdc_set_int32_2d_path
        module procedure hdc_set_int32_3d
        module procedure hdc_set_int32_3d_path
        module procedure hdc_set_int32_4d
        module procedure hdc_set_int32_4d_path
        module procedure hdc_set_int32_5d
        module procedure hdc_set_int32_5d_path
        module procedure hdc_set_int32_6d
        module procedure hdc_set_int32_6d_path
        module procedure hdc_set_int32_7d
        module procedure hdc_set_int32_7d_path
        module procedure hdc_set_int64_scalar_path
        module procedure hdc_set_int64_scalar
        module procedure hdc_set_int64_1d
        module procedure hdc_set_int64_1d_path
        module procedure hdc_set_int64_2d
        module procedure hdc_set_int64_2d_path
        module procedure hdc_set_int64_3d
        module procedure hdc_set_int64_3d_path
        module procedure hdc_set_int64_4d
        module procedure hdc_set_int64_4d_path
        module procedure hdc_set_int64_5d
        module procedure hdc_set_int64_5d_path
        module procedure hdc_set_int64_6d
        module procedure hdc_set_int64_6d_path
        module procedure hdc_set_int64_7d
        module procedure hdc_set_int64_7d_path
        module procedure hdc_set_float_scalar_path
        module procedure hdc_set_float_scalar
        module procedure hdc_set_float_1d
        module procedure hdc_set_float_1d_path
        module procedure hdc_set_float_2d
        module procedure hdc_set_float_2d_path
        module procedure hdc_set_float_3d
        module procedure hdc_set_float_3d_path
        module procedure hdc_set_float_4d
        module procedure hdc_set_float_4d_path
        module procedure hdc_set_float_5d
        module procedure hdc_set_float_5d_path
        module procedure hdc_set_float_6d
        module procedure hdc_set_float_6d_path
        module procedure hdc_set_float_7d
        module procedure hdc_set_float_7d_path
        module procedure hdc_set_double_scalar_path
        module procedure hdc_set_double_scalar
        module procedure hdc_set_double_1d
        module procedure hdc_set_double_1d_path
        module procedure hdc_set_double_2d
        module procedure hdc_set_double_2d_path
        module procedure hdc_set_double_3d
        module procedure hdc_set_double_3d_path
        module procedure hdc_set_double_4d
        module procedure hdc_set_double_4d_path
        module procedure hdc_set_double_5d
        module procedure hdc_set_double_5d_path
        module procedure hdc_set_double_6d
        module procedure hdc_set_double_6d_path
        module procedure hdc_set_double_7d
        module procedure hdc_set_double_7d_path
    end interface hdc_set

    interface hdc_get_slice
       module procedure hdc_get_slice_
       module procedure hdc_get_slice_l
       module procedure hdc_get_slice_path
    end interface hdc_get_slice

   interface hdc_get
   ! Generic hdc_get interface
       module procedure hdc_get_child_sub
       module procedure hdc_as_string_sub
       module procedure hdc_as_string_path_sub
       module procedure hdc_get_slice_path_sub
       module procedure hdc_as_int8_1d_sub
       module procedure hdc_as_int8_1d_path_sub
       module procedure hdc_as_int8_2d_sub
       module procedure hdc_as_int8_2d_path_sub
       module procedure hdc_as_int8_3d_sub
       module procedure hdc_as_int8_3d_path_sub
       module procedure hdc_as_int8_4d_sub
       module procedure hdc_as_int8_4d_path_sub
       module procedure hdc_as_int8_5d_sub
       module procedure hdc_as_int8_5d_path_sub
       module procedure hdc_as_int8_6d_sub
       module procedure hdc_as_int8_6d_path_sub
       module procedure hdc_as_int8_7d_sub
       module procedure hdc_as_int8_7d_path_sub
       module procedure hdc_as_int8_sub
       module procedure hdc_as_int8_path_sub
       module procedure hdc_as_int16_1d_sub
       module procedure hdc_as_int16_1d_path_sub
       module procedure hdc_as_int16_2d_sub
       module procedure hdc_as_int16_2d_path_sub
       module procedure hdc_as_int16_3d_sub
       module procedure hdc_as_int16_3d_path_sub
       module procedure hdc_as_int16_4d_sub
       module procedure hdc_as_int16_4d_path_sub
       module procedure hdc_as_int16_5d_sub
       module procedure hdc_as_int16_5d_path_sub
       module procedure hdc_as_int16_6d_sub
       module procedure hdc_as_int16_6d_path_sub
       module procedure hdc_as_int16_7d_sub
       module procedure hdc_as_int16_7d_path_sub
       module procedure hdc_as_int16_sub
       module procedure hdc_as_int16_path_sub
       module procedure hdc_as_int32_1d_sub
       module procedure hdc_as_int32_1d_path_sub
       module procedure hdc_as_int32_2d_sub
       module procedure hdc_as_int32_2d_path_sub
       module procedure hdc_as_int32_3d_sub
       module procedure hdc_as_int32_3d_path_sub
       module procedure hdc_as_int32_4d_sub
       module procedure hdc_as_int32_4d_path_sub
       module procedure hdc_as_int32_5d_sub
       module procedure hdc_as_int32_5d_path_sub
       module procedure hdc_as_int32_6d_sub
       module procedure hdc_as_int32_6d_path_sub
       module procedure hdc_as_int32_7d_sub
       module procedure hdc_as_int32_7d_path_sub
       module procedure hdc_as_int32_sub
       module procedure hdc_as_int32_path_sub
       module procedure hdc_as_int64_1d_sub
       module procedure hdc_as_int64_1d_path_sub
       module procedure hdc_as_int64_2d_sub
       module procedure hdc_as_int64_2d_path_sub
       module procedure hdc_as_int64_3d_sub
       module procedure hdc_as_int64_3d_path_sub
       module procedure hdc_as_int64_4d_sub
       module procedure hdc_as_int64_4d_path_sub
       module procedure hdc_as_int64_5d_sub
       module procedure hdc_as_int64_5d_path_sub
       module procedure hdc_as_int64_6d_sub
       module procedure hdc_as_int64_6d_path_sub
       module procedure hdc_as_int64_7d_sub
       module procedure hdc_as_int64_7d_path_sub
       module procedure hdc_as_int64_sub
       module procedure hdc_as_int64_path_sub
       module procedure hdc_as_float_1d_sub
       module procedure hdc_as_float_1d_path_sub
       module procedure hdc_as_float_2d_sub
       module procedure hdc_as_float_2d_path_sub
       module procedure hdc_as_float_3d_sub
       module procedure hdc_as_float_3d_path_sub
       module procedure hdc_as_float_4d_sub
       module procedure hdc_as_float_4d_path_sub
       module procedure hdc_as_float_5d_sub
       module procedure hdc_as_float_5d_path_sub
       module procedure hdc_as_float_6d_sub
       module procedure hdc_as_float_6d_path_sub
       module procedure hdc_as_float_7d_sub
       module procedure hdc_as_float_7d_path_sub
       module procedure hdc_as_float_sub
       module procedure hdc_as_float_path_sub
       module procedure hdc_as_double_1d_sub
       module procedure hdc_as_double_1d_path_sub
       module procedure hdc_as_double_2d_sub
       module procedure hdc_as_double_2d_path_sub
       module procedure hdc_as_double_3d_sub
       module procedure hdc_as_double_3d_path_sub
       module procedure hdc_as_double_4d_sub
       module procedure hdc_as_double_4d_path_sub
       module procedure hdc_as_double_5d_sub
       module procedure hdc_as_double_5d_path_sub
       module procedure hdc_as_double_6d_sub
       module procedure hdc_as_double_6d_path_sub
       module procedure hdc_as_double_7d_sub
       module procedure hdc_as_double_7d_path_sub
       module procedure hdc_as_double_sub
       module procedure hdc_as_double_path_sub
   end interface hdc_get

    public ::   hdc_t, &
                hdc_data_t, &
                dp, &
                sp, &
                hdc_clean, &
                hdc_add_child, &
                hdc_get_child, &
                hdc_set_child, &
                hdc_exists, &
                hdc_delete_child, &
                hdc_set, &
                hdc_get_shape, &
                hdc_set_data, &
                hdc_set_external, &
                hdc_get_slice, &
                hdc_get, &
                hdc_copy, &
                hdc_dump, &
                hdc_get_ptr_f, &
                hdc_get_rank, &
                hdc_insert_slice, &
                hdc_append_slice, &
                hdc_set_slice, &
                hdc_new_array, &
                hdc_get_type, &
                hdc_destroy, &
                hdc_init, &
                hdc_is_scalar, &
                hdc_is_external, &
                hdc_as_string_sub, &
                hdc_as_string, &
                hdc_new_string, &
                hdc_print_info, &
                hdc_new, &
                hdc_as_int8, &
                hdc_as_int8_1d, &
                hdc_as_int8_2d, &
                hdc_as_int8_3d, &
                hdc_as_int8_4d, &
                hdc_as_int8_5d, &
                hdc_as_int8_6d, &
                hdc_as_int8_7d, &
                hdc_as_int16, &
                hdc_as_int16_1d, &
                hdc_as_int16_2d, &
                hdc_as_int16_3d, &
                hdc_as_int16_4d, &
                hdc_as_int16_5d, &
                hdc_as_int16_6d, &
                hdc_as_int16_7d, &
                hdc_as_int32, &
                hdc_as_int32_1d, &
                hdc_as_int32_2d, &
                hdc_as_int32_3d, &
                hdc_as_int32_4d, &
                hdc_as_int32_5d, &
                hdc_as_int32_6d, &
                hdc_as_int32_7d, &
                hdc_as_int64, &
                hdc_as_int64_1d, &
                hdc_as_int64_2d, &
                hdc_as_int64_3d, &
                hdc_as_int64_4d, &
                hdc_as_int64_5d, &
                hdc_as_int64_6d, &
                hdc_as_int64_7d, &
                hdc_as_float, &
                hdc_as_float_1d, &
                hdc_as_float_2d, &
                hdc_as_float_3d, &
                hdc_as_float_4d, &
                hdc_as_float_5d, &
                hdc_as_float_6d, &
                hdc_as_float_7d, &
                hdc_as_double, &
                hdc_as_double_1d, &
                hdc_as_double_2d, &
                hdc_as_double_3d, &
                hdc_as_double_4d, &
                hdc_as_double_5d, &
                hdc_as_double_6d, &
                hdc_as_double_7d, &
                hello
contains

    subroutine hdc_add_child(this, path, node)
    ! Adds child to the given node
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        type(hdc_t) :: node
        if (.not.present(path)) path = ""
        call c_hdc_add_child(this, trim(path)//c_null_char, node)
    end subroutine hdc_add_child

    subroutine hdc_delete_child(this, path)
    ! Deletes child on the given path
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        if (.not.present(path)) path = ""
        call c_hdc_delete_child(this, trim(path)//c_null_char)
    end subroutine hdc_delete_child

    function hdc_exists(this, path) result(res)
    ! returns true if a node under given path exists
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        logical(kind=c_bool) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_exists(this, trim(path)//c_null_char)
    end function hdc_exists

    function hdc_is_scalar(this, path) result(res)
        ! returns true if a node under given path contains scalar data
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        logical(kind=c_bool) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_is_scalar(this, trim(path)//c_null_char)
     end function hdc_is_scalar

    function hdc_is_external(this, path) result(res)
    ! returns true if a node under given path contains external data
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        logical(kind=c_bool) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_is_external(this, trim(path)//c_null_char)
    end function hdc_is_external

    function hdc_get_rank(this,path) result(res)
    ! Returns rank of stored array data
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        integer(kind=c_size_t) :: res
        type(hdc_data_t) :: data ! Encapsulated data
        if (.not.present(path)) path = ""
        data = hdc_get_data(this,path)
        res = data%rank
    end function hdc_get_rank

    function hdc_get_shape_path(this,path) result(res)
    ! Returns shape of array data under given path
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_size_t) :: rank
        character(len=*), optional :: path ! relative path within HDC tree
        integer(kind=c_long), allocatable :: res(:)
        type(hdc_data_t) :: data ! Encapsulated data
        if (.not.present(path)) path = ""
        data = hdc_get_data(this,trim(path)//c_null_char)
        rank = data%rank
        allocate(res(data%rank))
        res(1:data%rank) = data%dshape(1:data%rank)
    end function hdc_get_shape_path

    function hdc_get_shape_pos(this, pos) result(res)
    ! Returns shape of array data under given position/index
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t) :: pos ! Position/index
        integer(kind=c_long) :: res
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,c_null_char)
        res = data%dshape(pos)
    end function hdc_get_shape_pos

    function hdc_get_type(this) result(res)
    ! Retunrs numeric representation of stored data type
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_size_t) :: res
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,c_null_char)
        res = data%dtype
    end function hdc_get_type

    subroutine hdc_set_child(this, path, node)
    ! Sets(replaces) node under the given path
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        type(hdc_t) :: node
        if (.not.present(path)) path = ""
        call c_hdc_set_child(this, trim(path)//c_null_char, node)
    end subroutine hdc_set_child

    subroutine hdc_copy(src, dest, deep_copy)
    ! Pertorms copy of "src" node into "dest" node
        use iso_c_binding
        type(hdc_t) :: src, dest ! HDC object
        logical, optional :: deep_copy ! Recursively copy entire subtree
        logical(kind=c_bool) :: deep_copy_ = .false.
        if (present(deep_copy)) deep_copy_ = deep_copy
        dest = c_hdc_copy(src, deep_copy_)
    end subroutine hdc_copy

    subroutine hdc_set_string_path(this, path, str)
    ! Sets string under the given path
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        character(len=*), intent(in) :: str
        call c_hdc_set_string(this, trim(path)//c_null_char, trim(str)//c_null_char)
    end subroutine hdc_set_string_path

    subroutine hdc_set_string(this, str)
    ! Sets string
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*) :: str
        call c_hdc_set_string(this, c_null_char, trim(str)//c_null_char)
    end subroutine hdc_set_string

    function hdc_get_child(this, path) result(res)
    ! Returns child node under the given path
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        type(hdc_t) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_get_child(this, trim(path)//c_null_char)
    end function hdc_get_child

    subroutine hdc_get_child_sub(this, path, res)
    ! Returns child node under the given path. The node is stored in "res".
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        type(hdc_t) :: res
        if (.not.present(path)) path = ""
        res = hdc_get_child(this, trim(path)//c_null_char)
    end subroutine hdc_get_child_sub

    function hdc_get_slice_path(this, path, ii) result(res)
    ! Returns HDC onbject under the given path and position
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_size_t) :: ii ! position/index
        type(hdc_t) :: res
        res = c_hdc_get_slice_path(this, trim(path)//c_null_char, ii)
    end function hdc_get_slice_path

    subroutine hdc_get_slice_path_sub(this, path, ii, res)
    ! Returns HDC onbject under the given path and position. The node is stored in "res".
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! relative path within HDC tree
        integer(kind=c_size_t) :: ii
        type(hdc_t) :: res
        if (.not.present(path)) path = ""
        res = hdc_get_slice_path(this, trim(path)//c_null_char, ii)
    end subroutine hdc_get_slice_path_sub


    function hdc_get_slice_(this, ii) result(res)
    ! Returns HDC node under the given position (int32)
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t) :: ii
        type(hdc_t) :: res
        res = c_hdc_get_slice(this, int(ii,c_long))
    end function hdc_get_slice_

    function hdc_get_slice_l(this, ii) result(res)
    ! Returns HDC node under the given position (int64)
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=8) :: ii
        type(hdc_t) :: res
        res = c_hdc_get_slice(this, int(ii,c_long))
    end function hdc_get_slice_l

    function hdc_get_data(this, path) result(res)
    ! Returns hdc_data_t struct containing data array and some metadata.
        use iso_c_binding
        type(hdc_t), intent(in) :: this
        character(len=*), intent(in), optional :: path ! relative path within HDC tree
        type(hdc_data_t) :: res
        if (present(path)) then
            res = c_hdc_get_data(this,trim(path)//c_null_char)
        else
            res = c_hdc_get_data(this,c_null_char)
        end if
    end function hdc_get_data

    function C_to_F_string(c_string_pointer) result(f_string)
    ! Converts C string into fortran one. Helper fuction.
        use, intrinsic :: iso_c_binding, only: c_ptr,c_f_pointer,c_char,c_null_char
        type(c_ptr), intent(in) :: c_string_pointer
        character(len=:), allocatable :: f_string
        character(kind=c_char), dimension(:), pointer :: char_array_pointer => null()
        character(len=255) :: aux_string
        integer :: i, length
        length = 0
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

    subroutine hdc_as_string_sub(this, res)
    ! Returns string data. The data are stored to *res*.
        type(hdc_t) :: this ! HDC node
        type(c_ptr) :: char_ptr
        character(len=:),allocatable :: res
        char_ptr = c_hdc_as_voidptr(this)
        res = C_to_F_string(char_ptr)
    end subroutine hdc_as_string_sub

    function hdc_new_array(rank, shape_, type_str) result(res)
    ! Constructs HDC node containint array of provided shape and type
        integer(kind=c_size_t) :: rank
        integer(kind=c_long), dimension(:), target :: shape_
        character(len=*), intent(in) :: type_str
        type(hdc_t) :: res
        res = c_hdc_new_array(rank, c_loc(shape_), trim(type_str)//c_null_char)
    end function hdc_new_array

    function hdc_new_string(str) result(res)
    ! Constructs HDC from the given string.
        character(len=*), intent(in) :: str
        type(hdc_t) :: res
        res = c_hdc_new_string(trim(str)//c_null_char)
    end function hdc_new_string

    function hdc_as_string(this,path) result(res)
    ! Returns string stored under the given path
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(c_ptr) :: char_ptr
        character(len=*), optional :: path ! Path string
        character(len=:), allocatable :: res
        if (.not.present(path)) path = ""
        char_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        res = C_to_F_string(char_ptr)
    end function hdc_as_string

    subroutine hdc_as_string_path_sub(this, path, res)
        type(hdc_t) :: this ! HDC node
        type(c_ptr) :: char_ptr
        character(len=*) :: path ! relative path within HDC tree
        character(len=*) :: res
        char_ptr = c_hdc_as_voidptr_path(this,trim(path)//c_null_char)
        res = C_to_F_string(char_ptr)
    end subroutine hdc_as_string_path_sub

!    subroutine hdc_to_json(this,path,mode)
!        type(hdc_t) :: this ! HDC node
!        character(len=*), optional :: path ! Path string
!        integer(kind=c_int32_t) :: mode
!        if (.not.present(path)) path = ""
!        call c_hdc_to_json(this,trim(path)//c_null_char, mode)
!    end subroutine hdc_to_json

    function hdc_get_ptr_f(tree) result(res)
    ! Returns fortran pointer of HDC object. Helper fucntion. Do not use it unless you know what you are doing.
        use iso_c_binding
        type(hdc_t), target :: tree
        type(c_ptr) :: res
        res = c_loc(tree)
    end function hdc_get_ptr_f

    subroutine hdc_set_data_path(tree, path, data)
    ! Sets hdc_data_t encapsulated data to the given node under the given path
        use iso_c_binding
        type(hdc_t), target :: tree
        type(hdc_data_t) :: data ! Encapsulated data
        character(len=*), intent(in) :: path ! relative path within HDC tree
        call c_hdc_set_external(tree, trim(path)//c_null_char, data)
    end subroutine hdc_set_data_path

    subroutine hdc_set_data_(tree, data)
    ! Sets hdc_data_t encapsulated data under to the given node
        use iso_c_binding
        type(hdc_t), target :: tree
        type(hdc_data_t) :: data ! Encapsulated data
        call c_hdc_set_external(tree, c_null_char, data)
    end subroutine hdc_set_data_

    subroutine hdc_set_external_path(tree, path, data)
        use iso_c_binding
        type(hdc_t), target :: tree
        type(hdc_data_t) :: data ! Encapsulated data
        character(len=*), intent(in) :: path ! relative path within HDC tree
        call c_hdc_set_external(tree, trim(path)//c_null_char, data)
    end subroutine hdc_set_external_path

    subroutine hdc_set_external_(tree, data)
        use iso_c_binding
        type(hdc_t), target :: tree
        type(hdc_data_t) :: data ! Encapsulated data
        call c_hdc_set_external(tree, c_null_char, data)
    end subroutine hdc_set_external_

    subroutine hdc_destroy() bind(c,name="HDC_destroy_c")
    ! Destroy HDC
        use iso_c_binding
    end subroutine hdc_destroy

    subroutine hdc_init(pluginFileName, pluginSettingsString)
    ! HDC Initialization
        use iso_c_binding
        character(kind=c_char,len=*), optional :: pluginFileName
        character(kind=c_char,len=*), optional :: pluginSettingsString
        if (.not.present(pluginFileName)) pluginFileName = ""
        if (.not.present(pluginSettingsString)) pluginSettingsString = ""
        call c_hdc_init(trim(pluginFileName)//c_null_char,trim(pluginSettingsString)//c_null_char)
    end subroutine hdc_init

    function hdc_new_dt(shape_, dtype) result(res)
    ! Constructs HDC object from given shape and dtype string.
        integer(kind=c_long), dimension(:), target, optional :: shape_
        character(len=*), intent(in), optional :: dtype ! type string, e.g. int8/uint16/float/double/boolean/string
        integer(kind=c_size_t) :: rank
        type(hdc_t) :: res
        if (present(shape_)) then
            if (.not.present(dtype)) stop "hdc_new_dt: Please, provide both shape and dtype, or none of them..."
            rank = size(shape_)
            res = c_hdc_new_array(rank, c_loc(shape_), trim(dtype)//c_null_char)
        else
            res = c_hdc_new_empty()
        end if
    end function hdc_new_dt

    function hdc_new_size(size) result(res)
    ! Constructs a new HDC object with given size.
        integer(kind=c_size_t), value :: size ! Size in bytes
        type(hdc_t) :: res
        res = c_hdc_new_size(size)
    end function hdc_new_size

    function hdc_as_int8_1d(this, path) result(res)
    ! Returns 1d int8 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_1d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int8_1d

    subroutine hdc_as_int8_1d_sub(this, res)
    ! Returns 1d int8 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_1d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_1d_sub

    subroutine hdc_set_int8_1d(this, data, flags_in)
    ! Sets 1d int8 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int8_1d

    subroutine hdc_set_int8_1d_path(this, path, data, flags_in)
    ! Sets 1d int8 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int8_1d_path

    subroutine hdc_as_int8_1d_path_sub(this,path,res)
    ! Sets provided 1d int8 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int8_t), pointer, intent(inout) :: res(:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_1d_path_sub

    function hdc_as_int8_2d(this, path) result(res)
    ! Returns 2d int8 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_2d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int8_2d

    subroutine hdc_as_int8_2d_sub(this, res)
    ! Returns 2d int8 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_2d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_2d_sub

    subroutine hdc_set_int8_2d(this, data, flags_in)
    ! Sets 2d int8 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int8_2d

    subroutine hdc_set_int8_2d_path(this, path, data, flags_in)
    ! Sets 2d int8 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int8_2d_path

    subroutine hdc_as_int8_2d_path_sub(this,path,res)
    ! Sets provided 2d int8 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int8_t), pointer, intent(inout) :: res(:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_2d_path_sub

    function hdc_as_int8_3d(this, path) result(res)
    ! Returns 3d int8 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_3d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int8_3d

    subroutine hdc_as_int8_3d_sub(this, res)
    ! Returns 3d int8 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_3d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_3d_sub

    subroutine hdc_set_int8_3d(this, data, flags_in)
    ! Sets 3d int8 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int8_3d

    subroutine hdc_set_int8_3d_path(this, path, data, flags_in)
    ! Sets 3d int8 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int8_3d_path

    subroutine hdc_as_int8_3d_path_sub(this,path,res)
    ! Sets provided 3d int8 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int8_t), pointer, intent(inout) :: res(:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_3d_path_sub

    function hdc_as_int8_4d(this, path) result(res)
    ! Returns 4d int8 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_4d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int8_4d

    subroutine hdc_as_int8_4d_sub(this, res)
    ! Returns 4d int8 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_4d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_4d_sub

    subroutine hdc_set_int8_4d(this, data, flags_in)
    ! Sets 4d int8 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int8_4d

    subroutine hdc_set_int8_4d_path(this, path, data, flags_in)
    ! Sets 4d int8 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int8_4d_path

    subroutine hdc_as_int8_4d_path_sub(this,path,res)
    ! Sets provided 4d int8 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int8_t), pointer, intent(inout) :: res(:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_4d_path_sub

    function hdc_as_int8_5d(this, path) result(res)
    ! Returns 5d int8 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_5d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int8_5d

    subroutine hdc_as_int8_5d_sub(this, res)
    ! Returns 5d int8 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_5d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_5d_sub

    subroutine hdc_set_int8_5d(this, data, flags_in)
    ! Sets 5d int8 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int8_5d

    subroutine hdc_set_int8_5d_path(this, path, data, flags_in)
    ! Sets 5d int8 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int8_5d_path

    subroutine hdc_as_int8_5d_path_sub(this,path,res)
    ! Sets provided 5d int8 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int8_t), pointer, intent(inout) :: res(:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_5d_path_sub

    function hdc_as_int8_6d(this, path) result(res)
    ! Returns 6d int8 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_6d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int8_6d

    subroutine hdc_as_int8_6d_sub(this, res)
    ! Returns 6d int8 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_6d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_6d_sub

    subroutine hdc_set_int8_6d(this, data, flags_in)
    ! Sets 6d int8 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int8_6d

    subroutine hdc_set_int8_6d_path(this, path, data, flags_in)
    ! Sets 6d int8 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int8_6d_path

    subroutine hdc_as_int8_6d_path_sub(this,path,res)
    ! Sets provided 6d int8 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int8_t), pointer, intent(inout) :: res(:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_6d_path_sub

    function hdc_as_int8_7d(this, path) result(res)
    ! Returns 7d int8 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_7d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int8_7d

    subroutine hdc_as_int8_7d_sub(this, res)
    ! Returns 7d int8 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int8_t), pointer :: res(:,:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int8_7d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_7d_sub

    subroutine hdc_set_int8_7d(this, data, flags_in)
    ! Sets 7d int8 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int8_7d

    subroutine hdc_set_int8_7d_path(this, path, data, flags_in)
    ! Sets 7d int8 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data(:,:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT8
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int8_7d_path

    subroutine hdc_as_int8_7d_path_sub(this,path,res)
    ! Sets provided 7d int8 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int8_t), pointer, intent(inout) :: res(:,:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int8_7d_path_sub

    subroutine hdc_set_int8_scalar(this, data)
    ! Sets int8 data into the given node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t), intent(in), target :: data
        call c_hdc_set_scalar(this, c_null_char, c_loc(data), HDC_INT8)
    end subroutine hdc_set_int8_scalar

    subroutine hdc_set_int8_scalar_path(this, path, data)
    ! Sets int8 data under the given path of the provided node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_int8_t), intent(in), target :: data
        call c_hdc_set_scalar(this, trim(path)//c_null_char, c_loc(data), HDC_INT8)
    end subroutine hdc_set_int8_scalar_path

    subroutine hdc_as_int8_sub(this, res)
    ! Sets int8 data into provided variable.
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int8_t) :: res
        res = c_hdc_as_int8_scalar(this, c_null_char)
    end subroutine hdc_as_int8_sub


    function hdc_as_int8(this, path) result(res)
    ! Returns int8 scalar data.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        integer(kind=c_int8_t) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_as_int8_scalar(this, trim(path)//c_null_char)
    end function hdc_as_int8

    subroutine hdc_as_int8_path_sub(this, path, res)
    ! Sets int8 scalar data under the given path and provided node to provided variable.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int8_t) :: res
        res = c_hdc_as_int8_scalar(this, trim(path)//c_null_char)
    end subroutine hdc_as_int8_path_sub


    function hdc_as_int16_1d(this, path) result(res)
    ! Returns 1d int16 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_1d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int16_1d

    subroutine hdc_as_int16_1d_sub(this, res)
    ! Returns 1d int16 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_1d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_1d_sub

    subroutine hdc_set_int16_1d(this, data, flags_in)
    ! Sets 1d int16 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int16_1d

    subroutine hdc_set_int16_1d_path(this, path, data, flags_in)
    ! Sets 1d int16 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int16_1d_path

    subroutine hdc_as_int16_1d_path_sub(this,path,res)
    ! Sets provided 1d int16 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int16_t), pointer, intent(inout) :: res(:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_1d_path_sub

    function hdc_as_int16_2d(this, path) result(res)
    ! Returns 2d int16 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_2d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int16_2d

    subroutine hdc_as_int16_2d_sub(this, res)
    ! Returns 2d int16 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_2d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_2d_sub

    subroutine hdc_set_int16_2d(this, data, flags_in)
    ! Sets 2d int16 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int16_2d

    subroutine hdc_set_int16_2d_path(this, path, data, flags_in)
    ! Sets 2d int16 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int16_2d_path

    subroutine hdc_as_int16_2d_path_sub(this,path,res)
    ! Sets provided 2d int16 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int16_t), pointer, intent(inout) :: res(:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_2d_path_sub

    function hdc_as_int16_3d(this, path) result(res)
    ! Returns 3d int16 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_3d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int16_3d

    subroutine hdc_as_int16_3d_sub(this, res)
    ! Returns 3d int16 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_3d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_3d_sub

    subroutine hdc_set_int16_3d(this, data, flags_in)
    ! Sets 3d int16 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int16_3d

    subroutine hdc_set_int16_3d_path(this, path, data, flags_in)
    ! Sets 3d int16 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int16_3d_path

    subroutine hdc_as_int16_3d_path_sub(this,path,res)
    ! Sets provided 3d int16 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int16_t), pointer, intent(inout) :: res(:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_3d_path_sub

    function hdc_as_int16_4d(this, path) result(res)
    ! Returns 4d int16 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_4d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int16_4d

    subroutine hdc_as_int16_4d_sub(this, res)
    ! Returns 4d int16 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_4d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_4d_sub

    subroutine hdc_set_int16_4d(this, data, flags_in)
    ! Sets 4d int16 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int16_4d

    subroutine hdc_set_int16_4d_path(this, path, data, flags_in)
    ! Sets 4d int16 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int16_4d_path

    subroutine hdc_as_int16_4d_path_sub(this,path,res)
    ! Sets provided 4d int16 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int16_t), pointer, intent(inout) :: res(:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_4d_path_sub

    function hdc_as_int16_5d(this, path) result(res)
    ! Returns 5d int16 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_5d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int16_5d

    subroutine hdc_as_int16_5d_sub(this, res)
    ! Returns 5d int16 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_5d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_5d_sub

    subroutine hdc_set_int16_5d(this, data, flags_in)
    ! Sets 5d int16 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int16_5d

    subroutine hdc_set_int16_5d_path(this, path, data, flags_in)
    ! Sets 5d int16 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int16_5d_path

    subroutine hdc_as_int16_5d_path_sub(this,path,res)
    ! Sets provided 5d int16 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int16_t), pointer, intent(inout) :: res(:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_5d_path_sub

    function hdc_as_int16_6d(this, path) result(res)
    ! Returns 6d int16 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_6d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int16_6d

    subroutine hdc_as_int16_6d_sub(this, res)
    ! Returns 6d int16 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_6d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_6d_sub

    subroutine hdc_set_int16_6d(this, data, flags_in)
    ! Sets 6d int16 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int16_6d

    subroutine hdc_set_int16_6d_path(this, path, data, flags_in)
    ! Sets 6d int16 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int16_6d_path

    subroutine hdc_as_int16_6d_path_sub(this,path,res)
    ! Sets provided 6d int16 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int16_t), pointer, intent(inout) :: res(:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_6d_path_sub

    function hdc_as_int16_7d(this, path) result(res)
    ! Returns 7d int16 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_7d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int16_7d

    subroutine hdc_as_int16_7d_sub(this, res)
    ! Returns 7d int16 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int16_t), pointer :: res(:,:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int16_7d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_7d_sub

    subroutine hdc_set_int16_7d(this, data, flags_in)
    ! Sets 7d int16 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int16_7d

    subroutine hdc_set_int16_7d_path(this, path, data, flags_in)
    ! Sets 7d int16 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data(:,:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT16
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int16_7d_path

    subroutine hdc_as_int16_7d_path_sub(this,path,res)
    ! Sets provided 7d int16 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int16_t), pointer, intent(inout) :: res(:,:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int16_7d_path_sub

    subroutine hdc_set_int16_scalar(this, data)
    ! Sets int16 data into the given node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t), intent(in), target :: data
        call c_hdc_set_scalar(this, c_null_char, c_loc(data), HDC_INT16)
    end subroutine hdc_set_int16_scalar

    subroutine hdc_set_int16_scalar_path(this, path, data)
    ! Sets int16 data under the given path of the provided node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_int16_t), intent(in), target :: data
        call c_hdc_set_scalar(this, trim(path)//c_null_char, c_loc(data), HDC_INT16)
    end subroutine hdc_set_int16_scalar_path

    subroutine hdc_as_int16_sub(this, res)
    ! Sets int16 data into provided variable.
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int16_t) :: res
        res = c_hdc_as_int16_scalar(this, c_null_char)
    end subroutine hdc_as_int16_sub


    function hdc_as_int16(this, path) result(res)
    ! Returns int16 scalar data.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        integer(kind=c_int16_t) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_as_int16_scalar(this, trim(path)//c_null_char)
    end function hdc_as_int16

    subroutine hdc_as_int16_path_sub(this, path, res)
    ! Sets int16 scalar data under the given path and provided node to provided variable.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int16_t) :: res
        res = c_hdc_as_int16_scalar(this, trim(path)//c_null_char)
    end subroutine hdc_as_int16_path_sub


    function hdc_as_int32_1d(this, path) result(res)
    ! Returns 1d int32 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_1d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int32_1d

    subroutine hdc_as_int32_1d_sub(this, res)
    ! Returns 1d int32 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_1d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_1d_sub

    subroutine hdc_set_int32_1d(this, data, flags_in)
    ! Sets 1d int32 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int32_1d

    subroutine hdc_set_int32_1d_path(this, path, data, flags_in)
    ! Sets 1d int32 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int32_1d_path

    subroutine hdc_as_int32_1d_path_sub(this,path,res)
    ! Sets provided 1d int32 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int32_t), pointer, intent(inout) :: res(:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_1d_path_sub

    function hdc_as_int32_2d(this, path) result(res)
    ! Returns 2d int32 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_2d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int32_2d

    subroutine hdc_as_int32_2d_sub(this, res)
    ! Returns 2d int32 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_2d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_2d_sub

    subroutine hdc_set_int32_2d(this, data, flags_in)
    ! Sets 2d int32 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int32_2d

    subroutine hdc_set_int32_2d_path(this, path, data, flags_in)
    ! Sets 2d int32 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int32_2d_path

    subroutine hdc_as_int32_2d_path_sub(this,path,res)
    ! Sets provided 2d int32 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int32_t), pointer, intent(inout) :: res(:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_2d_path_sub

    function hdc_as_int32_3d(this, path) result(res)
    ! Returns 3d int32 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_3d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int32_3d

    subroutine hdc_as_int32_3d_sub(this, res)
    ! Returns 3d int32 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_3d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_3d_sub

    subroutine hdc_set_int32_3d(this, data, flags_in)
    ! Sets 3d int32 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int32_3d

    subroutine hdc_set_int32_3d_path(this, path, data, flags_in)
    ! Sets 3d int32 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int32_3d_path

    subroutine hdc_as_int32_3d_path_sub(this,path,res)
    ! Sets provided 3d int32 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int32_t), pointer, intent(inout) :: res(:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_3d_path_sub

    function hdc_as_int32_4d(this, path) result(res)
    ! Returns 4d int32 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_4d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int32_4d

    subroutine hdc_as_int32_4d_sub(this, res)
    ! Returns 4d int32 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_4d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_4d_sub

    subroutine hdc_set_int32_4d(this, data, flags_in)
    ! Sets 4d int32 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int32_4d

    subroutine hdc_set_int32_4d_path(this, path, data, flags_in)
    ! Sets 4d int32 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int32_4d_path

    subroutine hdc_as_int32_4d_path_sub(this,path,res)
    ! Sets provided 4d int32 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int32_t), pointer, intent(inout) :: res(:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_4d_path_sub

    function hdc_as_int32_5d(this, path) result(res)
    ! Returns 5d int32 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_5d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int32_5d

    subroutine hdc_as_int32_5d_sub(this, res)
    ! Returns 5d int32 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_5d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_5d_sub

    subroutine hdc_set_int32_5d(this, data, flags_in)
    ! Sets 5d int32 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int32_5d

    subroutine hdc_set_int32_5d_path(this, path, data, flags_in)
    ! Sets 5d int32 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int32_5d_path

    subroutine hdc_as_int32_5d_path_sub(this,path,res)
    ! Sets provided 5d int32 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int32_t), pointer, intent(inout) :: res(:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_5d_path_sub

    function hdc_as_int32_6d(this, path) result(res)
    ! Returns 6d int32 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_6d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int32_6d

    subroutine hdc_as_int32_6d_sub(this, res)
    ! Returns 6d int32 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_6d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_6d_sub

    subroutine hdc_set_int32_6d(this, data, flags_in)
    ! Sets 6d int32 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int32_6d

    subroutine hdc_set_int32_6d_path(this, path, data, flags_in)
    ! Sets 6d int32 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int32_6d_path

    subroutine hdc_as_int32_6d_path_sub(this,path,res)
    ! Sets provided 6d int32 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int32_t), pointer, intent(inout) :: res(:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_6d_path_sub

    function hdc_as_int32_7d(this, path) result(res)
    ! Returns 7d int32 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_7d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int32_7d

    subroutine hdc_as_int32_7d_sub(this, res)
    ! Returns 7d int32 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int32_t), pointer :: res(:,:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_7d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_7d_sub

    subroutine hdc_set_int32_7d(this, data, flags_in)
    ! Sets 7d int32 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int32_7d

    subroutine hdc_set_int32_7d_path(this, path, data, flags_in)
    ! Sets 7d int32 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data(:,:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int32_7d_path

    subroutine hdc_as_int32_7d_path_sub(this,path,res)
    ! Sets provided 7d int32 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int32_t), pointer, intent(inout) :: res(:,:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_7d_path_sub

    subroutine hdc_set_int32_scalar(this, data)
    ! Sets int32 data into the given node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t), intent(in), target :: data
        call c_hdc_set_scalar(this, c_null_char, c_loc(data), HDC_INT32)
    end subroutine hdc_set_int32_scalar

    subroutine hdc_set_int32_scalar_path(this, path, data)
    ! Sets int32 data under the given path of the provided node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_int32_t), intent(in), target :: data
        call c_hdc_set_scalar(this, trim(path)//c_null_char, c_loc(data), HDC_INT32)
    end subroutine hdc_set_int32_scalar_path

    subroutine hdc_as_int32_sub(this, res)
    ! Sets int32 data into provided variable.
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int32_t) :: res
        res = c_hdc_as_int32_scalar(this, c_null_char)
    end subroutine hdc_as_int32_sub


    function hdc_as_int32(this, path) result(res)
    ! Returns int32 scalar data.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        integer(kind=c_int32_t) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_as_int32_scalar(this, trim(path)//c_null_char)
    end function hdc_as_int32

    subroutine hdc_as_int32_path_sub(this, path, res)
    ! Sets int32 scalar data under the given path and provided node to provided variable.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int32_t) :: res
        res = c_hdc_as_int32_scalar(this, trim(path)//c_null_char)
    end subroutine hdc_as_int32_path_sub


    function hdc_as_int64_1d(this, path) result(res)
    ! Returns 1d int64 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_1d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int64_1d

    subroutine hdc_as_int64_1d_sub(this, res)
    ! Returns 1d int64 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_1d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_1d_sub

    subroutine hdc_set_int64_1d(this, data, flags_in)
    ! Sets 1d int64 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int64_1d

    subroutine hdc_set_int64_1d_path(this, path, data, flags_in)
    ! Sets 1d int64 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int64_1d_path

    subroutine hdc_as_int64_1d_path_sub(this,path,res)
    ! Sets provided 1d int64 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int64_t), pointer, intent(inout) :: res(:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_1d_path_sub

    function hdc_as_int64_2d(this, path) result(res)
    ! Returns 2d int64 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_2d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int64_2d

    subroutine hdc_as_int64_2d_sub(this, res)
    ! Returns 2d int64 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_2d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_2d_sub

    subroutine hdc_set_int64_2d(this, data, flags_in)
    ! Sets 2d int64 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int64_2d

    subroutine hdc_set_int64_2d_path(this, path, data, flags_in)
    ! Sets 2d int64 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int64_2d_path

    subroutine hdc_as_int64_2d_path_sub(this,path,res)
    ! Sets provided 2d int64 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int64_t), pointer, intent(inout) :: res(:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_2d_path_sub

    function hdc_as_int64_3d(this, path) result(res)
    ! Returns 3d int64 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_3d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int64_3d

    subroutine hdc_as_int64_3d_sub(this, res)
    ! Returns 3d int64 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_3d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_3d_sub

    subroutine hdc_set_int64_3d(this, data, flags_in)
    ! Sets 3d int64 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int64_3d

    subroutine hdc_set_int64_3d_path(this, path, data, flags_in)
    ! Sets 3d int64 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int64_3d_path

    subroutine hdc_as_int64_3d_path_sub(this,path,res)
    ! Sets provided 3d int64 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int64_t), pointer, intent(inout) :: res(:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_3d_path_sub

    function hdc_as_int64_4d(this, path) result(res)
    ! Returns 4d int64 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_4d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int64_4d

    subroutine hdc_as_int64_4d_sub(this, res)
    ! Returns 4d int64 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_4d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_4d_sub

    subroutine hdc_set_int64_4d(this, data, flags_in)
    ! Sets 4d int64 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int64_4d

    subroutine hdc_set_int64_4d_path(this, path, data, flags_in)
    ! Sets 4d int64 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int64_4d_path

    subroutine hdc_as_int64_4d_path_sub(this,path,res)
    ! Sets provided 4d int64 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int64_t), pointer, intent(inout) :: res(:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_4d_path_sub

    function hdc_as_int64_5d(this, path) result(res)
    ! Returns 5d int64 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_5d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int64_5d

    subroutine hdc_as_int64_5d_sub(this, res)
    ! Returns 5d int64 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_5d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_5d_sub

    subroutine hdc_set_int64_5d(this, data, flags_in)
    ! Sets 5d int64 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int64_5d

    subroutine hdc_set_int64_5d_path(this, path, data, flags_in)
    ! Sets 5d int64 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int64_5d_path

    subroutine hdc_as_int64_5d_path_sub(this,path,res)
    ! Sets provided 5d int64 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int64_t), pointer, intent(inout) :: res(:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_5d_path_sub

    function hdc_as_int64_6d(this, path) result(res)
    ! Returns 6d int64 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_6d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int64_6d

    subroutine hdc_as_int64_6d_sub(this, res)
    ! Returns 6d int64 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_6d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_6d_sub

    subroutine hdc_set_int64_6d(this, data, flags_in)
    ! Sets 6d int64 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int64_6d

    subroutine hdc_set_int64_6d_path(this, path, data, flags_in)
    ! Sets 6d int64 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int64_6d_path

    subroutine hdc_as_int64_6d_path_sub(this,path,res)
    ! Sets provided 6d int64 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int64_t), pointer, intent(inout) :: res(:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_6d_path_sub

    function hdc_as_int64_7d(this, path) result(res)
    ! Returns 7d int64 array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_7d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int64_7d

    subroutine hdc_as_int64_7d_sub(this, res)
    ! Returns 7d int64 array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        integer(kind=c_int64_t), pointer :: res(:,:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int64_7d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_7d_sub

    subroutine hdc_set_int64_7d(this, data, flags_in)
    ! Sets 7d int64 array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_int64_7d

    subroutine hdc_set_int64_7d_path(this, path, data, flags_in)
    ! Sets 7d int64 array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data(:,:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_INT64
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_int64_7d_path

    subroutine hdc_as_int64_7d_path_sub(this,path,res)
    ! Sets provided 7d int64 array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int64_t), pointer, intent(inout) :: res(:,:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int64_7d_path_sub

    subroutine hdc_set_int64_scalar(this, data)
    ! Sets int64 data into the given node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t), intent(in), target :: data
        call c_hdc_set_scalar(this, c_null_char, c_loc(data), HDC_INT64)
    end subroutine hdc_set_int64_scalar

    subroutine hdc_set_int64_scalar_path(this, path, data)
    ! Sets int64 data under the given path of the provided node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_int64_t), intent(in), target :: data
        call c_hdc_set_scalar(this, trim(path)//c_null_char, c_loc(data), HDC_INT64)
    end subroutine hdc_set_int64_scalar_path

    subroutine hdc_as_int64_sub(this, res)
    ! Sets int64 data into provided variable.
        type(hdc_t) :: this ! HDC node
        integer(kind=c_int64_t) :: res
        res = c_hdc_as_int64_scalar(this, c_null_char)
    end subroutine hdc_as_int64_sub


    function hdc_as_int64(this, path) result(res)
    ! Returns int64 scalar data.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        integer(kind=c_int64_t) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_as_int64_scalar(this, trim(path)//c_null_char)
    end function hdc_as_int64

    subroutine hdc_as_int64_path_sub(this, path, res)
    ! Sets int64 scalar data under the given path and provided node to provided variable.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        integer(kind=c_int64_t) :: res
        res = c_hdc_as_int64_scalar(this, trim(path)//c_null_char)
    end subroutine hdc_as_int64_path_sub


    function hdc_as_float_1d(this, path) result(res)
    ! Returns 1d float array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_1d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_float_1d

    subroutine hdc_as_float_1d_sub(this, res)
    ! Returns 1d float array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_1d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_1d_sub

    subroutine hdc_set_float_1d(this, data, flags_in)
    ! Sets 1d float array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_float_1d

    subroutine hdc_set_float_1d_path(this, path, data, flags_in)
    ! Sets 1d float array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_float_1d_path

    subroutine hdc_as_float_1d_path_sub(this,path,res)
    ! Sets provided 1d float array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=sp), pointer, intent(inout) :: res(:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_1d_path_sub

    function hdc_as_float_2d(this, path) result(res)
    ! Returns 2d float array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_2d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_float_2d

    subroutine hdc_as_float_2d_sub(this, res)
    ! Returns 2d float array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_2d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_2d_sub

    subroutine hdc_set_float_2d(this, data, flags_in)
    ! Sets 2d float array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_float_2d

    subroutine hdc_set_float_2d_path(this, path, data, flags_in)
    ! Sets 2d float array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_float_2d_path

    subroutine hdc_as_float_2d_path_sub(this,path,res)
    ! Sets provided 2d float array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=sp), pointer, intent(inout) :: res(:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_2d_path_sub

    function hdc_as_float_3d(this, path) result(res)
    ! Returns 3d float array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_3d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_float_3d

    subroutine hdc_as_float_3d_sub(this, res)
    ! Returns 3d float array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_3d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_3d_sub

    subroutine hdc_set_float_3d(this, data, flags_in)
    ! Sets 3d float array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_float_3d

    subroutine hdc_set_float_3d_path(this, path, data, flags_in)
    ! Sets 3d float array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_float_3d_path

    subroutine hdc_as_float_3d_path_sub(this,path,res)
    ! Sets provided 3d float array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=sp), pointer, intent(inout) :: res(:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_3d_path_sub

    function hdc_as_float_4d(this, path) result(res)
    ! Returns 4d float array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_4d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_float_4d

    subroutine hdc_as_float_4d_sub(this, res)
    ! Returns 4d float array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_4d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_4d_sub

    subroutine hdc_set_float_4d(this, data, flags_in)
    ! Sets 4d float array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_float_4d

    subroutine hdc_set_float_4d_path(this, path, data, flags_in)
    ! Sets 4d float array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_float_4d_path

    subroutine hdc_as_float_4d_path_sub(this,path,res)
    ! Sets provided 4d float array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=sp), pointer, intent(inout) :: res(:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_4d_path_sub

    function hdc_as_float_5d(this, path) result(res)
    ! Returns 5d float array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_5d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_float_5d

    subroutine hdc_as_float_5d_sub(this, res)
    ! Returns 5d float array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_5d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_5d_sub

    subroutine hdc_set_float_5d(this, data, flags_in)
    ! Sets 5d float array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_float_5d

    subroutine hdc_set_float_5d_path(this, path, data, flags_in)
    ! Sets 5d float array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_float_5d_path

    subroutine hdc_as_float_5d_path_sub(this,path,res)
    ! Sets provided 5d float array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=sp), pointer, intent(inout) :: res(:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_5d_path_sub

    function hdc_as_float_6d(this, path) result(res)
    ! Returns 6d float array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_6d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_float_6d

    subroutine hdc_as_float_6d_sub(this, res)
    ! Returns 6d float array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_6d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_6d_sub

    subroutine hdc_set_float_6d(this, data, flags_in)
    ! Sets 6d float array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_float_6d

    subroutine hdc_set_float_6d_path(this, path, data, flags_in)
    ! Sets 6d float array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_float_6d_path

    subroutine hdc_as_float_6d_path_sub(this,path,res)
    ! Sets provided 6d float array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=sp), pointer, intent(inout) :: res(:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_6d_path_sub

    function hdc_as_float_7d(this, path) result(res)
    ! Returns 7d float array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_7d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_float_7d

    subroutine hdc_as_float_7d_sub(this, res)
    ! Returns 7d float array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=sp), pointer :: res(:,:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_float_7d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_7d_sub

    subroutine hdc_set_float_7d(this, data, flags_in)
    ! Sets 7d float array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_float_7d

    subroutine hdc_set_float_7d_path(this, path, data, flags_in)
    ! Sets 7d float array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data(:,:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_FLOAT
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_float_7d_path

    subroutine hdc_as_float_7d_path_sub(this,path,res)
    ! Sets provided 7d float array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=sp), pointer, intent(inout) :: res(:,:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_float_7d_path_sub

    subroutine hdc_set_float_scalar(this, data)
    ! Sets float data into the given node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=sp), intent(in), target :: data
        call c_hdc_set_scalar(this, c_null_char, c_loc(data), HDC_FLOAT)
    end subroutine hdc_set_float_scalar

    subroutine hdc_set_float_scalar_path(this, path, data)
    ! Sets float data under the given path of the provided node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), intent(in) :: path ! relative path within HDC tree
        real(kind=sp), intent(in), target :: data
        call c_hdc_set_scalar(this, trim(path)//c_null_char, c_loc(data), HDC_FLOAT)
    end subroutine hdc_set_float_scalar_path

    subroutine hdc_as_float_sub(this, res)
    ! Sets float data into provided variable.
        type(hdc_t) :: this ! HDC node
        real(kind=sp) :: res
        res = c_hdc_as_float_scalar(this, c_null_char)
    end subroutine hdc_as_float_sub


    function hdc_as_float(this, path) result(res)
    ! Returns float scalar data.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        real(kind=sp) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_as_float_scalar(this, trim(path)//c_null_char)
    end function hdc_as_float

    subroutine hdc_as_float_path_sub(this, path, res)
    ! Sets float scalar data under the given path and provided node to provided variable.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=sp) :: res
        res = c_hdc_as_float_scalar(this, trim(path)//c_null_char)
    end subroutine hdc_as_float_path_sub


    function hdc_as_double_1d(this, path) result(res)
    ! Returns 1d double array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_1d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_double_1d

    subroutine hdc_as_double_1d_sub(this, res)
    ! Returns 1d double array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_1d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_1d_sub

    subroutine hdc_set_double_1d(this, data, flags_in)
    ! Sets 1d double array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_double_1d

    subroutine hdc_set_double_1d_path(this, path, data, flags_in)
    ! Sets 1d double array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(2:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_double_1d_path

    subroutine hdc_as_double_1d_path_sub(this,path,res)
    ! Sets provided 1d double array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=dp), pointer, intent(inout) :: res(:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_1d_path_sub

    function hdc_as_double_2d(this, path) result(res)
    ! Returns 2d double array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_2d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_double_2d

    subroutine hdc_as_double_2d_sub(this, res)
    ! Returns 2d double array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_2d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_2d_sub

    subroutine hdc_set_double_2d(this, data, flags_in)
    ! Sets 2d double array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_double_2d

    subroutine hdc_set_double_2d_path(this, path, data, flags_in)
    ! Sets 2d double array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 2
        out%dshape(1:2) = shape(data)
        out%dshape(3:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_double_2d_path

    subroutine hdc_as_double_2d_path_sub(this,path,res)
    ! Sets provided 2d double array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=dp), pointer, intent(inout) :: res(:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_2d_path_sub

    function hdc_as_double_3d(this, path) result(res)
    ! Returns 3d double array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_3d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_double_3d

    subroutine hdc_as_double_3d_sub(this, res)
    ! Returns 3d double array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_3d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_3d_sub

    subroutine hdc_set_double_3d(this, data, flags_in)
    ! Sets 3d double array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_double_3d

    subroutine hdc_set_double_3d_path(this, path, data, flags_in)
    ! Sets 3d double array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 3
        out%dshape(1:3) = shape(data)
        out%dshape(4:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_double_3d_path

    subroutine hdc_as_double_3d_path_sub(this,path,res)
    ! Sets provided 3d double array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=dp), pointer, intent(inout) :: res(:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_3d_path_sub

    function hdc_as_double_4d(this, path) result(res)
    ! Returns 4d double array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_4d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_double_4d

    subroutine hdc_as_double_4d_sub(this, res)
    ! Returns 4d double array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_4d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_4d_sub

    subroutine hdc_set_double_4d(this, data, flags_in)
    ! Sets 4d double array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_double_4d

    subroutine hdc_set_double_4d_path(this, path, data, flags_in)
    ! Sets 4d double array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 4
        out%dshape(1:4) = shape(data)
        out%dshape(5:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_double_4d_path

    subroutine hdc_as_double_4d_path_sub(this,path,res)
    ! Sets provided 4d double array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=dp), pointer, intent(inout) :: res(:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_4d_path_sub

    function hdc_as_double_5d(this, path) result(res)
    ! Returns 5d double array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_5d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_double_5d

    subroutine hdc_as_double_5d_sub(this, res)
    ! Returns 5d double array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_5d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_5d_sub

    subroutine hdc_set_double_5d(this, data, flags_in)
    ! Sets 5d double array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_double_5d

    subroutine hdc_set_double_5d_path(this, path, data, flags_in)
    ! Sets 5d double array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 5
        out%dshape(1:5) = shape(data)
        out%dshape(6:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_double_5d_path

    subroutine hdc_as_double_5d_path_sub(this,path,res)
    ! Sets provided 5d double array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=dp), pointer, intent(inout) :: res(:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_5d_path_sub

    function hdc_as_double_6d(this, path) result(res)
    ! Returns 6d double array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_6d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_double_6d

    subroutine hdc_as_double_6d_sub(this, res)
    ! Returns 6d double array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_6d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_6d_sub

    subroutine hdc_set_double_6d(this, data, flags_in)
    ! Sets 6d double array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_double_6d

    subroutine hdc_set_double_6d_path(this, path, data, flags_in)
    ! Sets 6d double array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 6
        out%dshape(1:6) = shape(data)
        out%dshape(7:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_double_6d_path

    subroutine hdc_as_double_6d_path_sub(this,path,res)
    ! Sets provided 6d double array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=dp), pointer, intent(inout) :: res(:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_6d_path_sub

    function hdc_as_double_7d(this, path) result(res)
    ! Returns 7d double array stored under the given path.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:,:,:,:,:)
        if (.not.present(path)) path = ""
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_7d"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_double_7d

    subroutine hdc_as_double_7d_sub(this, res)
    ! Returns 7d double array.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        type(hdc_data_t) :: data ! Encapsulated data
        real(kind=dp), pointer :: res(:,:,:,:,:,:,:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_double_7d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_7d_sub

    subroutine hdc_set_double_7d(this, data, flags_in)
    ! Sets 7d double array to the given HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:,:,:,:,:)
        integer(kind=c_size_t), intent(in), optional :: flags_in ! Optional flags
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out)
    end subroutine hdc_set_double_7d

    subroutine hdc_set_double_7d_path(this, path, data, flags_in)
    ! Sets 7d double array to the given path within the provided HDC node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data(:,:,:,:,:,:,:)
        character(len=*), intent(in) :: path ! relative path within HDC tree
        integer(kind=c_size_t), intent(in), optional :: flags_in
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_in)) flags = flags_in
        out%dtype = HDC_DOUBLE
        out%flags = flags
        out%rank = 7
        out%dshape(1:7) = shape(data)
        out%dshape(8:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out)
    end subroutine hdc_set_double_7d_path

    subroutine hdc_as_double_7d_path_sub(this,path,res)
    ! Sets provided 7d double array to the the data given by path within the provided HDC node.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=dp), pointer, intent(inout) :: res(:,:,:,:,:,:,:)
        type(hdc_data_t) :: data ! Encapsulated data
        data = hdc_get_data(this,path)
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_double_7d_path_sub

    subroutine hdc_set_double_scalar(this, data)
    ! Sets double data into the given node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        real(kind=dp), intent(in), target :: data
        call c_hdc_set_scalar(this, c_null_char, c_loc(data), HDC_DOUBLE)
    end subroutine hdc_set_double_scalar

    subroutine hdc_set_double_scalar_path(this, path, data)
    ! Sets double data under the given path of the provided node.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), intent(in) :: path ! relative path within HDC tree
        real(kind=dp), intent(in), target :: data
        call c_hdc_set_scalar(this, trim(path)//c_null_char, c_loc(data), HDC_DOUBLE)
    end subroutine hdc_set_double_scalar_path

    subroutine hdc_as_double_sub(this, res)
    ! Sets double data into provided variable.
        type(hdc_t) :: this ! HDC node
        real(kind=dp) :: res
        res = c_hdc_as_double_scalar(this, c_null_char)
    end subroutine hdc_as_double_sub


    function hdc_as_double(this, path) result(res)
    ! Returns double scalar data.
        use iso_c_binding
        type(hdc_t) :: this ! HDC node
        character(len=*), optional :: path ! Path string
        real(kind=dp) :: res
        if (.not.present(path)) path = ""
        res = c_hdc_as_double_scalar(this, trim(path)//c_null_char)
    end function hdc_as_double

    subroutine hdc_as_double_path_sub(this, path, res)
    ! Sets double scalar data under the given path and provided node to provided variable.
        type(hdc_t) :: this ! HDC node
        character(len=*) :: path ! relative path within HDC tree
        real(kind=dp) :: res
        res = c_hdc_as_double_scalar(this, trim(path)//c_null_char)
    end subroutine hdc_as_double_path_sub



end module hdc_fortran
! http://fortranwiki.org/fortran/show/Fortran+and+Cpp+objs
! https://gcc.gnu.org/onlinedocs/gfortran/Derived-Types-and-struct.html