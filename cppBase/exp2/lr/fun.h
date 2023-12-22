#include<fstream>
#include <iostream>
#include<string>
#include "functions.h"

#define REAL 9
#define TOKENLEN 30
#define MAX_KEY_NUMBER 40 /*关键字的最大数量*/
using namespace std;
const char *KEY_WORD_END = "Waiting for extending"; /*关键字结束标记*/
const char *KEY_WORD_START = "Waiting for starting"; /*关键字开始标记*/
const char *Keyword[MAX_KEY_NUMBER] = {KEY_WORD_START, "if", "else", "for", "do", "begin", "end",
                                           KEY_WORD_END};/*关键词数组*/
const char *Value[MAX_KEY_NUMBER] = {KEY_WORD_START, "IF", "ELSE", "FOR", "DO", "BEGIN", "END", "ID", "INT", "REAL",
                                         "LT", "LE", "EQ", "NE", "GT", "GE", "IS", "PL", "MI", "MU", "DI", "AND", "OR",
                                         "NOT", "OCT", "HEX", "CHAR", "STR", "LP", "RP", "#", KEY_WORD_END};/*助记符数组*/
char TOKEN[TOKENLEN];/*存放单词词文*/
int lines = 1;/*记录当前运行到的行数*/

 /**
  * 显示错误信息
  * @param s 错误信息
  */
void report_error(string s) {
    cout << "词法错误：error: in (" << lines << ")lines, " << s << endl;
}

 /**
  * 以token查询关键字表
  * @param token 待查字符串
  * @return 如果为关键字，返回所处位置，没有找到返回0
  */
int lookUp(char *token) {
    int i = 1;
    while (strcmp(Keyword[i], KEY_WORD_END) != 0) {
        if (strcmp(token, Keyword[i]) == 0)return i;
        i++;
    }
    return 0;
}

 /**
  * 输出二元式
  * @param a 单词的value值
  * @param token 有值单词的值
  * @return
  */
string out(int a, const char *token) {
    return Value[a];
}

 /**
  * 扫描文件字符并拼接token进行查询
  * @param fp 文件指针
  * @return
  */
