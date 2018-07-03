

     interface hdc_as_int32_1d
        module procedure hdc_as_int32_1d_
        module procedure hdc_as_int32_1d_path
     end interface hdc_as_int32_1d

    function hdc_as_int32_1d_(this) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        type(hdc_data_t) :: data
        integer(c_int32_t), pointer :: res(:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_1d_"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int32_1d_

    subroutine hdc_as_int32_1d_sub(this, res)
        use iso_c_binding
        type(hdc_t) :: this
        type(hdc_data_t) :: data
        integer(c_int32_t), pointer :: res(:)
        data = c_hdc_get_data(this,c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_1d_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_1d_sub

    function hdc_as_int32_1d_path(this, path) result(res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        type(hdc_data_t) :: data
        integer(c_int32_t), pointer :: res(:)
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_1d_path"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end function hdc_as_int32_1d_path

    subroutine hdc_as_int32_1d_path_sub(this,path,res)
        use iso_c_binding
        type(hdc_t) :: this
        character(len=*), intent(in) :: path
        type(hdc_data_t) :: data
        integer(c_int32_t), pointer :: res(:)
        data = c_hdc_get_data(this,trim(path)//c_null_char)
        if (data%rank /= 1) stop "incompatible ranks in hdc_as_int32_1d_path_sub"
        call c_f_pointer(data%data, res, data%dshape(1:data%rank))
    end subroutine hdc_as_int32_1d_path_sub

    module procedure hdc_set_int32_scalar

    subroutine hdc_set_int32_1d(this, data, flags_)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t), dimension(:), target :: data
        integer(kind=c_size_t), intent(in), optional :: flags_
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_)) flags = flags_
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(1+1:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, c_null_char, out);
    end subroutine hdc_set_int32_1d

    subroutine hdc_set_int32_1d_path(this, path, data, flags_)
        use iso_c_binding
        type(hdc_t) :: this
        integer(kind=c_int32_t), dimension(:), target :: data
        character(len=*), intent(in) :: path
        integer(kind=c_size_t), intent(in), optional :: flags_
        integer(kind=c_size_t) :: flags = HDCFortranOrder
        type(hdc_data_t) :: out
        if (present(flags_)) flags = flags_
        out%dtype = HDC_INT32
        out%flags = flags
        out%rank = 1
        out%dshape(1:1) = shape(data)
        out%dshape(1+1:) = 0
        out%data = c_loc(data)
        call c_hdc_set_data(this, trim(path)//c_null_char, out);
    end subroutine hdc_set_int32_1d_path
