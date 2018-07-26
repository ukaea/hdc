program test_hdc_fortran
    use hdc_fortran
    use iso_c_binding
    implicit none
    call f_main()
contains

    subroutine f_main()
        type(hdc_t), pointer :: tree, subtree, data, node, node2
        integer(kind=4), dimension(1:4) :: array
        integer(kind=4), dimension(:), pointer:: array2
        integer(kind=c_long), dimension(1:1) :: shape2
        integer(kind=8) :: rank2
        character(len=:), allocatable :: str
        integer, parameter :: nx = 2, ny = 3
        real(kind=DP) :: array2d(nx,ny)
        ! init array first
        array2d(:,:) = 2.22_dp
!         array2d(:,3) = 0.0_dp
        array2d(2,:) = 3.14159_dp
        allocate(tree)
        allocate(subtree)
        allocate(data)
        allocate(node)
        allocate(node2)
        call hdc_init()
        ! Create new HDC tree
        tree = hdc_new_empty()

        ! Add some children
        call hdc_add_child(tree,"aaa/bbb/ccc",hdc_new_empty())
        call hdc_add_child(tree,"aaa/bbb/eee",hdc_new_empty())
        call hdc_add_child(tree,"bbb/eee/aaa",hdc_new_empty())
        call hdc_set_data(tree,"aaa/bbb/ccc","pokus")
        call hdc_set(tree,"scalar",3.14159_dp)
        str = hdc_as_string(tree,"aaa/bbb/ccc")
        ! Get subtree
        subtree = hdc_get_child(tree,"aaa/bbb")
        ! Get node
        node = hdc_get_child(subtree,"ccc")

        ! Ask whether child exists
        print *,"exists: ", hdc_exists(tree,"aaa/bbb/ccc")

        ! Delete subtree
        call hdc_delete_child(tree,"aaa/eee")

        node2 = hdc_new_empty()
        call hdc_set_data(node2,array2d)
        call hdc_add_child(tree,"2ddata",node2)

        call hdc_dump(tree)

        ! Prepare some data
        array = [7,20,3,4]

        ! Add data to a single node
        data = hdc_new_empty()
        call hdc_set_data(data,array)

        ! Add data to subtree
        call hdc_set_data(tree,"aaa/bbb/ccc",array)
        ! Ask on some data details, use subtree to shorten path
        rank2 = hdc_get_rank(node)
        shape2 = hdc_get_shape(node)
        print *,"Dimension: ", rank2
        print *,"Shape: ", shape2
        call hdc_get(node,array2)
        print *,array2
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        call hdc_set_data(node,3*array2(:))
        call hdc_get(node,array2)
        print *,array2
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        ! Serialize data to JSON
        call hdc_to_json(tree,"tree.txt",0)
        ! test dump
        call hdc_dump(tree)
    end subroutine f_main

end program test_hdc_fortran
