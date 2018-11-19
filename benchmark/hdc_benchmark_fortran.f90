program benchmark_fortran
    use hdc_fortran
    use iso_c_binding
    use c_perf
    implicit none
    call f_main()
contains
    subroutine storage_reset()
        call hdc_destroy()
        call hdc_init("mdbm","{""filename"": ""/tmp/db1.mdbm"", ""persistent"": false}");
    end subroutine storage_reset

    subroutine bm_create_delete(n_items)
        implicit none
        integer(8), intent(in) :: n_items
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        type(hdc_t) :: tree
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

        do i = 1,n_items
            tree = hdc_new()
            call hdc_clean(tree)
        end do
        call storage_reset()
        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_create_delete, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2,"k items/s = ",E8.2,"s/item")',n_items,e_time,1.0e-3/e_time*n_items,e_time/n_items
        call storage_reset()
    end subroutine bm_create_delete

    subroutine bm_create_delete_no_alloc(n_items)
        implicit none
        integer(8), intent(in) :: n_items
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        type(hdc_t) :: tree
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

        do i = 1,n_items
            tree = hdc_new()
            call hdc_clean(tree)
        end do

        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_create_delete_no_alloc, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2,"k items/s = ",E8.2,"s/item")',n_items,e_time,1.0e-3/e_time*n_items,e_time/n_items
        call storage_reset()
    end subroutine bm_create_delete_no_alloc

    subroutine bm_add_child(n_items,child_size)
        implicit none
        integer(8), intent(in) :: n_items, child_size
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        type(hdc_t) :: tree
        character(len=8) :: fmt ! format descriptor
        character(len=30) :: path ! format descriptor
        fmt = '(I10)' ! an integer of width 5 with zeros at the left
        tree = hdc_new(child_size)
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

        do i = 1,n_items
            write (path,fmt)i
            call hdc_add_child(tree,path,hdc_new())
        end do

        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_add_child, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2,"k items/s = ",E8.2,"s/item")',n_items,e_time,1.0e-3/e_time*n_items,e_time/n_items
        call storage_reset()
    end subroutine bm_add_child

    subroutine bm_add_child_depth(depth,child_size)
        implicit none
        integer(8), intent(in) :: depth, child_size
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        type(hdc_t) :: tree
        character(len=2*depth) :: path ! format descriptor
        path = ""
        do i=1,depth-1
            path = trim(path)//"a/"
        end do
        path = trim(path)//"a"
        call system_clock(count_rate=clock_rate) !Find the time rate
        e_time = 0
        do i=1,1000
            tree = hdc_new(child_size)
            call system_clock(count=clock_start)     !Start Timer
            call hdc_add_child(tree,trim(path),hdc_new())
            call system_clock(count=clock_stop)      ! Stop Timer
            e_time = e_time + real(clock_stop-clock_start)/real(clock_rate)
            call hdc_clean(tree)
        end do
        print '("bm_add_child_depth, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2,"k items/s = ",E8.2,"s/item")',depth,e_time/100,1.0e-3/e_time*1000,e_time/1000
        call storage_reset()
    end subroutine bm_add_child_depth

    subroutine bm_get_child(max_items)
        implicit none
        integer(8), intent(in) :: max_items
        integer(8) :: i, clock_rate, clock_start, clock_stop, k
        real(8) :: e_time,r
        character(len=8) :: fmt ! format descriptor
        character(len=10) :: paths(100)
        character(len=10) :: path ! format descriptor
        type(hdc_t) :: tree, node
        fmt = '(I8)' ! an integer of width 5 with zeros at the left
        tree = hdc_new(1024_8*1024_8*4)
        do i = 0,max_items-1
            write (path,fmt)i
            call hdc_add_child(tree,trim(path),hdc_new())
        end do
        ! init random numbers paths
        do k = 1,100
            call random_number(r)
            write (paths(k),fmt)floor(r*max_items)
        end do
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer
!         call c_getMillis(t1)
!         call cpu_time(ts)
        do i = 1, 100
            do k = 1,100
                node = hdc_get_child(tree,paths(k))
            end do
        end do
!         call c_getMillis(t2)
        call system_clock(count=clock_stop)      ! Stop Timer
!         call cpu_time(tf)
        e_time = real(clock_stop-clock_start)/real(clock_rate)
!         e_time = (t2-t1)/dble(10000)
!         e_time = (tf-ts)
        print '("bm_get_child, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f16.2,"k items/s")',max_items,e_time/100_dp,100_dp/dble(e_time*1000/100)
        call storage_reset()
    end subroutine bm_get_child

