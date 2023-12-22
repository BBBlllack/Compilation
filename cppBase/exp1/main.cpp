# include <iostream>
# include <ctype.h>
# include <string>
#include "functions.h"

using namespace std;

# define ID 6
# define UCON 7
# define LT 8
# define LE 9
# define EQ 10
# define NE 11
# define GT 12
# define GE 13
# define PL 14
# define MI 15
# define MU 16
# define DI 17

char TOKEN[20];
int line = 1; // 当前行

extern void report_error(int, char);

extern void out(int, char *);

extern int lookup(char *);

extern char *parseScientificNotation(const char *token);

void scanner_example(FILE *fp) {
    char ch;
    int i, c; // i为TOKEN索引
    while ((ch = fgetc(fp)) != EOF) {
        // 如果是空格和制表符则跳过
        if (ch == ' ' || ch == '\t') {
            continue;
        }
        if (ch == '\n') {
            line++;
            continue;
        }
        if (isalpha(ch)) /*it must be an identifer!*/ {
            // 标识符
            TOKEN[0] = ch;
            ch = fgetc(fp);
            i = 1;
            while (isalnum(ch)) {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
            }
            TOKEN[i] = '\0';
            fseek(fp, -1, 1); /* retract */
            c = lookup(TOKEN);
            if (c == 0)
                // 是标识符, 则输出(ID, 标识符内容)
                out(ID, TOKEN);
            else
                // 是关键字, 输出关键字
                out(c, " ");
        } else if (isdigit(ch) || ch == '.') {
            if (ch == '.') {
                TOKEN[0] = '0';
                TOKEN[1] = ch;
                i = 2;
                while (isdigit(ch = fgetc(fp))) {
                    TOKEN[i++] = ch;
                }
                fseek(fp, -1, 1);
                TOKEN[i] = '\0';
                out(UCON, TOKEN);
                continue;
            }
            TOKEN[0] = ch;
            ch = fgetc(fp);
            i = 1;
            // 如果类似1qw识别为非法单词
            if (isalpha(ch) && ch != 'e' && ch != 'E') {
                TOKEN[i++] = ch;
                while ((ch = fgetc(fp)) != ' ')
                    TOKEN[i++] = ch;
                TOKEN[i] = '\0';
                report_error(line, TOKEN);
                continue;
            }
            while (isdigit(ch) || ch == '.' || ch == 'E' || ch == 'e') {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
            }
            TOKEN[i] = '\0';
            fseek(fp, -1, 1);
            out(UCON, parseScientificNotation(TOKEN));
        } else {
            switch (ch) {
                case '<':
                    ch = fgetc(fp);
                    if (ch == '=')
                        out(LE, " ");
                    else if (ch == '>')
                        out(NE, " ");
                    else {
                        fseek(fp, -1, 1);
                        out(LT, " ");
                    }
                    break;
                case '=':
                    out(EQ, " ");
                    break;
                case '>':
                    ch = fgetc(fp);
                    if (ch == '=')
                        out(GE, " ");
                    else {
                        fseek(fp, -1, 1);
                        out(GT, " ");
                    }
                    break;
                case '+':
                    out(PL, " ");
                    break;
                case '-':
                    out(MI, " ");
                    break;
                case '*':
                    out(MU, " ");
                    break;
                case '/':
                    out(DI, " ");
                    break;
                case '(':
                    out(LP, " ");
                    break;
                case ')':
                    out(RP, " ");
                    break;
                default:
                    report_error(line, ch);
                    break;
            }
        }
    }
}

int main() {
/** 实验一测试用
+ - * / while
do 12 .34 3e4 th th1
1weq if myid>=1.5E1 then x=y
*/
    FILE *fp = fopen("/Users/shj/CLionProjects/compliationBak/cppBase/exp1/code.txt", "r");
    scanner_example(fp);

    return 0;
}
/** 和实验二配合用
if a = 4 then
    while i do
        if b = 5 then
            c = 1 + 5
        else
            d = 7 * 8
end
*/