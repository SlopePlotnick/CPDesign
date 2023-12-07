#include "GA.h"

// 代码生成
void gen(int f, int l, int a) {
    Pcode[cx].f = f;
    Pcode[cx].l = l;
    Pcode[cx].a = a;
    cx++;
}

// 变量加入符号表
void addVar(string name, int level, int addr) {
    tx++;
    SymTable[tx].type = VAR;
    SymTable[tx].name = name;
    SymTable[tx].level = level;
    SymTable[tx].addr = addr;
    SymTable[tx].num = 1;
}

// 常数加入符号表
void addConst(string name, int level, int val) {
    tx++;
    SymTable[tx].type = CONST;
    SymTable[tx].name = name;
    SymTable[tx].level = level;
    SymTable[tx].value = val;
    SymTable[tx].num = 1;
}

// 过程加入符号表
void addProcedure(string name, int level, int addr) {
    tx++;
    SymTable[tx].type = PROCEDURE;
    SymTable[tx].name = name;
    SymTable[tx].level = level;
    SymTable[tx].addr = addr;
    SymTable[tx].num = 1;
}

//在符号表中查找某id
int position(string name) {
    for (int i = tx; i > 0; i--) {
        if (SymTable[i].name == name) {
            return i;
        }
    }
    return -1; // 表示没找到？
}

// 是否同层
bool is_same_level(string name, int lev) {
    for (int i = 1; i <= tx; i++) {
        if (SymTable[i].name == name && SymTable[i].level == lev) {
            return true;
        }
    }
    return false;
}

// 是否在外层
bool is_pre_level(string name, int lev) {
    for (int i = 1; i <= tx; i++) {
        if (SymTable[i].name == name && SymTable[i].level <= lev) {
            return true;
        }
    }
    return false;
}

// string转int函数
int s2i(string str) {
    int res = 0;
    for (int i = 0; i < str.size(); i++) {
        res *= 10;
        res += str[i] - '0';
    }
    return res;
}

// 寻找过程
int findproc(int pos) {
    for (int i = pos; i > 0; i--) {
        if (SymTable[i].type == PROCEDURE) {
            return i;
        }
    }
    return -1;
}

// 根据层差找基地址
int getBase(int nowSp, int lev) {
    int oldSp = nowSp;
    // 存在层差时 寻找非局部变量
    while (lev) {
        oldSp = dataStack[oldSp + 1]; // 寻找定义外层的活动记录首地址
        lev--;
    }
    return oldSp;
}

// 输出中间代码
void printPcode() {
    for (int i = 0; i < cx; i++) {
        cout << i << ' ';
        cout << order[Pcode[i].f] << ' ' << Pcode[i].l << ' ' << Pcode[i].a << endl;
    }
};

// 输出符号表
void printTable() {
    int i = 1;
    while (SymTable[i].num) {
        printf("名称:%s\t类型:%d\t数值:%d\t层次:%d\t相对地址:%d\t出现次数:%d\n",
               SymTable[i].name.c_str(), SymTable[i].type, SymTable[i].value, SymTable[i].level, SymTable[i].addr,
               SymTable[i].num);
        i++;
    }
}

