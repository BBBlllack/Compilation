#include<fstream>
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
struct WORD {
    string value;
    string token;
};

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
    int n = 1;
    // 判断是否到达关键字表末尾, 到达代表为标识符
    // 返回值为0代表字符串相等
    while (strcmp(Keyword[n], KEY_WORD_END)) { /*strcmp 比较两串是否相同，若相同返回 0*/
        if (!strcmp(Keyword[n], token)) { /*比较 token 所指向的关键字和保留字表中哪个关键字相符*/
            return n; /*根据单词分类码表 I，设置正确的关键字类别码，并返回此类别码的值*/
        }
        n++;
    }
    return 0; /*单词不是关键字，而是标识符*/
}

/**
 * 输出二元式
 * @param a 单词的value值
 * @param token 有值单词的值
 * @return
 */
WORD out(int a, const char *token) {
    WORD word = {Value[a], token};
    return word;
}

/**
 * 扫描文件字符并拼接token进行查询
 * @param fp 文件指针
 * @return 解析的单词
 */
WORD scanner_example(FILE *fp) {
    char ch;
    int i, c;
    while ((ch = fgetc(fp)) != EOF) {
//        每次清空TOKEN
        for (int j = 0; j < TOKENLEN; ++j) {
            TOKEN[j] = '\0';
        }
        if (ch == ' ' || ch == '\t') { //不识别空格和tab
            continue;
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
            else if (c == 6) {
                return out(6, " ");
            } else {
                return out(c, " ");
            }
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
            } else if (isdigit(ch)) {
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
                case ':':
                    ch = fgetc(fp);
                    if (ch == '='){
                        out(16, " ");
                    }
                    break;
                case '<':
                    ch = fgetc(fp);
                    if (ch == '=') {
                        out(11, " ");
                    } else if (ch == '>') {
                        out(13, " ");
                    } else {
                        fseek(fp, -1, SEEK_CUR);
                        out(10, " ");
                    }
                    break;
                case '=':
                    out(12, " ");
                    break;
                case '>':
                    ch = fgetc(fp);
                    if (ch == '='){
                        out(15, " ");
                    }
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
                    break;
                case ')':
                    return out(29, " ");
                    break;
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

WORD myLex(FILE *fp) {
    WORD word = scanner_example(fp);
    return word;
}
