#include<iostream>
#include<fstream>
#include "functions.h"

/**
 * 展示原文法的拓广文法
 */
void show_G() {
    cout << "原文法拓广为文法G[M]：" << endl;
    for (unsigned int i = 0; i < G.size(); i++) { //输出G'[S']
        cout << i << "）";
        for (unsigned int j = 0; j < G[i].size(); j++) {
            if (j == 1)cout << "->";
            cout << G[i][j];
        }
        cout << endl;
    }
    cout << endl << endl;
}
/**
 * 展示闭包
 */
void show_Closure() {  //项目集和GO函数输出到Closure.txt文件
    fstream f("Closure.txt", ios::out);
    if (!f) {
        cout << "Closure.txt文件打开出错！" << endl;
        return;
    }
    f << "该文法的项目集和GO函数：" << endl;
    cout << "该文法的项目集和GO函数：" << endl;
    for (unsigned int i = 0; i < c.size(); i++) {
        f << "I" << i << ":" << endl;
        cout << "I" << i << ":" << endl;
        for (unsigned int j = 0; j < c[i].project.size(); j++) {
            for (unsigned int k = 0; k < c[i].project[j].size(); k++) {
                if (k == 1) {
                    f << "->";
                    cout << "->";
                }
                if (c[i].project[j][k] == ' ') {
                    f << "·";
                    cout << "·";
                } else {
                    f << c[i].project[j][k];
                    cout << c[i].project[j][k];
                }
            }
            f << "，";
            cout << "，";
            for (auto it: c[i].outlook[j]) {
                if (it == *(c[i].outlook[j].begin())) {
                    f << it;
                    cout << it;
                } else {
                    f << "/" << it;
                    cout << "/" << it;
                }
            }
            f << endl;
            cout << endl;
        }
        for (auto it: c[i].go) {
            f << "GO(I" << i << "，" << it.first << ") = I" << it.second << endl;
            cout << "GO(I" << i << "，" << it.first << ") = I" << it.second << endl;
        }
        f << endl;
        cout << endl;
    }
    cout << "已将项目集和GO函数生成到Closure.txt文件中。" << endl << endl;
}

/**
 * 展示分析表
 */
void show_Table() {  //LR分析表输出到LR_Table.txt文件
    fstream f("LR_Table.txt", ios::out);
    if (!f) {
        cout << "LR_Table.txt文件打开出错！" << endl;
        return;
    }
    for (int i = -1; i < (int) c.size(); i++) {
        if (i == -1) {
            f << " " << '\t';
            cout << " " << '\t';
        } else {
            f << i << '\t';
            cout << i << '\t';
        }
        for (auto it: ts) {
            if (i == -1) {
                f << it.first << '\t';
                cout << it.first << '\t';
            } else {
                map<string, char> m;
                m[to_string(i)] = it.first;
                f << table[m] << '\t';
                cout << table[m] << '\t';
            }
        }
        if (i == -1) {
            f << "#" << '\t';
            cout << "#" << '\t';
        } else {
            map<string, char> m;
            m[to_string(i)] = '#';
            f << table[m] << '\t';
            cout << table[m] << '\t';
        }
        for (auto it: nts) {
            if (it.first == 'M') continue;
            if (i == -1) {
                f << it.first << '\t';
                cout << it.first << '\t';
            } else {
                map<string, char> m;
                m[to_string(i)] = it.first;
                f << table[m] << '\t';
                cout << table[m] << '\t';
            }
        }
        f << endl;
        cout << endl;
    }
    f.close();
    cout << "已将LR分析表生成到LR_Table.txt文件中。" << endl << endl;
    /*for (auto it1 : table) {
        for (auto it2 : it1.first) {
            cout << it2.first << it2.second << ":"<<it1.second<<endl;
        }
    }*/
}

/**
 * 处理文法输入
 */