// 解释器
void interpreter() {
    //init
    P = 0; // 下条指令位置
    B = 0; // 基址
    T = 0; // 数据栈顶位置

    do {
        I = P;
        P++;
        switch (Pcode[I].f) {
            case 0: // LIT
                dataStack[T] = Pcode[I].a;
                T++;
                break;
            case 1: // OPR
                switch (Pcode[I].a) {
                    case 0: // 返回调用点
                        T = B; // 恢复栈顶
                        P = dataStack[B + 2]; // 返回地址
                        B = dataStack[B]; // 静态链
                        break;
                    case 1: // 取反
                        dataStack[T - 1] = -dataStack[T - 1];
                        break;
                    case 2: // 相加
                        dataStack[T - 2] = dataStack[T - 1] + dataStack[T - 2];
                        T--;
                        break;
                    case 3: // 相减
                        dataStack[T - 2] = dataStack[T - 2] - dataStack[T - 1];
                        T--;
                        break;
                    case 4: // 相乘
                        dataStack[T - 2] = dataStack[T - 2] * dataStack[T - 1];
                        T--;
                        break;
                    case 5: // 相除
                        dataStack[T - 2] = dataStack[T - 2] / dataStack[T - 1];
                        T--;
                        break;
                    case 6: // 判断奇偶 奇数为1
                        dataStack[T - 1] = dataStack[T - 1] % 2;
                        break;
                    case 7: // 判断相等
                        if (dataStack[T - 1] == dataStack[T - 2]) {
                            dataStack[T - 2] = 1;
                        } else dataStack[T - 2] = 0;
                        T--;
                        break;
                    case 8: // 判断不相等
                        if (dataStack[T - 1] != dataStack[T - 2]) {
                            dataStack[T - 2] = 1;
                        } else dataStack[T - 2] = 0;
                        T--;
                        break;
                    case 9: // 判断次栈顶是否小于栈顶
                        if (dataStack[T - 2] < dataStack[T - 1]) {
                            dataStack[T - 2] = 1;
                        } else dataStack[T - 2] = 0;
                        T--;
                        break;
                    case 10: // 判断次栈顶是否不小于栈顶
                        if (dataStack[T - 2] >= dataStack[T - 1]) {
                            dataStack[T - 2] = 1;
                        } else dataStack[T - 2] = 0;
                        T--;
                        break;
                    case 11: // 判断次栈顶是否大于栈顶
                        if (dataStack[T - 2] > dataStack[T - 1]) {
                            dataStack[T - 2] = 1;
                        } else dataStack[T - 2] = 0;
                        T--;
                        break;
                    case 12: // 判断次栈顶是否不大于栈顶
                        if (dataStack[T - 2] <= dataStack[T - 1]) {
                            dataStack[T - 2] = 1;
                        } else dataStack[T - 2] = 0;
                        T--;
                        break;
                    case 13: // 输出至屏幕
                        cout << dataStack[T - 1];
                        break;
                    case 14: // 输出换行
                        cout << endl;
                        break;
                }
                break;
            case 2: // LOD
                dataStack[T] = dataStack[Pcode[I].a + getBase(B, Pcode[I].l)];
                T++;
                break;
            case 3: // STO
                if (Pcode[I].l != -1) {
                    dataStack[Pcode[I].a + getBase(B, Pcode[I].l)] = dataStack[T - 1];
                }
                // 用l==-1来表示参数传递
                else {
                    //将当前栈顶的值（参数）放到栈顶上面3个偏移量的位置 并将当前栈顶元素出栈
                    dataStack[T + 2] = dataStack[T - 1];
                }
                T--;
                break;
            case 4: // CAL
                dataStack[T] = B; // 老sp
                dataStack[T + 1] = getBase(B, Pcode[I].l); // 静态链（直接外层）
                dataStack[T + 2] = P; // 返回地址 下一条指令地址

                B = T; // 新sp
                P = Pcode[I].a; // 被调用程序入口
                break;
            case 5: // INT
                T = B + Pcode[I].a;
                break;
            case 6: // JMP
                P = Pcode[I].a;
                break;
            case 7: // JPC
                if (!dataStack[T - 1]) {
                    //条件为假时跳转
                    P = Pcode[I].a;
                }
                break;
            case 8: // RED
                cout << "Please input a number:" << endl;
                int t;
                cin >> t;
                dataStack[T] = t;
                T++;
                break;
            case 9: // WRT
                cout << dataStack[T - 1];
                break;
        }
    } while (P);
}

void ReadLine() {
    if (getline(gaSource, line)) {

        itLine = line.begin();
        // Jump annotations & empty lines
        while (*itLine == '#' || line.empty()) {
            getline(gaSource, line);
            itLine = line.begin();
        }

        former_unit = unit; //读取前保存上一unit的line和column备用

        istringstream iss(line);

        // Lexical error
        if (*itLine == '^') {
            char ch;
            iss >> ch; // '^'
            iss >> unit.value;
            iss >> unit.key;
            iss >> unit.line;
            iss >> unit.column;
            getline(gaSource, line); // jump error information
        }
            // No lexical error
        else {
            iss >> unit.value;
            iss >> unit.key;
            iss >> unit.line;
            iss >> unit.column;

//            cout << "current token:" << ' ' << unit.value << ' ' << unit.key << ' ' << unit.line << ' ' << unit.column << endl;
        }
    }
}

