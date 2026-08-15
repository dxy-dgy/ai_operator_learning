#include<stdio.h>
#include<stdlib.h>
int main(){
    int n = 100;
    /*申请内存*/
    int* p = malloc(n * sizeof *p);
    /*检查内存是否申请成功*/
    if (p == NULL){
        perror("malloc");
        return 1;
    }
    for (int i = 0; i < n; i++){
        p[i] = i;
    }
    /*释放内存*/
    free(p);
    p = NULL;
    return 0;
}