!     subroutine bm_get_child_path(depth)
!         implicit none
!         integer(8), intent(in) :: depth
!         integer(8) :: i, clock_rate, clock_start, clock_stop
!         real(8) :: e_time
!         type(hdc_t) :: tree, node
!         character(len=4*depth-1) :: path ! format descriptor
!         if (depth > 16) stop "Too deep."
!
!         tree = hdc_new()
!         path = ""
!         do i = 1,depth-1
!             path = trim(path)//"aaa/"
!         end do
!         path = trim(path)//"aaa"
!         call hdc_add_child(tree,path,hdc_new())
!
!         call system_clock(count_rate=clock_rate) !Find the time rate
!         call system_clock(count=clock_start)     !Start Timer
!
!         do i = 1,100000
!             node = hdc_get_child(tree,trim(path))
!         end do
!
!         call system_clock(count=clock_stop)      ! Stop Timer
!         e_time = real(clock_stop-clock_start)/real(clock_rate)
!         print '("bm_get_child_path, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2,"k items/s = ",E8.6,"us/item")',depth,e_time/100,1.0e-3/e_time*100000,e_time/100000/1000000
!         call storage_reset()
!     end subroutine bm_get_child_path

    subroutine bm_set_data(array_size)
        implicit none
        integer(8), intent(in) :: array_size
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        real(8) :: data(array_size)
        type(hdc_t) :: tree
        data = 1!.0d0
        tree = hdc_new()
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

        do i = 1,100
            call hdc_set_data(tree,data)
        end do

        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_set_data, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2," GB/s")',array_size,e_time/100,8.0_dp*array_size/dble(2**30*e_time)*100
        call storage_reset()
    end subroutine bm_set_data

    subroutine bm_set_ets()
        implicit none
        integer(8), parameter :: array_size = 100000
        integer(8) :: i, clock_rate, clock_start, clock_stop, j, k
        real(8) :: e_time
        real(8) :: data(array_size)
        type(hdc_t) :: tree
        character(len=32) :: fmt
        character(len=32) :: path
        data = 1!.0d0
        fmt = '("a"I0.2,"/a",I0.2,"/a",I0.2)' ! an integer of width 3 with zeros at the left
        tree = hdc_new(4096_8)

        do i = 0,100
            do j = 0,100
                do k = 0,100
                    write (path,fmt)i,j,k
                    call hdc_add_child(tree,trim(path),hdc_new())
                end do
            end do
        end do

        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

        do i = 1,100
            call hdc_set_data(tree,"a10/a10/a10",data)
        end do

        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_set_ets, n = ",i10,",Time = ",f16.12,"s")',array_size,e_time
        call storage_reset()
    end subroutine bm_set_ets

    subroutine bm_get_ets()
        implicit none
        integer(8), parameter :: array_size = 100000
        integer(8) :: i, clock_rate, clock_start, clock_stop,j,k
        real(8) :: e_time
        real(8) :: data(array_size)
        real(8) :: gdata(array_size)
        type(hdc_t) :: tree
        character(len=32) :: fmt
        character(len=32) :: path
        data = 1!.0d0
        tree = hdc_new(4096_8)
        fmt = '("a"I0.2,"/a",I0.2,"/a",I0.2)' ! an integer of width 3 with zeros at the left
        do i = 1,100
            do j = 1,100
                do k = 1,100
                    write (path,fmt)i,j,k
                    call hdc_add_child(tree,trim(path),hdc_new())
                end do
            end do
        end do
        call hdc_set_data(tree,"a10/a10/a10",data)
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

        do i = 1,100
            gdata = hdc_as_double_1d(tree,"a10/a10/a10")
        end do

        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_get_ets, n = ",i10,",Time = ",f16.12,"s")',array_size,e_time
        call storage_reset()
    end subroutine bm_get_ets

    subroutine bm_construct_ets()
        implicit none
        integer(8) :: array_size = 100
        integer(8) :: i, clock_rate, clock_start, clock_stop,j,k,l
        real(8) :: e_time
        real(8) :: data(100)
        type(hdc_t) :: tree
        character(len=32) :: fmt
        character(len=32) :: path(1000000)
        data = 1!.0d0
        tree = hdc_new(4096_8)
        fmt = '("a"I0.2,"/a",I0.2,"/a",I0.2)' ! an integer of width 3 with zeros at the left
        l = 1
        do i = 1,100
            do j = 1,100
                do k = 1,100
                    write (path(l),fmt)i,j,k
                    l = l + 1
                end do
            end do
        end do
        call hdc_set_data(tree,"a10/a10/a10",data)
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

            do l = 1, 1000000
                call hdc_add_child(tree,trim(path(l)),hdc_new())
            end do

        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_construct_ets, n = ",i10,",Time = ",f16.12,"s")',array_size,e_time
        call storage_reset()
    end subroutine bm_construct_ets


!     subroutine bm_set_data_preallocated(array_size)
!         implicit none
!         integer(8), intent(in) :: array_size
!         integer(8) :: i, clock_rate, clock_start, clock_stop
!         real(8) :: e_time
!         real(8) :: data(array_size)
!         type(hdc_t) :: tree
!         data = 1!.0d0
!         tree = hdc_new(8*array_size)
!         call system_clock(count_rate=clock_rate) !Find the time rate
!         call system_clock(count=clock_start)     !Start Timer
!
!         do i = 1,100
!             call hdc_set_data(tree,data)
!         end do
!
!         call system_clock(count=clock_stop)      ! Stop Timer
!         e_time = real(clock_stop-clock_start)/real(clock_rate)
!         print '("bm_set_data_preallocated, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2," GB/s")',array_size,e_time/100,8.0_dp*array_size/dble(2**30*e_time)*100
!         call storage_reset()
!     end subroutine bm_set_data_preallocated

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
        print '("bm_array_copy, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2," GB/s")',array_size,e_time/100,8_8*array_size/dble(2**30*e_time)*100
        call storage_reset()
    end subroutine bm_array_copy