// 恐慌模式处理
void panic(int type) {
    // 恐慌到匹配statement
    if (type == 1) {
        while (unit.key != "ID" && unit.value != "if" && unit.value != "while" && unit.value != "call" && unit.value != "begin" && unit.value != "read" && unit.value != "write") {
            ReadLine();
        }
    }
    // 恐慌到匹配block
    else if (type == 2) {
        while (unit.value != "const" && unit.value != "var" && unit.value != "procedure" && unit.key != "ID" && unit.value != "if" && unit.value != "while" && unit.value != "call" && unit.value != "begin" && unit.value != "read" && unit.value != "write") {
            ReadLine();
        }
    }
}

void ThrowError(int type, string name = "") {
    error = true; // change the error status
    *errorType = type; // push current error into the stack
    errorType++; // move the pointer of the stack's top after pushing a new item

    switch (type)
    {
        case 0://拼写错误
            printf("[Grammar error][%d,%d] Spell error \"program\"\n", unit.line, unit.column);
            break;
        case 1://program后缺少标识符
            printf("[Grammar error][%d,%d] Missing identifier after \"program\"\n", unit.line, unit.column);
            break;
        case 2://缺少分号
            printf("[Grammar error][%d,%d] Missing \";\" \n", former_unit.line, former_unit.column);
            break;
        case 3://缺少右括号
            printf("[Grammar error][%d,%d] Missing \")\" \n", unit.line, unit.column);
            break;
        case 4://缺少左括号
            printf("[Grammar error][%d,%d] Missing \"(\" \n", unit.line, unit.column);
            break;
        case 5://缺少逗号
            printf("[Grammar error][%d,%d] Missing \",\" \n", unit.line, unit.column);
            break;
        case 6://缺少id
            printf("[Grammar error][%d,%d] Missing identifier \n", unit.line, unit.column);
            break;
        case 7://缺少比较符
            printf("[Grammar error][%d,%d] Missing compare operator \n", unit.line, unit.column);
            break;
        case 8://缺少赋值号
            printf("[Grammar error][%d,%d] Missing assignment operator \n", unit.line, unit.column);
            break;
        case 9://缺少then
            printf("[Grammar error][%d,%d] Missing \"then\" \n", unit.line, unit.column);
            break;
        case 10://缺少do
            printf("[Grammar error][%d,%d] Missing \"do\" \n", unit.line, unit.column);
            break;
        case 11://缺少begin
            printf("[Grammar error][%d,%d] Missing \"begin\" \n", unit.line, unit.column);
            break;
        case 12://缺少end
            printf("[Grammar error][%d,%d] Missing \"end\" \n", unit.line, unit.column);
            break;
        case 13://常量赋值号后面应为数字
            printf("[Grammar error][%d,%d] after\":=\" should be a number  \n", unit.line, unit.column);
            break;
        case 14://缺少:
            printf("[Grammar error][%d,%d] Missing \":\" \n", unit.line, unit.column);
            break;
        case 15://缺少:=
            printf("[Grammar error][%d,%d] Missing \":=\" \n", unit.line, unit.column);
            break;
        case 16://缺少program
            printf("[Grammar error][%d,%d] Missing \"program\" \n", unit.line, unit.column);
            break;
        case 17://存在多余的单词
            printf("[Grammar error][%d,%d] the word is superfluous \n", unit.line, unit.column);
            break;
        case 18://该变量未定义
            if(name != "")
                printf("[error][%d,%d] not exist %s\n", unit.line, unit.column, name.c_str());
            else
                printf("[error][%d,%d] not exist %s\n", unit.line, unit.column, unit.value.c_str());
            break;
        case 19://不是变量
            if(name != "")
                printf("[error][%d,%d] %s is not a variable \n", unit.line, unit.column, name.c_str());
            else
                printf("[error][%d,%d] %s is not a variable \n", unit.line, unit.column, unit.value.c_str());
            break;
        case 20://不是常量
            if(name != "")
                printf("[error][%d,%d] %s is not a const \n", unit.line, unit.column, name.c_str());
            else
                printf("[error][%d,%d] %s is not a const \n", unit.line, unit.column, unit.value.c_str());
            break;
        case 21://不是过程
            if(name != "")
                printf("[error][%d,%d] %s is not a procedure \n", unit.line, unit.column, name.c_str());
            else
                printf("[error][%d,%d] %s is not a procedure \n", unit.line, unit.column, unit.value.c_str());
            break;
        case 22://参数个数不匹配
            printf("[error][%d,%d] The number of parameters does not match \n", unit.line, unit.column);
            break;
        case 23://多重定义
            if(name != "")
                printf("[error][%d,%d] Duplicate definition %s\n", unit.line, unit.column, name.c_str());
            else
                printf("[error][%d,%d] Duplicate definition %s\n", unit.line, unit.column, unit.value.c_str());
            break;

        default:
            printf("[error][%d,%d] Unknown error\n", unit.line, unit.column);
            break;
    }
}