void read_G() { //读取文法G[S]->G'[M]，并区分终结符和非终结符
    char ch; //当前读入的字符
    int i = 0; //当前行读取的第i个字符
    vector<char> v; //存放输入的一行产生式
    char X; //若遇到形如X->α|β|……,用于保存X以便消除|
    set<char> m;
    nts['M'] = m;
    while (ch = getchar()) {
        if (ch == '#') {
            break;
        }
        if (ch == '\n') { //换行
            if (!v.empty()) {
                G.push_back(v);
            }
            v.clear();
            i = 0;
            continue;
        }
        if (ch != ' ' || ch != '\t') { //去掉空格等多余字符
            if (ch == '|') { //消除元语言符号'或|'
                G.push_back(v);
                v.clear();
                i = 3;
                v.push_back(X);
                continue;
            }
            i++;
            if (i == 1) {
                X = ch;
                // 添加所有的非终结符到nts中
                nts[ch] = m; //产生式左边(第一个字符)的为非终结符
            } else if (i != 2 && i != 3 && ch != '~') {
                ts[ch] = m; //此时ts里既有非终结符又有终结符
            }
            if (i != 2 && i != 3) {
                v.push_back(ch); //去掉产生式的->
            }
        }
    }
    if (G.empty())
        exit(0);

    //不考虑S的表达式个数，直接拓广文法M
    v.clear();
    v.push_back('M');
    v.push_back(G[0][0]);
    G.insert(G.begin(), v);

    //去掉ts中的非终结符
    // 遍历非终结符
    for (unordered_map<char, set<char> >::iterator it = nts.begin(); it != nts.end(); it++) {
        unordered_map<char, set<char> >::iterator iter;
        // 如果在终结符ts中找到了就擦除
        iter = ts.find(it->first);
        if (iter != ts.end()) {
            ts.erase(iter);
        }
    }
}

/**
 * 求每个符号的first集合
 */
void get_First() { //得到First集合
    for (auto &it: ts) {
        it.second.insert(it.first);//终结符的first集合是它自己
    }
    //求非终结符的First集合
//    非终结符的First集合构造方法：
/**  连续使用下述规则，直到每个集合的First不再增大为止。
 *      若有产生式X–>a···，则把a加入到First(X)中；若X–>ε ，则把 ε 加入First(X)中。
 *      若有X–>Y···，则将First(Y)加入First(X)中，具体步骤如下：
 *          将First(Y)中所有非 ε 元素都加入First(X)中；
 *          若First(Y)中存在 ε ：
 *              若X–>Y，则将 ε 加入First(X)中；
 *              若X–>Ya···，则将 a 加入First(X)中；
 *              若X–>YZ···，则将First(Z)加入First(X)中，即重复步骤(2)；
 */
    int r = 0;
    int change = 1;
    while (change) {
        if (r == 20) {
            break; // 防止死循环
        }
        r++;
        change = 0;
        for (auto &it: nts) { //对每个非终结符
            for (unsigned int i = 0; i < G.size(); i++) { //遍历产生式
                if (G[i][0] == it.first) {
                    // 文法左边匹配到当前非终结符
                    unsigned int size = it.second.size(); //操作前First(X)的大小
                    unordered_map<char, set<char> >::iterator iter = ts.find(G[i][1]);
                    if (ts.find(G[i][1]) != ts.end() || G[i][1] == '~') { //形如X->a……或X->空字，把a加入First(X)中
                        it.second.insert(G[i][1]);
                        if (it.second.size() > size) change = 1;
                    } else {  //形如X->Y……，把First(Y)加入First(X)
                        unsigned int col = 1;
                        while (1) { //若X->Y1Y2……,循环把First(Y)加入First(X)
                            int flag = 0; //标记当前First(Y)中是否有空字
                            unordered_map<char, set<char> >::iterator itt = nts.find(G[i][col]);
                            for (auto &iter: itt->second) { //遍历First(Y)
                                if (iter == '~') flag = 1;
                                else it.second.insert(iter);
                            }
                            if (flag) {
                                col++;
                                if (G[i].size() <= col) {
                                    it.second.insert('~'); //形如X->Y,将空字加入First(X)
                                    break;
                                } else if (ts.find(G[i][col]) != ts.end()) { //形如X->Ya……，将a加入First(X)
                                    it.second.insert(G[i][col]);
                                    break;
                                } else { //形如X->YZ……，将First(Z)加入First(X)
                                }
                            } else break;
                        }
                        if (it.second.size() > size) change = 1;
                    }
                }
            }
        }
    }
}

