#include<stdio.h>
int factorial(int n){
    if (n == 1){
        return 1;
    }else{
        return n * factorial(n-1);
    }
}
int main(){
    int n, sum = 0;
    printf("请输入一个正整数：");
    scanf("%d", &n);
    for (int i = 1; i <= n; i++){
        sum += factorial(i);
    }
    printf("1! + 2! + ... + %d! = %d\n", n, sum);
    return 0;
}