!     subroutine bm_array_copy_ptr(array_size)
!         implicit none
!         integer(8), intent(in) :: array_size
!         integer(8) :: i, clock_rate, clock_start, clock_stop
!         real(8) :: e_time
!         real(8), pointer :: data(:), data_copy(:)
!         allocate(data(array_size), data_copy(array_size))
!         data(:) = 1.0d0
!         call system_clock(count_rate=clock_rate) !Find the time rate
!         call system_clock(count=clock_start)     !Start Timer
!         do i = 1,100
!             data_copy(:) = data(:)
!         end do
!         call system_clock(count=clock_stop)      ! Stop Timer
!         e_time = real(clock_stop-clock_start)/real(clock_rate)
!         print '("bm_array_copy_ptr, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2," GB/s")',array_size,e_time/100,8_8*array_size/dble(2**30*e_time)*100
!         call storage_reset()
!     end subroutine bm_array_copy_ptr

    subroutine bm_get_data(array_size)
        implicit none
        integer(8), intent(in) :: array_size
        integer(8) :: i, clock_rate, clock_start, clock_stop
        real(8) :: e_time
        real(8) :: data(array_size), data_copy(array_size)
        real(8), dimension(:), pointer :: data2
        type(hdc_t) :: tree
        data(:) = 1
        tree = hdc_new()
        call hdc_set_data(tree,data)
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

        do i = 1,100
            call hdc_get(tree,data2)
            data_copy(:) = data2(:)
        end do

        call system_clock(count=clock_stop)      ! Stop Timer
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_get_data, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2," GB/s")',array_size,e_time/100,100*8_8*array_size/dble(2**30*e_time)
        call storage_reset()
    end subroutine bm_get_data

    subroutine bm_get_data_zero_copy(array_size)
        implicit none
        integer(8), intent(in) :: array_size
        integer(8) :: i, clock_rate, clock_start, clock_stop, k
        real(8) :: e_time
        real(8) :: data(array_size)
        real(8), dimension(:), pointer :: data2
        real(8) :: s
        type(hdc_t) :: tree
        data(:) = 1
        tree = hdc_new()
        call hdc_set_data(tree,data)
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

        do i = 1,100
            call hdc_get(tree,data2)
            do k=1,array_size
                s = data2(k)
            end do
        end do
        call system_clock(count=clock_stop)      ! Stop Timer
        if (s < 0) print *,s
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_get_data_zero_copy, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2," GB/s")',array_size,e_time/100,8_8*array_size/dble(2**30*e_time)*100
        call storage_reset()
    end subroutine bm_get_data_zero_copy

    subroutine bm_set_data_zero_copy(array_size)
        implicit none
        integer(8), intent(in) :: array_size
        integer(8) :: i, clock_rate, clock_start, clock_stop, k
        real(8) :: e_time
        real(8) :: data(array_size)
        real(8), dimension(:), pointer :: data2
        real(8) :: s = 1
        type(hdc_t) :: tree
        data(:) = 1
        tree = hdc_new()
        call hdc_set_data(tree,data)
        call system_clock(count_rate=clock_rate) !Find the time rate
        call system_clock(count=clock_start)     !Start Timer

        do i = 1,100
            call hdc_get(tree,data2)
            do k=1,array_size
                data2(k) = 2.0d0
            end do
        end do
        call system_clock(count=clock_stop)      ! Stop Timer
        if (s < 0) print *,s
        e_time = real(clock_stop-clock_start)/real(clock_rate)
        print '("bm_set_data_zero_copy, n = ",i10,",Time = ",f16.12,"s,  Throughput = ",f6.2," GB/s")',array_size,e_time/100,8_8*array_size/dble(2**30*e_time)*100
        call storage_reset()
    end subroutine bm_set_data_zero_copy

    subroutine f_main()
        implicit none
        integer(8) :: k, n
        call bm_array_copy(1024_8*1024_8*4_8)
        do k=1,6
            n = 2**k
            call bm_set_data(1024_8*1024_8*n)
        end do
        do k=1,6
            n = 2**k
            call bm_get_data(1024_8*1024_8*n)
        end do
        do k=1,6
            n = 2**k
            call bm_get_data_zero_copy(1024_8*1024_8*n)
        end do
        do k=1,6
            n = 2**k
            call bm_set_data_zero_copy(1024_8*1024_8*n)
        end do
        call bm_create_delete(1000000_8)
        call bm_create_delete_no_alloc(1000000_8)
        do k=4,4
            n = 2**k
            call bm_get_child(n)
        end do
        do k=2,5
            n = 2**k
            call bm_add_child(n*1024_8,0_8)
        end do
        do k=0,5
            n = 2**k
            call bm_add_child_depth(n,1024_8)
        end do
        call bm_construct_ets()
        call bm_set_ets()
        call bm_get_ets()
    end subroutine f_main
end program benchmark_fortran
