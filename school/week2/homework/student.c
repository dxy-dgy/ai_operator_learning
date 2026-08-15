#include <stdio.h>
#include <stdlib.h>
#include "student.h"

/* 创建带头节点的链表 */
Node* create_head(void)
{
    Node *head = (Node *)malloc(sizeof(Node));
    if (head == NULL) {
        perror("malloc");
        return NULL;
    }
    head->next = NULL;
    return head;
}

/* 添加学生（头插法） */
void add_student(Node *head)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("malloc");
        return;
    }

    printf("请输入学号：");
    scanf("%d", &new_node->data.id);

    printf("请输入姓名：");
    scanf("%19s", new_node->data.name);

    printf("请输入年龄：");
    scanf("%d", &new_node->data.age);

    new_node->next = head->next;
    head->next = new_node;
}

/* 显示所有学生 */
void show_students(Node *head)
{
    Node *p = head->next;
    if (p == NULL) {
        printf("暂无学生信息\n");
        return;
    }

    while (p != NULL) {
        printf("学号：%d 姓名：%s 年龄：%d\n",
               p->data.id,
               p->data.name,
               p->data.age);
        p = p->next;
    }
}

/* 保存到文件 */
void save_to_file(Node *head, const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("fopen");
        return;
    }

    Node *p = head->next;
    while (p != NULL) {
        fwrite(&p->data, sizeof(Student), 1, fp);
        p = p->next;
    }

    fclose(fp);
    printf("保存成功\n");
}

/* 释放链表 */
void free_list(Node *head)
{
    Node *p = head;
    while (p != NULL) {
        Node *tmp = p;
        p = p->next;
        free(tmp);
    }
}
