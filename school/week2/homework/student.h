#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

/* 学生信息结构体 */
typedef struct {
    int id;
    char name[20];
    int age;
} Student;

/* 链表节点 */
typedef struct Node {
    Student data;
    struct Node *next;
} Node;

/* 链表操作函数声明 */
Node* create_head(void);
void add_student(Node *head);
void show_students(Node *head);
void save_to_file(Node *head, const char *filename);
void free_list(Node *head);

#endif
