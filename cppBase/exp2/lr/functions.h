#include <string>

/**
 * 封装了手动实现的atof函数解析科学计数法
 * @param s 科学计数法字符串指针
 * @return 解析后的数
 */
double myatof(const char *s) {
    int sign, sign_e;//数值的符号，指数部分的符号
    int hasdot = 0;
    int hase = 0;
    double intpart = 0.0;//小数的整数部分
    double decpart = 0.0; //小数的小数部分
    int decdigit = 1; //小数的小数位数
    int exp = 0; //指数部分
    double ret;
    int i;
    //跳过开头的空格
    for (i = 0; isspace(s[i]); i++);
    //判断符号，如有，跳过
    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '-' || s[i] == '+')
        i++;
    //判断浮点数
    //第一部分：
    for (; s[i] != '\0'; i++) {
        if (isdigit(s[i])) //数字
            intpart = 10 * intpart + s[i] - '0';//计算小数的整数部分
        else if (s[i] == '.') //小数点
        {
            hasdot = 1;
            i++;
            break;
        } else if (s[i] == 'e' || s[i] == 'E') //科学计数符
        {
            hase = 1;
            i++;
            break;
        } else //非法字符
            return sign * intpart;
    }
    /*第一部分结束，有如下情况：
    1. 扫描数字知道非法字符或字符串结尾，2d 234
    2. 数字加小数点 2.
    3. 小数点 .
    4. 数字加科学计数符 3e
    5. 科学计数符 e   这种情况是非法表示，但最终计算的结果为0,
        因此可当作正常计算，不单独列出
    6. 非法字符, 直接退出
    */

    //第二部分，接着上述情况扫描
    //能进入下面循环，排除遇到字符串结尾，非法字符
    //因此只能遇到点号或科学计数符
    for (; s[i] != '\0'; i++) {
        //第一种：.3 或 3.4，均为合法，计算小数的小数部分
        if (hasdot && isdigit(s[i]))
            decpart += (s[i] - '0') / pow(10, decdigit++);
            //第二种：.e 或 2.e 或 .2e 或 3.3e 第一种非法，但计算结果为0
        else if (hasdot && (s[i] == 'e' || s[i] == 'E')) {
            hase = 1;
            i++;
            break;
        }
            //第三种：第一部分以e结束，3e e
        else if (hase)
            break;
            //第四种：第一部分以点号结束，现在扫描非数字，非科学计数符的其他非法字符
        else
            return sign * (intpart + decpart);
    }
    /*第三部分
    从第二部分退出后继续后面的程序，有如下情况：
    以科学计算符 e 结束第二部分，前面有小数点或者没有
    小数部分计算完，下面讨论指数部分
    */
    //判断指数部分符号
    sign_e = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-')
        i++;
    for (; s[i] != '\0'; i++) {
        if (isdigit(s[i]))
            exp = exp * 10 + s[i] - '0';
        else
            break;
    }
    ret = sign * ((intpart + decpart) * pow(10, sign_e * exp));
    return ret;
}

/**
 * 进一步科学计数法解析函数
 * @param token 科学计数法字符串
 * @return 解析后的字符串指针
 */
char *parseScientificNotation(const char *token) {
    double num;
    char spp[200];
    num = myatof(token);
    sprintf(spp, "%lf", num);
    int len = strlen(spp) - 1;
    while (spp[len] == '0') {
        spp[len--] = '\0';
    }
    if (spp[len] == '.') {
        spp[len] = '\0';
    }
    char *result = (char *) malloc(strlen(spp) + 1);
    strcpy(result, spp);
    return result;
}


