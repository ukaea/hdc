#include <iostream>
#include "hdc.hpp"

using namespace std;

int main()
{
    std::cout << "is_trivial " << std::is_trivial<HDC>::value << '\n';
    std::cout << "is_trivially_copyable " << std::is_trivially_copyable<HDC>::value << '\n';
    std::cout << "is_standard_layout " << std::is_standard_layout<HDC>::value << '\n';
    std::cout << "is_pod " << std::is_pod<HDC>::value << '\n';
    std::cout << "is_trivially_assignable " << std::is_trivially_assignable<HDC,HDC>::value << '\n';
    std::cout << "is_trivially_constructible " << std::is_trivially_constructible<HDC>::value << '\n';
    std::cout << "is_trivially_copy_assignable " << std::is_trivially_copy_assignable<HDC>::value << '\n';
    std::cout << "is_trivially_copy_constructible " << std::is_trivially_copy_constructible<HDC>::value << '\n';
    std::cout << "is_trivially_default_constructible " << std::is_trivially_default_constructible<HDC>::value << '\n';
    std::cout << "is_trivially_destructible " << std::is_trivially_destructible<HDC>::value << '\n';
    std::cout << "is_trivially_move_assignable " << std::is_trivially_move_assignable<HDC>::value << '\n';
    std::cout << "is_trivially_move_constructible " << std::is_trivially_move_constructible<HDC>::value << '\n';

    return 0;
}
