#include<iostream>
#include<stack>
#include<iomanip>
#include<string>
#include "fun.h"

#define E 0
#define T 1
#define F 2
using namespace std;
string sGoto;
int step = 1;
stack<int> status;/*状态栈*/
string word;
string value;
/*构造SLR(1)表，共有15个状态，需要构造ACTION表与GOTO表,然后用不同的函数处理*/
string Action[16][8] =
        {
                "s4", "0", "0", "0", "0", "0", "s5", "0",        //0
                "0", "0", "s6", "s7", "0", "0", "0", "acc",    //1
                "0", "r3", "r3", "r3", "s8", "s9", "0", "r3",    //2
                "0", "r6", "r6", "r6", "r6", "r6", "0", "r6",    //3
                "s4", "0", "0", "0", "0", "0", "s5", "0",        //4
                "0", "r8", "r8", "r8", "r8", "r8", "0", "r8",    //5
                "s4", "0", "0", "0", "0", "0", "s5", "0",        //6
                "s4", "0", "0", "0", "0", "0", "s5", "0",        //7
                "s4", "0", "0", "0", "0", "0", "s5", "0",        //8
                "s4", "0", "0", "0", "0", "0", "s5", "0",        //9
                "0", "s15", "s6", "s7", "0", "0", "0", "0",    //10
                "0", "r1", "r1", "r1", "s8", "s9", "0", "r1",    //11
                "0", "r2", "r2", "r2", "s8", "s9", "0", "r2",    //12
                "0", "r4", "r4", "r4", "r4", "r4", "0", "r4",    //13
                "0", "r5", "r5", "r5", "r5", "r5", "0", "r5",    //14
                "0", "r7", "r7", "r7", "r7", "r7", "0", "r7"    //15
        };
int Goto[16][3] =
        {
                1, 2, 3,
                0, 0, 0,
                0, 0, 0,
                0, 0, 0,
                10, 2, 3,
                0, 0, 0,
                0, 11, 3,
                0, 12, 3,
                0, 0, 13,
                0, 0, 14,
                0
        };

/**
 * 规约
 * @param num 按第num个产生式进行规约
 */
void reduction(int num) {
    switch (num) {
        case 1:// E→E+T
            status.pop();
            status.pop();
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top()][E]);
            status.push(Goto[status.top()][0]);
            break;
        case 2:// E→E-T
            status.pop();
            status.pop();
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top()][E]);
            status.push(Goto[status.top()][0]);
            break;
        case 3:// E→T
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top()][E]);
            status.push(Goto[status.top()][0]);
            break;
        case 4:// T→T*F
            status.pop();
            status.pop();
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top()][T]);
            status.push(Goto[status.top()][1]);
            break;
        case 5:// T→T/F
            status.pop();
            status.pop();
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top()][T]);
            status.push(Goto[status.top()][1]);
            break;
        case 6:// T→F
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top()][T]);
            status.push(Goto[status.top()][1]);
            break;
        case 7:// F→(E)
            status.pop();
            status.pop();
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top()][F]);
            status.push(Goto[status.top()][2]);
            break;
        case 8:// F→i
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top()][F]);
            status.push(Goto[status.top()][2]);
            break;
        default:
            break;
    }
}

/**
 * 打印栈的信息
 * @return
 */
string outStack() {
    string s = "";
    string c;
    int val;
    stack<int> temp;
    while (!status.empty()) {
        val = status.top();
        temp.push(val);
        c = to_string(val) + " ";
        s += c;
        status.pop();
    }
    //复原
    while (!temp.empty()) {
        val = temp.top();
        temp.pop();
        status.push(val);
    }
    return s;
}

/**
 * 获取当前的识别结果
 * @param word 识别的单词
 * @return
 */
int getChar(string word) {
    if (word == "LP")return 0; // (
    if (word == "RP")return 1; // )
    if (word == "PL")return 2; // +
    if (word == "MI")return 3; // -
    if (word == "MU")return 4; // *
    if (word == "DI")return 5; // /
    if (word == "ID" || word == "INT" || word == "REAL")return 6; // 标识符
    if (word == "#")return 7; // 结束
}

/**
 * 输出调试信息
 */
void out() {
    cout << std::left << setw(8) << step << std::right << setw(30) << outStack();
    cout << std::left << setw(12) << word << setw(10) << value << sGoto << endl;
    step++;
    sGoto = "";
}

/**
 * 总控程序
 * @param fp
 */
void scanner(FILE *fp) {
    int num;
    word = myLex(fp);
    while (value != "acc") {
        // 查符号分析表
        value = Action[status.top()][getChar(word)];
        out();
        if (value.substr(0, 1) == "r") {
            // 如果为r开头的则进行规约
            num = stoi(value.substr(1));
            reduction(num);
        } else if (value.substr(0, 1) == "s") {
            // 如果是s开头的进行移进，ai入栈，并且分析状态转为k
            num = stoi(value.substr(1));
            status.push(num);
            word = myLex(fp);
        } else if (value == "0") {
            cout << "语法错误：error: in (" << lines << ")lines " << word << " 处出现错误！" << endl;
            exit(0);
        }
    }
    cout << "RIGHT" << endl;
}

/**
 * 与当前目录同级的code.txt文件为本算法的测试文件,路径 fileName 要替换
 * @return
 */
int main() {

    char fileName[100] = "/Users/shj/CLionProjects/compliationBak/cppBase/exp2/lr/code.txt";
    FILE *fpInput = fopen(fileName, "r");
    status.push(0); // 状态栈入0
//     格式化输出
    cout << std::left << setw(8) << "步骤数" << setw(30) << "状态栈" << setw(12) << "待分析符号  " << setw(10) << "当前操作"
         << endl;
    scanner(fpInput);
    fclose(fpInput);

}
