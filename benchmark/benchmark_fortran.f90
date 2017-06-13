program benchmark_fortran
    use hdc_fortran
    use iso_c_binding
    implicit none
    call f_main()
contains
    subroutine bm_create_delete(n_items)
        implicit none
        integer(8), intent(in) :: n_items
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        type(hdc_t), pointer :: tree
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
        
        do i = 1,n_items
            allocate(tree)
            tree = hdc_new_empty()
            call hdc_delete(tree)
            deallocate(tree)
        end do
        
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_create_delete         , n_items = ",i10,",Time = ",f16.14," seconds.")',n_items,e_time
    end subroutine bm_create_delete

    subroutine bm_create_delete_no_alloc(n_items)
        implicit none
        integer(8), intent(in) :: n_items
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        type(hdc_t), pointer :: tree
        allocate(tree)
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
        
        do i = 1,n_items
            tree = hdc_new_empty()
            call hdc_delete(tree)
        end do
        
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_create_delete_no_alloc, n_items = ",i10,",Time = ",f16.12," seconds.")',n_items,e_time
        deallocate(tree)
    end subroutine bm_create_delete_no_alloc

    subroutine bm_add_child(n_items)
        implicit none
        integer(8), intent(in) :: n_items
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        type(hdc_t), pointer :: tree
        character(len=8) :: fmt ! format descriptor
        character(len=30) :: path ! format descriptor
        fmt = '(I10)' ! an integer of width 5 with zeros at the left
        allocate(tree)
        tree = hdc_new_empty()
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
        
        do i = 1,n_items
            write (path,fmt)i
            call hdc_add_child(tree,trim(path),hdc_new_empty())
        end do
        
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_add_child, n_items = ",i10,",Time = ",f16.12," seconds.")',n_items,e_time
        deallocate(tree)
    end subroutine bm_add_child
    
    subroutine bm_get_child(max_items)
        implicit none
        integer(8), intent(in) :: max_items
        integer(8) :: i, clock_rate, clock_start, clock_stop, k
        real(8) :: e_time,r
        type(hdc_t), pointer :: tree, node
        character(len=8) :: fmt ! format descriptor
        character(len=10) :: paths(100)
        character(len=10) :: path ! format descriptor
        fmt = '(I8)' ! an integer of width 5 with zeros at the left
        allocate(tree)
        allocate(node)
        tree = hdc_new_empty()
        do i = 1,max_items
            write (path,fmt)i
            call hdc_add_child(tree,trim(path),hdc_new_empty())
        end do
        ! init random numbers paths
        do k = 1,100
            call random_number(r)
            write (paths(k),fmt)floor(r*max_items)
        end do
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
        
        do k = 1,100
            do i = 1,100000
                node = hdc_get_child(tree,trim(paths(k)))
            end do
        end do
        
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_get_child, n = ",i10,",Time = ",f16.12," seconds.")',max_items,e_time/100000/100
        deallocate(tree)
        deallocate(node)
    end subroutine bm_get_child
    
    subroutine bm_get_child_path(depth)
        implicit none
        integer(8), intent(in) :: depth
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        type(hdc_t), pointer :: tree, node
        character(len=4*depth-1) :: path ! format descriptor
        if (depth > 16) stop "Too deep."
        allocate(tree)
        allocate(node)
        tree = hdc_new_empty()
        path = ""
        do i = 1,depth-1
            path = trim(path)//"aaa/"
        end do
        path = trim(path)//"aaa"
        call hdc_add_child(tree,path,hdc_new_empty())

        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
        
        do i = 1,100000
            node = hdc_get_child(tree,trim(path))
        end do
        
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_get_child_path, n = ",i10,",Time = ",f16.12," seconds.")',depth,e_time/100000
        deallocate(tree)
        deallocate(node)
    end subroutine bm_get_child_path

    subroutine bm_set_data(array_size)
        implicit none
        integer(8), intent(in) :: array_size
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        real(8) :: data(array_size)
!         integer(4) :: data(array_size)
        type(hdc_t), pointer :: tree
        data = 1!.0d0
        allocate(tree)
        tree = hdc_new_empty()
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
        
        do i = 1,100
            call hdc_set_data(tree,data)
        end do
        
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_set_data, n = ",i10,",Time = ",f16.12," seconds.")',array_size,e_time/100
        deallocate(tree)
    end subroutine bm_set_data

    subroutine bm_set_data_preallocated(array_size)
        implicit none
        integer(8), intent(in) :: array_size
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        real(8) :: data(array_size)
!         integer(4) :: data(array_size)
        type(hdc_t), pointer :: tree
        data = 1!.0d0
        allocate(tree)
        tree = hdc_new_size(8*array_size)
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
        
        do i = 1,1
            call hdc_set_data(tree,data)
        end do
        
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_set_data_preallocated, n = ",i10,",Time = ",f16.12," seconds.")',array_size,e_time/1
        deallocate(tree)
    end subroutine bm_set_data_preallocated
    
    subroutine bm_array_copy(array_size)
        implicit none
        integer(8), intent(in) :: array_size
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        real(8) :: data(array_size), data_copy(array_size)
        data(:) = 1.0d0
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
        
        do i = 1,100
            data_copy(:) = data(:)
        end do
        
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_array_copy, array_size = ",i10,",Time = ",f16.12," seconds.")',array_size,e_time/100
    end subroutine bm_array_copy
    
    subroutine bm_get_data(array_size)
        implicit none
        integer(8), intent(in) :: array_size
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        real(8) :: data(array_size), data_copy(array_size)
        real(8), dimension(:), pointer :: data2
!         integer(4) :: data(array_size)
        type(hdc_t), pointer :: tree
        data(:) = 1
        allocate(tree)
        tree = hdc_new_empty()
        call hdc_set_data(tree,data)
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
        
        do i = 1,100
            call hdc_get(tree,data2)
            data_copy(:) = data2(:)
        end do
        
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_get_data, n = ",i10,",Time = ",f16.12," seconds.")',array_size,e_time/100
        deallocate(tree)
    end subroutine bm_get_data
    
    
    
    subroutine f_main()
        implicit none
        integer(8) :: max_pwr, base, arg, i
!         call bm_create_delete(1000000_8)
!         call bm_create_delete_no_alloc(1000000_8)
!         call bm_add_child(100000_8)
!         call bm_get_child(100000_8)
!         call bm_get_child_path(4_8)
!         
        max_pwr = 5
        base = 1024_8*1024_8
        do i = 2,2
            arg = base*2_8**i
            call bm_set_data(arg)
        end do
!         
!         max_pwr = 5
!         base = 1024_8*1024_8
!         do i = 0,max_pwr
!             arg = base*2_8**i
!             call bm_set_data_preallocated(arg)
!         end do
! 
!         max_pwr = 2
!         base = 1024_8*1024_8
!         do i = max_pwr,max_pwr
!             arg = base*2_8**i
!             call bm_get_data(arg)
!         end do

!         max_pwr = 5
!         base = 1024_8*1024_8
!         do i = 2,2
!             arg = base*2_8**i
!             call bm_array_copy(arg)
!         end do
    end subroutine f_main
end program benchmark_fortran
