#include <iostream>
int max(int x, int y){
    int z;
    if (x > y){
        z = x;
    }else{
        z = y;
    }
    return (z);
}
int main(){
    int x,y;
    std::cout << "请输入两个整数，用空格分离:\n";
    std::cin >> x >> y;
    std::cout << "这两个整数中比较大的数是：" << max(x, y) << std::endl;
    return 0;
}