#include<map>
#include <set>
#include<string>
#include<vector>
#include<unordered_map>
#include "../../exp1/main.cpp" // 先利用实现一对其进行词法分析
#define NOTLR1 "该文法不是LR(1)文法，存在多重定义入口！"
using namespace std;
vector<vector<char> > G; //文法G[S]产生式 ，~为空字
unordered_map<char, set<char> > ts; //终结符(char)terminal symbol,及它的first集合(set<char>)
unordered_map<char, set<char> > nts; //非终结符(char)non-terminal symbol，及它的first集合(set<char>)
map<map<string, char>, string> table; //LR分析表

struct C { //闭包CLOSURE I0 I1 I2 ...
    vector<vector<char> > project; //项目集
    vector<set<char> > outlook; //向前搜索符号
    unordered_map<char, int> go; //GO函数
};
vector<C> c; // 闭包容器
map<string, string> mapping = {
        {"if",    "if"},
        {"ID",    "id"},
        {"IS",    "="},
        {"UCON",  "i"},
        {"then",  "then"},
        {"while", "while"},
        {"do",    "do"},
        {"else",  "else"},
        {"begin", "p"},
        {"end",   "end"},
        {"PL",    "+"},
        {"MI",    "-"},
        {"MU",    "*"},
        {"DI",    "/"}
}; // lex中识别结果和文法不一致，定义映射消除不一致