void Exp();

//<factor>→<id>|<integer>|(<exp>)
void Factor() {
    if (unit.key == "ID") {
        if (!is_pre_level(unit.value, lev))
            ThrowError(18);
        else {
            // 已定义
            int i = position(unit.value);
            SymTable[i].num++;

            // var类型
            if (SymTable[i].type == VAR) {
                gen(LOD, lev - SymTable[i].level, SymTable[i].addr);
            }
            // const类型
            else if (SymTable[i].type == CONST) {
                gen(LIT, 0, SymTable[i].value);
            }
            // 不是var和const类型
            else {
                ThrowError(19);
                return;
            }
        }
    }
    else if (unit.key == "INT") {
        gen(LIT, 0, s2i(unit.value));
        ReadLine();
    }
    else if (unit.key == "SOP" && unit.value == "(") {
        ReadLine();
        Exp();

        if (error) return;
        if (unit.value == ")")
            ReadLine();
        else
            ThrowError(5); // 缺少右括号
    }
    else {
        ThrowError(6); // 缺少标识符
    }
}

//<term> → <factor>{<mop><factor>}
void Term() {
    Factor();

    if (error) return;
    while (unit.value == "*" || unit.value == "/") {
        string opr = unit.value;
        ReadLine();
        Factor();

        if (error) return;
        if (opr == "*")
            gen(OPR, 0, 4);
        else
            gen(OPR, 0, 5);
    }
}


//<exp> → [+|-]<term>{<aop><term>}
void Exp() {
    string opr;
    if (unit.value == "+" || unit.value == "-") {
        opr = unit.value; // 预先记录操作符
        ReadLine();
    }
    Term();

    if (error) return;
    if (opr == "-")
        gen(OPR, 0, 1);

    while (unit.value == "+" || unit.value == "-") {
        opr = unit.value; // 预先记录操作符
        ReadLine();
        Term();

        if (error) return;
        if (opr == "+")
            gen(OPR, 0, 2);
        else if (opr == "-")
            gen(OPR, 0, 3);
    }
}

//<lexp> → <exp> <lop> <exp>|odd <exp>
void Lexp() {
    if (unit.value == "odd") {
        ReadLine();
        Exp();

        if (error) return;
        gen(OPR, 0, 6);
    }
    else {
        Exp();

        if (error) return;
        string opr = unit.value;
        if (unit.key != "COP")
        {
            ThrowError(7); // 缺少比较符号
            return;
        }
        else {
            ReadLine();
            Exp();
        }

        // 若能走到下面 opr一定是运算符
        if (opr == "=")
            gen(OPR, 0, 7);
        else if (opr == "<>")
            gen(OPR, 0, 8);
        else if (opr == "<")
            gen(OPR, 0, 9);
        else if (opr == ">=")
            gen(OPR, 0, 10);
        else if (opr == ">")
            gen(OPR, 0, 11);
        else if (opr == "<=")
            gen(OPR, 0, 12);
    }

}


void Body();

