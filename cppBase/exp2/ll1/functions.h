#define MAXSIZE 100
#include <fstream>
#include <iostream>

using namespace std;
// 构造分析栈
struct Stack {
    char data;
    Stack *next;
};
Stack stack, *pstack = &stack;

/**
 * 压栈函数
 * @param data 要压入的数据
 */
void push(char data) {
    Stack *temp = (Stack *) malloc(sizeof(Stack));
    temp->data = data;
    temp->next = pstack->next;
    pstack->next = temp;
}

/**
 * 弹栈函数
 * @return
 */
char pop() {
    Stack *temp;
    temp = pstack->next;
    if (!temp) {
        return NULL;
    }
    pstack->next = temp->next;
    char a = temp->data;
    free(temp);
    return a;
}

/**
 * 输出栈内容
 * @return 内容字符串指针
 */
char* PRINT_STACK() {
    char tmp[MAXSIZE]; // 临时输出字符串数组
    char i = 0;
    Stack *temp = pstack; // 临时栈指针
    int offset = 0;
    printf("STACK: ");
    while (temp->next) {
        offset += sprintf(tmp + offset, "%c ", temp->next->data);
        temp = temp->next;
    }
    printf("%s\n", tmp);
    return tmp;
}

/**
 * 输出栈的内容到文件中
 * @param output 文件输出流
 * @return
 */
char* PRINT_STACK(ofstream &output) {
    char tmp[MAXSIZE]; // 临时输出字符串数组
    char i = 0;
    Stack *temp = pstack; // 临时栈指针
    int offset = 0;
    printf("STACK: ");
    while (temp->next) {
        offset += sprintf(tmp + offset, "%c ", temp->next->data);
        temp = temp->next;
    }
    output << "STACK: ";
    output << tmp << endl;
    printf("%s\n", tmp);
    return tmp;
}


// 检查括号是否匹配
// ----------------------------------
Stack check, *pcheck = &check;
void push(Stack *top, char data){
    Stack *p = (Stack *) malloc(sizeof (struct Stack));
    p->data = data;
    p->next = top->next;
    top->next = p;
}

char pop(Stack *top){
    Stack *p;
    p = top->next;
    top->next =p->next;
    char a = p->data;
    free(p);
    return a;
}

/**
 * 检查括号是否匹配
 * @param string 输入串
 * @return
 */
int checkBracket(char *string){
    Stack *left = (Stack*) malloc(sizeof(struct Stack));
    int i = 0;
    int f = 1;
    while (string[i] || string[i] == ' '){
        char tmp;
        switch (string[i++]) {
            case '(':
                push(left,'(');
                break;
            case '[':
                push(left,'[');
                break;
            case '{':
                push(left,'{');
                break;
            case ')':
                if (left->next == NULL){
                    f = 0;
                    break;
                }
                tmp = pop(left);
                if (tmp != '('){
                    f = 0;
                }
                break;
            case ']':
                if (left->next == NULL){
                    f = 0;
                    break;
                }
                tmp = pop(left);
                if (tmp != '['){
                    f = 0;
                }
                break;
            case '}':
                if (left->next == NULL){
                    f = 0;
                    break;
                }
                tmp = pop(left);
                if (tmp != '{') {
                    f = 0;
                }
                break;
            default:
                break;
        }
    }
    return f && left->next == NULL;
}



