/* 运行程序，需要用户输入两个数字（无所谓整形或浮点型），中间用空格隔开，分别输出它们的和差积商 */
#include <stdio.h>
int main(){
    double a, b;
    printf("请输入两个数字（用空格隔开）：");
    scanf("%lf %lf", &a, &b);
    printf("和：%.2lf\n", a + b);
    printf("差：%.2lf\n", a - b);
    printf("积：%.2lf\n", a * b);
    if(b != 0){
        printf("商：%.2lf\n", a / b);
    } else {
        printf("商：除数不能为零！\n");
    }
    return 0;
}