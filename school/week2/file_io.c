#include <stdio.h>
int main(){
    int a[5] = {1,2,3,4,5};
    FILE* fp = fopen("data.bin","wb");
    if (!fp) return 1;
    fwrite(a, sizeof(int), 5, fp);
    fclose(fp);
    return 0;
}