/**
 * 求项目的闭包
 */
void get_Closure() {  //计算CLOSURE，包括GO
    /**
    1. 生成闭包 i0 的第一个项目【M–>•S，#】，其中S为输入的产生式的开始符号(即为拓广前文法G[S]的开始符号)
    2. 根据已知的项目，计算闭包CLOSURE(i) 的所有项目：
        (1) i 的任何项目都属于CLOSURE(i)。
        (2) 若项目【A–>α•Bβ，a】属于CLOSURE(i) ，且B–>γ是一个产生式，那么对于First(βa)中的每个终结符b，如果【B–>•γ，b】原来不在CLOSURE(Ｉ)中，则把它加进去。
        (3) 重复执行步骤(2)，直到 CLOSURE(i) 不再增大为止。
    根据已生成所有项目的闭包CLOSURE(i)，计算它的GO函数，并生成该GO函数对应的新项目
    (若该新项目属于新闭包，就生成新的闭包；若该新项目属于已有闭包，就将它加入该闭包里)
    从闭包CLOSURE(0)开始，对每个闭包，先计算其所有项目，再计算它的GO函数，并生成新项目(可能生成新闭包)，直到没有未处理的闭包。
     */
    int i = 0; //闭包编号
    C clo; //生成第一个闭包（I0）
    c.push_back(clo);
    while (1) {
        if (i == c.size()) {
//            对应(3) 重复执行步骤(2)，直到 CLOSURE(i) 不再增大为止。
            break; //没有新闭包，跳出循环（即已获得全部闭包及项目）
        }
        if (i == 0) { //确定项目集I0的第一个项目
//            将 G[0] 产生式拷贝到v中
            vector<char> v(G[0]);
            v.insert(v.begin() + 1, ' ');
            c[i].project.push_back(v);
            set<char> m;
            m.insert('#'); // 第一个产生式向前搜索符号为 #
            c[i].outlook.push_back(m);
        }
        for (unsigned int j = 0; j < c[i].project.size(); j++) {  //遍历已有项目，生成该闭包所有项目
            for (unsigned int k = 0; k < c[i].project[j].size(); k++) {  //扫描单个项目，找到当前位置· (这里用空格代替)
                if (c[i].project[j][k] == ' ') {
                    if (k == c[i].project[j].size() - 1) {
                        // . 位于句尾部
                        break;  //形如X->β·，不会生成新的项目
                    }
                    for (unsigned int x = 0; x < G.size(); x++) { //形如X->α·Yβ，  遍历G'[M]，查找所有对应的产生式，以求出新的项目并加入项目集
                        if (G[x][0] == c[i].project[j][k + 1]) { //对应的产生式
                            vector<char> v(G[x]); //用于保存新项目
                            v.insert(v.begin() + 1, ' '); //计算新项目
                            int exist = 0; //标记该新项目是否已存在
                            for (unsigned int y = 0; y < c[i].project.size(); y++) { //遍历已有项目，判断是新项目还是已有项目
                                if (c[i].project[y] == v) { //已有项目，只需保存项目下标（用于添加新的向前搜索符）
                                    exist = y;
                                    break;
                                }
                            }
                            if (exist == 0) {
                                c[i].project.push_back(v); //新项目，加入项目集
                            }
                            set<char> m; //用于保存新向前搜索符
                            //形如【形如X->α·Yβ，a】，计算向前搜索符，即计算First(βa)
                            //情况一：β为空字，First(βa)=a
                            //情况二：β中第一个字符为结束符b，First(βa)=b
                            //情况三：β中第一个字符为非结束符B，若First(B)中没有空字，First(βa)=First(B)；
                            //                                   若First(B)中包含空字，First(βa)=First(B)+First(γa)，其中γ为β除去第一个字符后形成的符号串
                            bool kong = true; //标记情况三B中是否有空字
                            int t = 0; //表示当前符号串γ为β除去第t个字符
                            while (kong) { //若为情况三且B含空字，计算First(γa)
                                kong = false;
                                if (k + t + 1 == c[i].project[j].size() - 1) {  //情况一 β为空字，First(βa)=a
                                    for (auto it: c[i].outlook[j]) m.insert(it);
                                } else if (ts.find(c[i].project[j][k + t + 2]) !=
                                           ts.end()) {  //情况二 β中第一个字符为结束符b，First(βa)=b
                                    m.insert(c[i].project[j][k + 2 + t]);
                                } else {  //情况三 β中第一个字符为非结束符B，若First(B)中没有空字，First(βa)=First(B)；
                                    set<char> m1((nts.find(c[i].project[j][k + 2 + t]))->second);
                                    for (auto it: m1) {
                                        if (it == '~') {  //含空字
                                            kong = true;
                                            t++;
                                        } else { //不含空字
                                            m.insert(it);
                                        }
                                    }
                                }
                            }
                            if (exist) { //已存在项目，将新向前搜索符加入原向前搜索符中
                                for (auto it: m) {
                                    c[i].outlook[exist].insert(it);
                                }
                            } else c[i].outlook.push_back(m); //新项目，新向前搜索符加入向前搜索符集
                        }
                    }
                    break;
                }
            }
        }
        for (unsigned int j = 0; j < c[i].project.size(); j++) {  //遍历本闭包的所有项目，计算GO函数，并生成新的闭包
            for (unsigned int k = 0; k < c[i].project[j].size(); k++) {  //扫描单个项目，找到当前位置·（这里用空格代替）
                if (c[i].project[j][k] == ' ') {
                    if (k == c[i].project[j].size() - 1) break; //形如【X->β·】，不会生成新闭包
                    //计算GO函数，并生成新的闭包
                    vector<char> new_closure_pro(c[i].project[j]); //用于保存新项目
                    new_closure_pro[k] = new_closure_pro[k + 1]; //计算新项目
                    new_closure_pro[k + 1] = ' ';
                    set<char> new_closure_search(c[i].outlook[j]); //用于保存新向前搜索符
                    bool dif = false; //标记 生成的新项目是否属于已有的闭包（判断是否需生成新闭包）
                    for (unsigned int x = 0; x < c.size(); x++) { //遍历已有闭包
                        //dif = false;
                        for (unsigned int y = 0;
                             y < c[x].project.size(); y++) { //将新项目和新向前搜索符与已有闭包的所有项目比较，若相同，则属于已有闭包，无需生成新闭包
                            dif = false;
                            if (new_closure_pro == c[x].project[y]) {
                                if (c[x].outlook[0].size() != new_closure_search.size()) { //比较向前搜索符长
                                    dif = true;
                                    continue;
                                }
                                auto iter = c[x].outlook[0].begin();
                                for (auto it: new_closure_search) {  //比较向前搜索符每个字符
                                    if (it != *iter) {
                                        dif = true;
                                        break;
                                    }
                                    iter++;
                                }
                                if (dif == false) { //属于已有闭包，只计算GO函数
                                    c[i].go[new_closure_pro[k]] = x;
                                    break;
                                }
                            } else dif = true;
                            if (dif == false) break;
                        }
                        if (dif == false) break;
                    }
                    if (c[i].go.count(new_closure_pro[k]) != 0 &&
                        dif) { //若有多个形如【X->α·Yβ】的产生式(相互不一样)，它们有同一GO函数(即属于同一闭包)，但生成的新项目不一样
                        c[c[i].go[new_closure_pro[k]]].project.push_back(new_closure_pro); //只需将新项目及新向前搜索符加入GO对应的闭包
                        c[c[i].go[new_closure_pro[k]]].outlook.push_back(new_closure_search);
                        break;
                    }
                    if (dif) { //不属于已有闭包，生成新闭包，并计算GO函数
                        C new_closure;
                        new_closure.project.push_back(new_closure_pro);
                        new_closure.outlook.push_back(new_closure_search);
                        c.push_back(new_closure);
                        c[i].go[new_closure_pro[k]] = c.size() - 1;
                    }
                }
            }
        }
        i++; //下一闭包
    }
}