//<statement> → <id> := <exp>
//               |if <lexp> then <statement>[else <statement>]
//               |while <lexp> do <statement>
//               |call <id>（[<exp>{,<exp>}]）
//               |<body>
//               |read (<id>{，<id>})
//               |write (<exp>{,<exp>})
void Statement() {
    if (unit.key == "RESERVED" && unit.value == "if") {
        ReadLine();
        Lexp();

        if (error) return;
        int cx1;
        if (unit.key == "RESERVED" && unit.value == "then") {
            cx1 = cx; // 记录待回填代码地址
            gen(JPC, 0, 0);
            ReadLine();
        }
        else {
            ThrowError(9); // 缺少then
            return;
        }

        Statement();

        if (error) return;
        int cx2 = cx; // 记录待回填代码地址
        gen(JMP, 0, 0);

        //回填
        Pcode[cx2].a = cx;
        Pcode[cx1].a = cx;
        if (unit.value == "else") {
            ReadLine();
            Statement();

            if (error) return;
            Pcode[cx2].a = cx; // 回填
        }

    }
    else if (unit.key == "RESERVED" && unit.value == "while") {
        int cx1 = cx;
        ReadLine();
        Lexp();

        if (error) return;
        int cx2 = cx;
        gen(JPC, 0, 0);
        if (unit.key == "RESERVED" && unit.value == "do")
            ReadLine();
        else {
            ThrowError(16); // 缺少do
            return;
        }

        Statement();
        gen(JMP, 0, cx1);
        Pcode[cx2].a = cx;

    }
    else if (unit.key == "RESERVED" && unit.value == "call") {
        ReadLine();
        int i; // 标识符在符号表中的位置
        int cnt = 0; // 传递参数的个数

        // 在符号表中搜索（检查是否已经定义过）
        if (unit.key == "ID") {
            if (!is_pre_level(unit.value, lev)){
                ThrowError(18); // 未定义
                return;
            }
            else {
               i = position(unit.value);
               SymTable[i].num++;
               if (SymTable[i].type != PROCEDURE) {
                   ThrowError(21); // call后面的ID不是过程的名称
                   return;
               }
            }
            ReadLine();
        }
        else
            ThrowError(6);

        if (unit.value == "(")
            ReadLine();
        else {
            ThrowError(4); // 左括号缺失
            return;
        }

        if (unit.value == ")")
            // 无参数传递的情况 此处SymTable[i].value是要跳转的指令地址
            gen(CAL, lev - SymTable[i].level, SymTable[i].value);
        if (unit.value == "+" || unit.value == "-" || unit.key == "ID" || unit.key == "INT" || unit.value == "(") {
            Exp();

            if (error) return;
            cnt++;

            while (unit.value == ",") {
                ReadLine();
                Exp();

                if (error) return;
                cnt++;
            }
            if (cnt != SymTable[i].size)
                ThrowError(22); // 参数个数不匹配

            for (int j = 0; j < SymTable[i].size; j++) {
                // 此处对STO的参数进行解释
                // 此时的i指向的是符号表中被调用的过程的id 对于被调用的参数而言 使用值的level要在过程id所处的level上再深一层 所以用lev - (SymTable[i].level + 1)表示参数的层差
                // 对于参数的相对地址 相对基址而言 在其上面3 - 1的位置（相当于原来的栈顶+3）参数按顺序从上往下摆 第一个参数的位置还要再加上size 此后对于每一个参数 按顺序减去对应的j即可
                gen(STO, lev - (SymTable[i].level + 1), SymTable[i].size + 3 - 1 - j);
            }
            gen(CAL, lev - SymTable[i].level, SymTable[i].value); // 对过程id value中存储的是被调用程序的入口地址
        }

        if (unit.value == ")")
            ReadLine();
        else
        {
            ThrowError(3); // 右括号缺失
            return;
        }
    }
    else if (unit.key == "RESERVED" && unit.value == "read") {
        ReadLine();
        if (unit.value == "(")
            ReadLine();
        else {
            ThrowError(4); // 左括号缺失
            return;
        }

        if (unit.key == "ID") {
            if (!is_pre_level(unit.value, lev)) {
                ThrowError(18); // id未定义
                return;
            }
            else {
                int i = position(unit.value);
                SymTable[i].num++;
                if (SymTable[i].type == VAR) {
                    gen(RED, 0, 0);
                    gen(STO, lev - SymTable[i].level, SymTable[i].addr);
                }
                else {
                    ThrowError(19); // 不是变量
                    return;
                }
            }
            ReadLine();
        }
        else {
            ThrowError(6); // 缺少id
            return;
        }

        while (unit.value == ",") {
            ReadLine();

            if (unit.key != "ID") {
                ThrowError(6); // 缺少id
                return;
            }

            // 能走到这里说明key肯定是id
            if (!is_pre_level(unit.value, lev)) {
                ThrowError(18); // id未定义
                return;
            }
            else {
                int i = position(unit.value);
                SymTable[i].num++;

                if (SymTable[i].type == VAR) {
                    gen(RED, 0, 0);
                    gen(STO, lev - SymTable[i].level, SymTable[i].addr);
                }
                else {
                    ThrowError(19);
                    return;
                }
            }
            ReadLine();
        }

        if (unit.value == ")")
            ReadLine();
        else {
            ThrowError(3); // 右括号缺失
            return;
        }
    }
    else if (unit.key == "RESERVED" && unit.value == "write") {
        ReadLine();

        if (unit.value == "(")
            ReadLine();
        else {
            ThrowError(4); // 左括号缺失
            return;
        }

        Exp();

        if (error) return;
        gen(WRT, 0, 0);
        while (unit.value == ",") {
            ReadLine();
            Exp();

            if (error) return;
            gen(WRT, 0, 0);
        }

        gen(OPR, 0, 14); // 换行

        if (unit.value == ")")
            ReadLine();
        else {
            ThrowError(3); // 右括号缺失
            return;
        }

    }
    else if (unit.key == "ID") {
        string name = unit.value; // 预先存储变量名
        ReadLine();

        if (unit.key == "AOP" && unit.value == ":=")
            ReadLine();
        else {
            ThrowError(8); // 缺少赋值号
            return;
        }

        int i = position(name);
        SymTable[i].num++;

        Exp();

        if (error) return;
        if (!is_pre_level(name, lev)) {
            ThrowError(18, name); // id未定义
            return;
        }
        else {
            if (SymTable[i].type == VAR)
                gen(STO, lev - SymTable[i].level, SymTable[i].addr);
            else {
                ThrowError(19); // 不是变量
                return;
            }
        }
    }
    else {
        Body();
    }
}

