/* 请用户输入一串数列，都是int类型，然后得到最大值和最小值，并求和 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(){
    int numbers[100];
    int count = 0;
    int sum = 0;
    int max, min;
    char input[500];
    printf("请输入一串整数，以空格分隔，输入结束后按回车：\n");
    fgets(input, sizeof(input), stdin);
    char *token = strtok(input, " ");
    while(token != NULL){
        numbers[count] = atoi(token);
        sum += numbers[count];
        if(count == 0){
            max = numbers[count];
            min = numbers[count];
        } else {
            if(numbers[count] > max){
                max = numbers[count];
            }
            if(numbers[count] < min){
                min = numbers[count];
            }
        }
        count++;
        token = strtok(NULL, " ");
    }
    printf("最大值: %d\n", max);
    printf("最小值: %d\n", min);
    printf("总和: %d\n", sum);
    return 0;
}