string scanner_example(FILE *fp) {
    char ch;
    int i, c;
    while ((ch = fgetc(fp)) != EOF) {
//        每次清空TOKEN
        for (int i = 0; i < TOKENLEN; ++i){
            TOKEN[i] = '\0';
        }
        if (ch == ' ' || ch == '\t')//不识别空格和tab
        {
            ch = fgetc(fp);
            fseek(fp, -1, SEEK_CUR);
        } else if (ch == '\n')//不识别换行
        {
            lines++;
        } else if (isalpha(ch))  /*it must be a identifer!*/
        {
            TOKEN[0] = ch;
            ch = fgetc(fp);
            i = 1;
            while (isalnum(ch)) {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
            }
            TOKEN[i] = '\0';
            if (ch != EOF) {
                fseek(fp, -1, SEEK_CUR);  /* retract*/
            }
            c = lookUp(TOKEN);
            if (c == 0) return out(7, TOKEN);
            else if (c == 6)return out(6, " ");
            else return out(c, " ");
        } else if (isdigit(ch) || ch == '.') {
            if (ch == '0') {
                ch = fgetc(fp);
                if (ch >= '0' && ch < '8') {
                    TOKEN[0] = '0';
                    i = 1;
                    while (isdigit(ch)) {
                        TOKEN[i] = ch;
                        i++;
                        ch = fgetc(fp);
                    }
                    TOKEN[i] = '\0';
                    return out(24, TOKEN);
                } else if (ch == 'x') {
                    TOKEN[0] = '0';
                    TOKEN[1] = 'x';
                    ch = fgetc(fp);
                    i = 2;
                    while (ch >= '.' && ch <= '9' || ch >= 'a' && ch <= 'f' || ch >= 'A' && ch <= 'F') {
                        TOKEN[i] = ch;
                        i++;
                        ch = fgetc(fp);
                    }
                    TOKEN[i] = '\0';
                    return out(25, TOKEN);
                } else if (ch != '.' && !isdigit(ch)) {
                    return out(8, "0\0");
                } else {
                    fseek(fp, -1, SEEK_CUR);  /* retract*/
                }
            } else if (isdigit(ch) || ch == '.') {
                TOKEN[0] = ch;
                ch = fgetc(fp);
                i = 1;
                while (isdigit(ch) || ch == '.' || ch == 'e' || ch == 'E') {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                }
                if (ch != EOF) {
                    // 到达文件末尾，此时不应回退
                    fseek(fp, -1, SEEK_CUR);
                }
                char *temp = parseScientificNotation(TOKEN);
                return out(REAL, temp);
            }
        } else {
            switch (ch) {
                case '#':
                    return "#";
                case ':':
                    ch = fgetc(fp);
                    if (ch == '=')out(16, " ");
                    break;
                case '<':
                    ch = fgetc(fp);
                    if (ch == '=')out(11, " ");
                    else if (ch == '>') out(13, " ");
                    else {
                        fseek(fp, -1, SEEK_CUR);
                        out(10, " ");
                    }
                    break;
                case '=':
                    out(12, " ");
                    break;
                case '>':
                    ch = fgetc(fp);
                    if (ch == '=')out(15, " ");
                    else {
                        fseek(fp, -1, SEEK_CUR);
                        out(14, " ");
                    }
                    break;
                case '+':
                    return out(17, " ");
                    break;
                case '-':
                    return out(18, " ");
                    break;
                case '*':
                    return out(19, " ");
                    break;
                case '/':
                    //不识别注释
                    ch = fgetc(fp);
                    if (ch == '/')//单行注释
                    {
                        while (ch != '\n')ch = fgetc(fp);
                        lines++;
                        ch = fgetc(fp);
                        fseek(fp, -1, SEEK_CUR);
                    } else if (ch == '*')//多行注释
                    {
                        while (true) {
                            ch = fgetc(fp);
                            if (ch == '\n') {
                                lines++;
                                ch = fgetc(fp);
                            }
                            if (ch == '*') {
                                ch = fgetc(fp);
                                if (ch == '/') {
                                    ch = fgetc(fp);
                                    if (ch == '\n') {
                                        lines++;
                                        ch = fgetc(fp);
                                    }
                                    break;
                                } else fseek(fp, -1, SEEK_CUR);
                            }
                        }
                    } else {
                        fseek(fp, -1, SEEK_CUR);
                        return out(20, " ");
                    }
                    break;
                case '&':
                    ch = fgetc(fp);
                    if (ch == '&')out(21, " ");
                    else report_error("逻辑运算出错！");
                    break;
                case '|':
                    ch = fgetc(fp);
                    if (ch == '|')out(22, " ");
                    else report_error("逻辑运算出错！");
                    break;
                case '!':
                    out(23, " ");
                    break;
                case '(':
                    return out(28, " ");
                case ')':
                    return out(29, " ");
                case '\'': {
                    ch = fgetc(fp);
                    TOKEN[0] = ch;
                    if ((ch = fgetc(fp) != '\'')) {
                        report_error("字符常量出错！");
                    } else {
                        TOKEN[1] = '\0';
                    }
                    out(26, TOKEN);
                    break;
                }
                case '\"': {
                    ch = fgetc(fp);
                    i = 0;
                    while (ch != '\"') {
                        TOKEN[i] = ch;
                        i++;
                        ch = fgetc(fp);
                    }
                    TOKEN[i] = '\0';
                    out(27, TOKEN);
                    break;
                }
                default:
                    report_error("运算符号出错！");
                    break;
            }
        }
    }
    if (ch == EOF) {
        return out(30, " ");
    }
}

string myLex(FILE *fp) {
    return scanner_example(fp);
}
