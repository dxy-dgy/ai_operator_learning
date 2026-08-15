#include <stdio.h>
#include <stdlib.h>
int main(){
    /*结构体定义，声明一次完成*/
    struct Student{
        long int id;
        char gender;
        int age;
        double score;
    } s1, s2;
    /*结构体的顺序初始化*/
    struct Student s3 = {23210180086, 'M', 20, 88.5};
    /*结构体的指定成员初始化*/
    struct Student s4 = {
        .id=23210180087, 
        .score=92.0,
        .age = 21,
        .gender = 'F',
    }; 
    return 0;
}