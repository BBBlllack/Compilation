#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "functions.h"

#define MAXSIZE 100
#define null "null"
#define none ""
#define true 1

using namespace std;

char ch; // 当前字符
int row = 0, col = 0; // 当前行列

/**
 * 封装原生strcmp，符合自己的习惯
 * @param src
 * @param dest
 * @return
 */
int mystrcmp(char src[], char dest[]) {
    return !strcmp(src, dest);
}

/**
 * 逆序入栈
 * @param s 入栈串
 */
void pushRev(char *s) {
    int len = strlen(s);
    while (len) {
        push(s[--len]);
    }
}

// 预测分析表
// 三个维度分别代表 row, col, len
// 其中用e表示E' 用t表示T'
char table[10][10][10] = {
        {"Te", null, null,   null, null, "Te",  null, null},
        {null, "ATe", "ATe", null, null, null,  none, none},
        {"Ft", null, null,   null, null, "Ft",  null, null},
        {null, none, none, "MFt", "MFt", null,  none, none},
        {"i",  null, null,   null, null, "(E)", null, null},
        {null, "+",   "-",   null, null, null,  null, null},
        {null, null, null, "*",   "/",   null,  null, null}
};
// 定义非终结符
map<char, int> noTerminal = {{'E', 0},
                             {'e', 1},
                             {'T', 2},
                             {'t', 3},
                             {'F', 4},
                             {'A', 5},
                             {'M', 6}};
// 定义终结符
map<char, int> terminal = {{'i', 0},
                           {'+', 1},
                           {'-', 2},
                           {'*', 3},
                           {'/', 4},
                           {'(', 5},
                           {')', 6},
                           {'#', 7}};

char token[MAXSIZE]; // 当前已经扫描的字符串
int token_index = 0; // token索引
//char input[MAXSIZE] = {"i+i*i-i/i# "};
//char input[MAXSIZE] = {"(i+i))+i/i-#"};
char input[MAXSIZE] = {"i-ii+#"};
int chindex = 0; // 指向当前ch的索引

void report_error() {
    printf("GRAMMAR IS ERROR!\n");
}

// 文件输出流
ofstream outputFile("parse.txt");

/**
 * 报错
 * @param s 报错信息
 */
void report_error(char *s) {
    char temp[MAXSIZE];
    sprintf(temp, "%s WITH %s\n", s, token);
    outputFile << temp << endl;
    printf("%s WITH %s\n", s, token);
}

/**
 * 识别成功
 * @param c 字符
 */
void success_recognize(char c) {
    char temp[MAXSIZE];
    sprintf(temp, "SUCCESS to recognize symbol %c\n", c);
    outputFile << temp << endl;
    printf("SUCCESS to recognize symbol %c\n", c);
}

// i+i
char *lookup(char symbol, char ch) {
    char *result = table[noTerminal[symbol]][terminal[ch]];
    if (mystrcmp(result, null)) {
        return NULL;
    }
    return result;
}

/**
 * 解析程序
 */
void parse() {
    char symbol;
    char temp[MAXSIZE];
    while ((ch = input[chindex++]) != '#') {
        token[token_index++] = ch;
        sprintf(temp, "current char: %c\n", ch);
        outputFile << temp << endl;
        printf("current char: %c\n", ch);
        if (ch == ' ') {
            continue;
        }
        while (true) {
            symbol = pop();
            if (symbol == ' ') {
                symbol = pop();
            }
            // 栈空但仍未分析完
            if (symbol == NULL) {
                report_error("ERROR! STACK IS NULL BUT NOT FINISHED!");
                return;
            }
            if (symbol == ch) {
                // 识别成功
                success_recognize(ch);
                break;
            }
            char *s = lookup(symbol, ch);
            // 查表空 例如-无法直接出现在+后面
            if (s == NULL) {
                char buffer[MAXSIZE];
                sprintf(buffer, "ERROR! THE SYMBOL %c can't appear at the back of %c", token[token_index - 1],
                        token[token_index - 2]);
                report_error(buffer);
                return;
            }
            // 识别到空串入栈空格，方便后续操作
            if (strlen(s) == 0) {
                push(' ');
                continue;
            }
            pushRev(s);
//            printf("%s\n", s);
            // 查看当前栈信息
            PRINT_STACK(outputFile);
        }
    }
    // 句子识别完之后的栈信息
    PRINT_STACK(outputFile);
    // 判断栈中剩余的是否可以识别为空串
    char tmp;
    char buffer[MAXSIZE];
    sprintf(buffer, "ERROR! THE SYMBOL '%c' can't appear at the end of a sentence", token[token_index - 1]);
    while ((tmp = pop()) != '#') {
        char *s = lookup(tmp, '#');
        // 剩余的非终结符无法推导出空串使程序结束
        if (s == NULL) {
            report_error(buffer);
            return;
        }
        if (strlen(s) != 0) {
            report_error(buffer);
            return;
        }
    }
    printf("RIGHT!");

}

/**
 * 通过不同的input就可以查看不同的输出, input为char[], 在上面定义
 * 会产生parse.txt文件，显示分析过程，也就是控制台的输出
 * @return
 */
int main() {
    push('#');
    push('E');
//    printf("input string: ");
//    gets(input);
    printf("dest string: %s\n", input);
    char temp[MAXSIZE];
    sprintf(temp, "dest string: %s\n", input);
    outputFile << temp << endl;
    if (!checkBracket(input)) {
        // 括号不匹配错误
        sprintf(temp, "ERROR! BRACKET IS MISMATCH!\n");
        outputFile << temp << endl;
        report_error("ERROR! BRACKET IS MISMATCH!\n");
    } else {
        parse();
    }
    outputFile.close();

    return 0;
}