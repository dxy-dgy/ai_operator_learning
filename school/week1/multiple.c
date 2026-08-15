/*我要打印99乘法表*/
#include<stdio.h>
int main(){
    for (int i = 1; i < 10; i++){
        for (int j = 1; j <= i; j++){
            printf("%d x %d = %-2d  ", j, i, j*i);
        }
        printf("\n");
    }
    return 0;
}