#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    int val;
    struct Node* next;
} Node;
void head_insert(Node* head, int val){
    // 头插法增加节点
    if (head == NULL){
        return;
    }
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->val = val;
    new_node->next = head->next;
    head->next = new_node;
}
void tail_insert(Node* head, int val){
    // 尾插法增加节点
    if (head == NULL){
        return;
    }
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->val = val;
    new_node->next = NULL;
    Node* p = head;
    while (p->next != NULL){
        p = p->next;
    }
    p->next = new_node;
}
void print_all(Node* head){
    // 打印链表所有节点的值
    if (head == NULL || head->next == NULL){
        printf("当前为空链表！\n");
        return;
    }
    Node* p = head->next;
    int i = 1;
    printf("以下为链表的值：\n");
    while (p->next != NULL){
        printf("第%d个值为%d\n", i, p->val);
        p = p->next;
        i+=1;
    }
    printf("第%d个值为%d\n", i, p->val);
}
int main(){
    // 申请一个头节点
    Node* head = malloc(sizeof(Node));
    if (head == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    head->next = NULL;
    print_all(head);
    head_insert(head, 3);
    print_all(head);
    head_insert(head, 2);
    head_insert(head, 1);
    head_insert(head, 0);
    print_all(head);
    tail_insert(head, 4);
    tail_insert(head, 5);
    print_all(head);
    return 0;
}