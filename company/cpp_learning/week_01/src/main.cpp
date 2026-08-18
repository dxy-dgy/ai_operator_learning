#include <iostream>
#include <memory>
#include <vector>
namespace a {
        int x = 1;
    }
namespace b {
        int x = 2;
    }
int main() {
    auto p = std::make_unique<int>(10);
    std::cout << "p = " << *p << '\n';
    *p = 20;
    std::cout << "p = " << *p << '\n';
    std::vector<int> arr{1,2,3,4,5,6,7,8,9};
    for (size_t i= 0; i < arr.size(); i++) {
        std::cout << "arr[" << i << "] = " << arr[i] << std::endl;
    }
    auto brr = std::make_unique<int[]>(100);
    for (size_t i = 0; i < 100; i++){
        brr[i] = i;
    }
    auto *p1 = &arr[0];
    auto *p2 = &arr[1];
    auto *p3 = &brr[0];
    auto *p4 = &brr[1];
    std::cout << p1 << " " << p2 << " " << p3 << " " << p4 << std::endl;
    std::cout << a::x << " " << b::x << std::endl;
    return 0;
}