//<body> → begin <statement>{;<statement>}end
void Body() {
    if (unit.key == "RESERVED" && unit.value == "begin")
        ReadLine();
    else
        ThrowError(11); // begin缺失

    panic(1); // 恐慌模式
    Statement();

    while (unit.value == ";") {
        ReadLine();

        panic(1);
        Statement();
    }

    if (unit.value == "end")
        ReadLine();
    else {
        ThrowError(12); // 缺少end
        return;
    }
}

void Block();

//<proc> → procedure <id>（[<id>{,<id>}]）;<block>{;<proc>}
// 保留字procedure在外层判断
void Proc() {
    int cnt = 0;
    int tx0;

    if (unit.key == "ID") {
        if (is_same_level(unit.value, lev)) {
            ThrowError(23); // 多重定义 过程不可多重定义
            return;
        }
        tx0 = tx + 1; // 预先记录过程在符号表中的位置 便于之后对其表项中的size（参数个数）进行回填
        addProcedure(unit.value, lev, dx); // 过程进符号表
        lev++; // 出现过程 层数+1
        ReadLine();

        if (unit.value != "(") {
            ThrowError(4); // 左括号缺失
            return;
        }
        else
            ReadLine();

        if (unit.key == "ID") {
            // 这里注意 在活动记录中最底下还有三个东西(老sp, 静态链, 返回地址) 因此变量的偏移量要加3
            addVar(unit.value, lev, cnt + 3);
            cnt++;
            SymTable[tx0].size = cnt;

            ReadLine();
            while (unit.value == "," || unit.key == "ID") {
                if (unit.value == ",")
                    ReadLine();
                else {
                    ThrowError(5); // 缺少逗号
                    return;
                }

                if (unit.key == "ID") {
                    addVar(unit.value, lev, cnt + 3);
                    cnt++;
                    SymTable[tx0].size = cnt;
                    ReadLine();
                }
                else {
                    ThrowError(6);
                    //恐慌模式
                    while (unit.value != ")" && unit.key != "ID" && unit.value != ";" && unit.value != "const" && unit.value != "var" && unit.value != "procedure")
                        ReadLine();
                    if (unit.key == "ID")
                        ReadLine();
                }
            }

            if (unit.value != ")") {
                ThrowError(3);
                return;
            }
            else
                ReadLine();
        }
        else if (unit.value == ")") {
            // 无参数传递
            ReadLine();
        }
        else {
            ThrowError(3); // 右括号缺失
            // 恐慌模式
            while(unit.value != ")" && unit.value != ";" && unit.value != "const" && unit.value != "var" && unit.value != "procedure")
                ReadLine();
        }
    }
    else {
        ThrowError(6); // 过程名id缺失
        return;
    }

    if (unit.value == ";")
        ReadLine();
    else {
        ThrowError(2); //分号缺失
        panic(2);
    }

    Block(); // 假设Block函数中如果出现proc 会在匹配proc出来后将lev恢复为进入Block时的层数 此层不需要做处理

    // 假设不会出现procedure关键字缺失的情况
    // 此时的lev为进入Block时的层数 若要进行下面的并列函数匹配 每次进入proc匹配前都要先将lev恢复
    int old_lev = lev; // 预先存储当前的层数
    while (unit.value == ";") {
        ReadLine();
        // 这里即将进行的proc匹配和当前层的proc是同层的 在进入proc匹配前应先恢复正常的层数
        lev = old_lev - 1;
        ReadLine(); // 匹配procedure关键字
        Proc(); // 从procedure的下一个开始匹配
    }

    // 结束函数前将lev恢复为预先存储的lev 注意正常情况下这里的lev相当于进入这层proc的lev + 1
    lev = old_lev;
}