/**
 * 获取lr1分析表
 * @return
 */
int get_Table() { //由CLOSURE计算LR(1)分析表table
    /**
        若形如【M–>S•，#】，则置ACTION[k,#]为=“acc”。
        若形如【A–>β•，a】，则置ACTION[k,a]=“rj”，表示"用产生式A–>β归约"，其中，j是产生式的编号。
        若形如【A–>α•aβ，b】，且GO(ik, a)=ij，则置ACTION[k,a]=“sj”，表示"将状态j和符号a移进栈"。
        若形如【A–>α•Bβ，b】，且GO(ik, B)=ij，则置GOTO[k,B]=“j”，表示"将(j, B)移进栈"。
        分析表中凡不能用上述规则填入信息的空白栏均为"出错"。
        按上述算法构造分析表，若出现多重定义入口的情形，则该文法不是LR(1)文法，无法构造出LR(1)分析表。函数应直接返回
     */
    for (unsigned int i = 0; i < c.size(); i++) { //遍历所有闭包
        for (unsigned int j = 0; j < c[i].project.size(); j++) { //遍历每个闭包中的所有项目
            for (unsigned int k = 0; k < c[i].project[j].size(); k++) { //扫描该项目,找到当前位置
                if (c[i].project[j][k] == ' ') {
                    // 找到了当前识别位置 .
                    if (k == c[i].project[j].size() - 1) { //形如 【X->α·，β】，归约/acc
                        // 如果当前位置位于项目的末尾, 识别成功, acc
                        if (c[i].project[j][0] == 'M') {  //形如 【M->X·，#】，令table[i,#]=acc
                            // 形如 M->S·，# 才能规约成功acc
                            map<string, char> m;
                            m[to_string(i)] = '#';
                            if (table.find(m) != table.end() && table[m] != "acc") {
                                cout << NOTLR1 << endl;
                                return 0;
                            } else {
                                table[m] = "acc";
                            }
                        } else {
                            //形如 【X->α·，a】，归约，令table[i,a]=rj
                            int id;
                            for (unsigned int x = 0; x < G.size(); x++) { //扫描G'[M]找到对应的产生式的编号
                                vector<char> v(c[i].project[j]);
                                v.pop_back();
                                if (G[x] == v) {
                                    id = x;
                                    break;
                                }
                            }
                            // lr1要求在向前搜索符号列才进行规约
                            for (auto it: c[i].outlook[j]) {
                                map<string, char> m;
                                m[to_string(i)] = it;
                                if (table.find(m) != table.end() && table[m] != (string) "r" + to_string(id)) {
                                    cout << NOTLR1 << endl;
                                    return 0;
                                } else {
                                    table[m] = (string) "r" + to_string(id);
                                }
                            }
                        }
                    } else { //形如 【X->α·β，γ】
                        // action子表
                        char next = c[i].project[j][k + 1];
                        // 证明在终结符中找到了next, 进行移入操作
                        if (ts.find(next) != ts.end()) {  //形如 【X->α·aβ，γ】,令table[i,a]=sj
                            map<string, char> m;
                            m[to_string(i)] = next;
                            if (table.find(m) != table.end() && table[m] != (string) "s" + to_string(c[i].go[next])) {
                                cout << NOTLR1 << endl;
                                return 0;
                            } else {
                                table[m] = (string) "s" + to_string(c[i].go[next]);
                            }
                        } else { //形如 【X->α·Yβ，γ】,令table[i,Y]=j
                            // GOTO子表
                            map<string, char> m;
                            m[to_string(i)] = next;
                            if (table.find(m) != table.end() && table[m] != to_string(c[i].go[next])) {
                                cout << NOTLR1 << endl;
                                return 0;
                            } else {
                                table[m] = to_string(c[i].go[next]);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
    return 1;
}

// 移进-规约
int analysis(int time) {
    //输入并分析语句sentence并输出分析过程，成功分析返回1，否则返回0
    // 处理输入
    ifstream infile("/Users/shj/CLionProjects/CompilationCPP/cmake-build-debug/out.txt");
    string lines[100];
    string line;
    string result = "#";
    int index = 0;
    // 逐行读取文件内容
    while (getline(infile, line)) {
//        cout << line << std::endl;
        size_t leftParenthesisPos = line.find('(');
        size_t commaPos = line.find(',');
        // 截取内容
        std::string extractedContent = line.substr(leftParenthesisPos + 1, commaPos - leftParenthesisPos - 1);
        cout << mapping[extractedContent] << endl;
        lines[index] = mapping[extractedContent];
        index++;
    }
    for (int i = 0; i < index; ++i) {
        result.append(lines[i]);
    }
    result.append("#");
    cout << result << endl;
    string sentence = result; //需要分析的语句
    cout << "分析的语句: " << sentence << endl;
//    cin >> sentence;
    fstream f("sentence_" + to_string(time) + ".txt", ios::out); //分析过程输出到该文件
    if (!f) {
        cout << "无法打开文件sentence.txt" << endl;
    }
    f << "步骤\t\t状态栈\t\t符号栈\t\t输入串\t\t动作说明" << endl;
    cout << "步骤\t\t状态栈\t\t符号栈\t\t输入串\t\t动作说明" << endl;
    vector<string> status; //状态栈
    vector<char> symbol; //符号栈
    int step = 1; //步骤数
    if (sentence[0] != '#' || sentence[sentence.length() - 1] != '#') {
        cout << "语句应以#开头，并以#结束" << endl;
        return 0;
    }
    symbol.push_back('#');
    sentence = sentence.substr(1);
    status.push_back("0");
    while (1) {
        f << step++ << "\t\t"; //输出步骤数
        cout << step++ << "\t\t"; //输出步骤数
        for (unsigned int i = 0; i < status.size(); i++) { //输出状态栈
            if (i == 0) {
                f << status[i];
                cout << status[i];
            } else {
                f << " " << status[i];
                cout << " " << status[i];
            }
        }
        f << "\t\t";
        cout << "\t\t";
        for (unsigned int i = 0; i < symbol.size(); i++) { //输出符号栈
            if (i == 0) {
                f << symbol[i];
                cout << symbol[i];
            } else {
                f << " " << symbol[i];
                cout << " " << symbol[i];
            }
        }
        f << "\t\t";
        cout << "\t\t";
        for (unsigned int i = 0; i < sentence.length(); i++) { //输出输入串
            if (i == 0) {
                f << sentence[i];
                cout << sentence[i];
            } else {
                f << " " << sentence[i];
                cout << " " << sentence[i];
            }
        }
        f << "\t\t";
        cout << "\t\t";
        string cur_status = status[status.size() - 1]; //当前状态
        char cur_symbol = sentence[0]; //当前'向前搜索'字符
        string new_status; //下一入栈的新状态
        map<string, char> m;
        m[cur_status] = cur_symbol;
        new_status = table[m];
        if (new_status == "acc") {
            cout << "分析成功，该语句合法！(具体分析过程请查看文件sentence_" << time << ".txt）" << endl << endl;
            f << "acc：分析成功" << endl;
            cout << "acc：分析成功" << endl;
            return 1;
        } else if (new_status[0] == 's') { //形如 table[i,b]=sj，状态sj入栈，并读入一个字符
            status.push_back(new_status.substr(1)); //入栈的状态要去掉第一个字符‘s’
            symbol.push_back(cur_symbol); //读入一个字符
            sentence = sentence.substr(1);
            f << "ACTION[" << cur_status << "," << cur_symbol << "]=" << new_status << "，即状态" << new_status << "入栈"
              << endl;
            cout << "ACTION[" << cur_status << "," << cur_symbol << "]=" << new_status << "，即状态" << new_status
                 << "入栈" << endl;
        } else if (new_status[0] == 'r') { //形如 table[i,b]=rj，用产生式G(j)归约，且table[x,y]入栈
            new_status = new_status.substr(1); //去掉‘r’
            f << "r" << new_status << "：用";
            cout << "r" << new_status << "：用";
            int gid = atoi(new_status.c_str()); //计算产生式编号
            int len = G[gid].size() - 1; //被归约的字符串长度
            char reduced_symbol = G[gid][0]; //归约得到的非终结符
            for (int i = 0; i < len; i++) {
                status.pop_back(); //归约，即去掉栈顶的len个状态项
                symbol.pop_back();
            }
            map<string, char> m;
            m[status[status.size() - 1]] = reduced_symbol;
            new_status = table[m];
            status.push_back(new_status);
            symbol.push_back(reduced_symbol);
            for (unsigned int i = 0; i < G[gid].size(); i++) {
                if (i == 1) f << "->";
                f << G[gid][i];
                cout << G[gid][i];
            }
            f << "归约，且GOTO[" << status[status.size() - 1] << "," << reduced_symbol << "]=" << new_status << "入栈"
              << endl;
            cout << "归约，且GOTO[" << status[status.size() - 1] << "," << reduced_symbol << "]=" << new_status << "入栈"
                 << endl;
        } else {
            cout << "该语句有语法错误！（详情请查看分析过程文件sentence_" << time << ".txt）" << endl << endl;
            return 0;
        }
    }
    f.close();
    cout << "该语句有语法错误！（详情请查看分析过程文件sentence_" << time << ".txt）" << endl << endl;
    return 0;
}
/**
 * 此代码需要使用实验1先进行词法分析，用得到的out.txt文件中的二元式进行语法分析
 * 使用流程如下：
 *  1. 修改此文件中analysis函数中的 infile 路径为实验一中out.txt的路径
 *  2. 重命名实验一中的main函数，例如命名为 main1, 在文件头中导入
 *  3. 在 此文件中 main 函数中找到 main1()(也就是实验一中重命名的函数), 如果改为了main1此处无需修改
 *  4. 实验一会对实验一的code.txt进行此法分析, 之后本算法会对out.txt中的二元式进行解析进行语法分析
 *  5. 在code.txt中输入要测试的代码，运行算法，显示词法分析结果，输入文法，自动构造项目集等，进行语法分析
 *  6. 产生sentence_0.txt文件 => 存放分析过程
 *  7. 产生Closure.txt文件 => 存放闭包和项目集
 *  8. 产生 LR_Table文件 => 存放分析表
 */
/** 测试文法
S->ifSthenSR
R->elseS|end
S->id=E
S->whileBdoS
E->E+T|T
T->T*F|F
F->(E)|i
B->bool|id
#
-----
#ifid=ithenwhileiddoifid=ithenid=i+ielseid=i*iend#
 */
int main() {
    cout << "词法分析结果" << endl;
    main1(); // 调用词法分析函数
    cout << "请输入文法(以单行#结束)：" << endl;
    read_G();
    cout << endl;
    show_G();

    get_First();// 求first集合
    get_Closure(); // 求闭包
    show_Closure(); // 展示闭包

    if (get_Table()) {
        show_Table();
        cout << endl;
        cout << "开始进行语法分析" << endl;
        analysis(0);
    }
}




