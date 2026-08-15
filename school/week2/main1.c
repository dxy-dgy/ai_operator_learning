#include <stdio.h>
#include <stdlib.h>
int main(){
    /*更常用的结构体定义方式*/
    typedef struct {
        int id;
        char gender;
        double score;
    } student;
    /*结构体声明,初始化*/
    student s1 = {
        .id = 23001, 
        .gender = 'F', 
        .score = 80.3,
    };
    printf("学生s1的个人信息为：\n");
    printf("id为%d\n", s1.id);
    printf("性别:%c\n", s1.gender);
    printf("分数:%.1lf\n", s1.score);
    return 0;
}