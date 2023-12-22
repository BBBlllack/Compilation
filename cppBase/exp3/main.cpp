#include <iostream>
#include <stack>
#include <iomanip>
#include <string>
#include "MyLex.h"
#include "funs.h"

using namespace std;

/**
 * 运算拓广文法
    0. E’→E
    1. E→E+T
    2. E→E-T
    3. E→T
    4. T→T*F
    5. T→T/F
    6. T→F
    7. F→(E)
    8. F→i
 */

char *m; // 生成临时变量
TempWord tword;
WORD vol;
// 四元式缓存
string expr;
string sGoto;
int step = 1;
stack<TempWord> status; // 状态栈
string word;
string value;
string fPlace; // 第一操作数
string tPlace; // 第二操作数
string ePlace;

/**
 * 规约函数
 * @param num 按第几表达式进行规约
 */
void reduction(int num) {
    int flag = 0;
    switch (num) {
        case 1:// E→E+T
            flag = 1;
            tPlace = status.top().word.token; // 获取第二操作数
            status.pop(); // 弹出当前数
            status.pop(); // 弹出运算符
            fPlace = status.top().word.token; // 获取第一操作数
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top().num][E]);
            m = NewTemp();
            expr = "产生四元式( + ," + fPlace + ", " + tPlace + ", " + m + ")";
            ePlace = m;
            tword = {Goto[status.top().num][0], {"E", ePlace}};
            status.push(tword);
            break;
        case 2:// E→E-T
            flag = 1;
            tPlace = status.top().word.token;
            status.pop();
            status.pop();
            fPlace = status.top().word.token;
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top().num][E]);
            m = NewTemp();
//            expr = "产生四元式( - ," + ePlace + ", " + tPlace + ", " + m + ")";
            expr = "产生四元式( - ," + fPlace + ", " + tPlace + ", " + m + ")";
            ePlace = m;
            tword = {Goto[status.top().num][0], {"E", ePlace}};
            status.push(tword);
            break;
        case 3:// E→T
            ePlace = status.top().word.token;
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top().num][T]);
            tword = {Goto[status.top().num][0], {"E", ePlace}};
            status.push(tword);
            expr = "E.PLACE = " + ePlace;
            break;
        case 4:// T→T*F
            flag = 1;
            tPlace = status.top().word.token;
            status.pop();
            status.pop();
            fPlace = status.top().word.token;
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top().num][T]);
            m = NewTemp();
            expr = "产生四元式( * ," + fPlace + ", " + tPlace + ", " + m + ")";
            tPlace = m;
            tword = {Goto[status.top().num][1], {"T", tPlace}};
            status.push(tword);
            break;
        case 5:// T→T/F
            flag = 1;
            tPlace = status.top().word.token;
            status.pop();
            status.pop();
            fPlace = status.top().word.token;
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top().num][T]);
            m = NewTemp();
            expr = "产生四元式( / ," + fPlace + "," + tPlace + "," + m + ")";
            tPlace = m;
            tword = {Goto[status.top().num][1], {"T", tPlace}};
            status.push(tword);
            break;
        case 6:// T→F
            tPlace = status.top().word.token;
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top().num][T]);
            tword = {Goto[status.top().num][1], {"T", fPlace}};
            status.push(tword);
            expr = "T.PLACE = " + tPlace;
            break;
        case 7:// F→(E)
            status.pop();
            fPlace = status.top().word.token;
            status.pop();
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top().num][F]);
            tword = {Goto[status.top().num][2], {"F", fPlace}};
            status.push(tword);
            expr = "F.PLACE = " + fPlace;
            break;
        case 8:// F→i
            fPlace = status.top().word.token;
            status.pop();
            sGoto = "goto" + to_string(Goto[status.top().num][F]);
            tword = {Goto[status.top().num][2], {"F", fPlace}};
            status.push(tword);
            expr = "F.PLACE = " + fPlace;
            break;
        default:
            break;
    }
    // 将所有的四元式保存
    string temp = expr;
    if (flag) {
        quaternios.push_back(temp.replace(0, strlen("产生四元式"), ""));
    }
}

/**
 * 打印栈的信息
 * @return
 */
string outStack() {
    string s = "";
    string c;
    TempWord val;
    stack<TempWord> temp;
    while (!status.empty()) {
        val = status.top();
        temp.push(val);
        c = to_string(val.num) + " ";
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
 * 定义表存储映射, 表在funs.h中
 * @param word
 * @return
 */
int getChar(const std::string &word) {
    auto it = wordMap.find(word);
    return (it != wordMap.end()) ? it->second : -1; // 返回 -1 表示未找到
}

/**
 * 输出
 */
void out() {
    cout << std::left << setw(8) << step << std::right << setw(30) << outStack();
    cout << std::left << setw(12) << word << setw(10) << value << setw(10) << sGoto;
    step++;
    sGoto = "";
    if (value[0] != 'r') {
        cout << endl;
    }
}

/**
 * 扫描
 * @param fp 文件指针
 */
void scanner(FILE *fp) {
    int num;
    vol = myLex(fp);
    word = vol.value;
    while (value != "acc") {
        value = Action[status.top().num][getChar(word)];
        out();
        if (value.substr(0, 1) == "r") {
            num = stoi(value.substr(1));
            // 规约
            reduction(num);
            cout << std::left << setw(10) << expr << endl;
        } else if (value.substr(0, 1) == "s") {
            // 移进
            num = stoi(value.substr(1));
            tword = {num, vol};
            status.push(tword);
            vol = myLex(fp);
            word = vol.value;
        } else if (value == "0") {
            cout << "语法错误：error: in (" << lines << ")lines " << word << " 处出现错误！" << endl;
            exit(0);
        }
    }
    cout << "RIGHT!" << endl;
}

/**
 * 本算法要使用需要在main函数中找到filename，替换实际的code文件地址
 * 其余不需要做修改就可以运行
 * @return
 */
// ((3.3-1.2E3)/(2*12)+320/2) + (2.5E4 - 40/80) * 2
int main() {
    // 源程序存放位置
    char fileName[100] = "/Users/shj/CLionProjects/compliationBak/cppBase/exp3/code";
    FILE *fpInput = fopen(fileName, "r");
    TempWord start = {0, {"#", ""}}; // 初始压入 #
    status.push(start);
    cout << std::left << setw(8) << "步骤数" << setw(30) << "状态栈" << setw(12) << "待分析符号" << setw(10) << "当前操作"
         << endl;
    scanner(fpInput);
    fclose(fpInput);
    cout << "------------------------------------" << endl << "产生的四元式:" << endl;
    for (const auto &item: quaternios) {
        cout << item << endl;
    }
}