//<vardecl> → var <id>{,<id>};
void Vardecl() {
    if (unit.key == "ID") {
        if (is_same_level(unit.value, lev)) {
            ThrowError(23); // 重复定义
            return;
        }

        addVar(unit.value, lev, dx);
        dx++;
        ReadLine();
    }
    else {
        ThrowError(6); // 标识符缺失
        // 恐慌模式
        while (unit.value != "," && unit.key != "ID" && unit.value != ";")
            ReadLine();
    }

    while (unit.value == "," || unit.key == "ID") {
        if (unit.key == "ID") {
            ThrowError(5); // 逗号缺失
            return;
        }
        else {
            ReadLine();

            if (unit.key == "ID") {
                if (is_same_level(unit.value, lev)) {
                    ThrowError(23); // 重复定义
                    return;
                }

                addVar(unit.value, lev, dx);
                dx++;
                ReadLine();
            }
            else {
                ThrowError(6); // 标识符缺失
                // 恐慌模式
                while (unit.value != "," && unit.key != "ID" && unit.value != ";")
                    ReadLine();
            }
        }
    }

    if (unit.value == ";")
        ReadLine();
    else
        ThrowError(2); // 分号缺失
}

//<const> → <id>:=<integer>
void Const() {
    string name;
    if (unit.key == "ID") {
        name = unit.value;
        ReadLine();
    }
    else {
        ThrowError(6); // 标识符缺失
        // 恐慌模式
        while (unit.value != ":=" && unit.value != "=" && unit.value != ";")
            ReadLine();
        if (unit.value == ";") {
            return;
        }
    }

    if (unit.value == ":=") {
        ReadLine();

        if (unit.key == "INT") {
            int value = s2i(unit.value);

            if (is_same_level(name, lev)) {
                ThrowError(23); // 多重定义
                return;
            }

            addConst(name, lev, value);
            ReadLine();
        }
        else {
            ThrowError(13); // 标识符缺失
            return;
        }
    }
    else {
        if (unit.value == "=") {
            ThrowError(14); // 缺少":"
            ReadLine();

            if (unit.key == "INT") {
                int value = s2i(unit.value);

                if (is_same_level(name, lev)) {
                    ThrowError(23); // 多重定义
                    return;
                }

                addConst(name, lev, value);
                ReadLine();
            }
            else {
                ThrowError(13); // 标识符缺失
                return;
            }
        }
        else {
            ThrowError(15); // 赋值号缺失
            return;
        }
    }
}

