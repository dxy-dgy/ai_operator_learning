#include <stdio.h>
#include "student.h"

int main(void)
{
    Node *head = create_head();
    if (head == NULL) {
        return 1;
    }

    int choice;

    while (1) {
        printf("\n===== 学生管理系统 =====\n");
        printf("1. 添加学生\n");
        printf("2. 显示所有学生\n");
        printf("3. 保存到文件\n");
        printf("4. 退出\n");
        printf("请选择：");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            add_student(head);
            break;
        case 2:
            show_students(head);
            break;
        case 3:
            save_to_file(head, "students.dat");
            break;
        case 4:
            free_list(head);
            printf("退出系统\n");
            return 0;
        default:
            printf("无效选择\n");
        }
    }
}