//<condecl> → const <const>{,<const>};
void Condecl() {
    Const();

    if (error) return;
    bool flag = false;
    while (unit.value == "," || unit.key == "ID") {
        flag = true;

        if (unit.key == "ID")
            ThrowError(5); // 逗号缺失
        else
            ReadLine();

        Const();

        if (error) return;
    }


}

//<block> → [<condecl>][<vardecl>][<proc>]<body>
void Block() {
    int dx0;
    int tx0;
    int n;
    int cx0 = cx; // 记录跳转指令的位置
    gen(JMP, 0, 0);
    n = findproc(tx); // 在当前的符号表中从后往前找procudure
    if (n != -1) {
        // 进入block之前有过程
        tx0 = tx - SymTable[n].size; // 保存旧符号表地址
        dx0 = dx; // 保存旧运行栈地址
        dx = 3 + SymTable[n].size; // 新运行栈地址
    }
    else
        // 进入block之前没有过程
        dx = 3;

    if (unit.value == "const" && unit.key == "RESERVED") {
        ReadLine();
        Condecl();

        if (error) return;
    }

    if (unit.value == "var" && unit.key == "RESERVED") {
        ReadLine();
        Vardecl();

        if (error) return;
    }

    if (unit.value == "procedure" && unit.key == "RESERVED") {
        ReadLine();
        Proc();

        if (error) return;
        // 注意这里从proc出来的时候 lev相当于进入block时的lev + 1 所以要减1复原
        lev--;
    }

    INT_pos = cx;
    gen(INT, 0, dx);
    Pcode[cx0].a = cx - 1; // 回填 让JMP指令跳到INT指令的位置
    if (n != -1) {
        SymTable[n].value = cx - 1; // 调用子过程的入口地址
    }

    Body();
    gen(OPR, 0, 0); // 返回调用点并出栈
    dx = dx0; //恢复老sp
}

//<prog> → program <id>；<block>
void Prog() {
    ReadLine();
    if ((unit.value == "program" && unit.key == "RESERVED") || *errorType == 0) {
        if (*errorType == 0)
            errorType++;
        ReadLine();
        if (unit.key == "ID" || *errorType == 1) {
            if (*errorType == 1)
                errorType++;
            else
                ReadLine();
            if ((unit.value == ";" && unit.key == "EOP") || *errorType == 2) {
                if (*errorType == 2 && unit.value != ";")
                    errorType++;
                else
                    ReadLine();
                Block();

                if (error) return;
            } else {
                ThrowError(2);
            }
        } else {
            ThrowError(1);
        }
    } else {
        ThrowError(0);
    }

}

/**
 * Open IO files.
 */
void OpenFile() {
    gaSource.open("la_output", ios::in); // Read file
    gaOutput.open("ga_output", ios::out | ios::trunc); // Write file

    if (!gaSource.is_open()) {
        cout << "Cannot open the gaSource file!\a" << endl;
        exit(1);
    }
    if (!gaOutput.is_open()) {
        cout << "Cannot open the gaOutput file!\a" << endl;
    } else {
        // Header of the file (DateTime & File name & Lang set)

        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        gaOutput << "# Grammar Analysis Result" << endl;
        gaOutput << "# Generated Time: " << asctime(timeinfo);
        gaOutput << "# Language Set: PL/0" << endl;
        gaOutput << endl;
    }
}

/**
 * Close IO files.
 */
void CloseFile() {
    gaSource.close();
    gaOutput.close();
}

/**
 * Print the error stack out.
 */
void PrintErrorStack() {
    cout << "[DEBUG] Error Stack" << endl;
    cout << endl << "|<<<< ERROR STACK <<<<<" << endl;
    int *p;
    cout << "|";
    for (p = errorStack; p != errorType; p++) {
        cout << *p << " ";
    }
    cout << endl << "|<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

int GA() {

    OpenFile();
    Prog();
    CloseFile();
    // when there's an error, the program will end and when will start from begin again.
    while (error) {
        error = false;
        errorType = errorStack;
        OpenFile();
        Prog();
        CloseFile();
    }

//    PrintErrorStack();
    cout << "Grammatical Analysis done!" << endl;
    return 0;
}
