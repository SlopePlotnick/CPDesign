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
    // 找到statement的first集中的元素
    if (type == 1) {
        while (unit.key != "ID" && unit.value != "if" && unit.value != "while" && unit.value != "call" && unit.value != "begin" && unit.value != "read" && unit.value != "write") {
            ReadLine();
        }
    }
    // 找到block的first集中的元素
    else {
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

    }
    else if (unit.key == "RESERVED" && unit.value == "read") {
        ReadLine();
        if ((unit.key == "SOP" && unit.value == "(") || *errorType == 20) {
            if (*errorType == 20 && unit.value != "(")
                errorType++;
            else
                ReadLine();
            if (unit.key == "ID" || *errorType == 19) {
                if (*errorType == 19 && unit.key != "ID")
                    errorType++;
                else
                    ReadLine();
                while (unit.key == "SOP" && unit.value == ",") {
                    ReadLine();
                    if (unit.key == "ID" || *errorType == 19) {
                        if (*errorType == 19 && unit.key != "ID")
                            errorType++;
                        else
                            ReadLine();
                    } else {
                        ThrowError(19);
                        return;
                    }
                }
                if (unit.key == "SOP" && unit.value == ")" || *errorType == 20) {
                    if (*errorType == 20 && unit.value != ")")
                        errorType++;
                    else
                        ReadLine();
                } else {
                    ThrowError(20);
                    return;
                }
            } else {
                ThrowError(19);
                return;
            }

        } else {
            ThrowError(20);
            return;
        }

    } else if (unit.key == "RESERVED" && unit.value == "write") {
        ReadLine();
        if (unit.key == "SOP" && unit.value == "(" || *errorType == 21) {
            if (*errorType == 21 && unit.value != "(")
                errorType++;
            else
                ReadLine();
            Exp();

            if (error) return;
            while (unit.key == "SOP" && unit.value == ",") {
                ReadLine();
                Exp();

                if (error) return;
            }
            if (unit.key == "SOP" && unit.value == ")" || *errorType == 21) {
                if (*errorType == 21 && unit.value != ")")
                    errorType++;
                else
                    ReadLine();
            } else {
                ThrowError(21);
                return;
            }
        } else {
            ThrowError(21);
            return;
        }

    } else if (unit.key == "ID") {
        ReadLine();
        if (unit.key == "AOP" && unit.value == ":=" || *errorType == 4) {
            if (*errorType == 4 && unit.value != ":=")
                errorType++;
            else
                ReadLine();
            Exp();

            if (error) return;
        } else {
            ThrowError(4);
            return;
        }
    } else {
        Body();
    }
}

//<body> → begin <statement>{;<statement>}end
void Body() {
    if (unit.key == "RESERVED" && unit.value == "begin" || *errorType == 12) {
        if (*errorType == 12 && unit.value != "begin")
            errorType++;
        else
            ReadLine();
        Statement();

        if (error) return;
        while (unit.key == "EOP" && unit.value == ";") {
            ReadLine();
            if (unit.value == "end") {
                if (*errorType != 25)
                    ThrowError(25);
                break;
            }
            Statement();

            if (error) return;
        }
        if (*errorType == 25) {
            errorType++;
        }
        if (unit.key == "RESERVED" && unit.value == "end" || *errorType == 13) {
            if (*errorType == 13 && unit.value != "end")
                errorType++;
            else
                ReadLine();
        } else {
            ThrowError(13);
        }
    } else {
        ThrowError(12);
    }

}

void Block();

//<proc> → procedure <id>（[<id>{,<id>}]）;<block>{;<proc>}
void Proc() {
    if (unit.key == "ID" || *errorType == 9) {
        if (*errorType == 9 && unit.key != "ID")
            errorType++;
        else
            ReadLine();
        if (unit.key == "SOP" && unit.value == "(" || *errorType == 10) {
            if (*errorType == 10 && unit.value != "(")
                errorType++;
            else
                ReadLine();
            if (unit.value != ")" && unit.value != ";") {
                if (unit.key == "ID" || *errorType == 11) {
                    if (*errorType == 11 && unit.key != "ID")
                        errorType++;
                    ReadLine();

                    while (unit.value == "," && unit.key == "SOP") {
                        ReadLine();
                        if (unit.key == "ID" || *errorType == 11) {
                            if (unit.key != "ID" && *errorType == 11)
                                errorType++;
                            else
                                ReadLine();
                        } else {
                            ThrowError(11); // Missing id
                        }
                    }
                } else {
                    ThrowError(11);
                    return;
                }
            }
            if (unit.key == "SOP" && unit.value == ")" || *errorType == 10) {
                if (*errorType == 10 && unit.value != ")")
                    errorType++;
                else
                    ReadLine();

                if (unit.key == "EOP" && unit.value == ";" || *errorType == 2) {
                    if (*errorType == 2 && unit.value != ";")
                        errorType++;
                    else
                        ReadLine();
                    Block();

                    if (error) return;

                    while (unit.key == "EOP" && unit.value == ";") {
                        ReadLine();
                        Proc();

                        if (error) return;
                    }
                } else {
                    ThrowError(2);
                }
            } else {
                ThrowError(10);
            }
        } else {
            ThrowError(10);
        }
    } else {
        ThrowError(9);
    }
}

//<vardecl> → var <id>{,<id>};
void Vardecl() {
    if (unit.key == "ID" || *errorType == 6) {
        if (unit.key != "ID" && *errorType == 6)
            errorType++;
        else
            ReadLine();
        while (unit.value == "," && unit.key == "SOP" || *errorType == 26) {
            if (*errorType == 26 && unit.value != ",")
                errorType++;
            else
                ReadLine();
            if (unit.key == "ID" || *errorType == 7) {
                if (unit.key != "ID" && *errorType == 7)
                    errorType++;
                else
                    ReadLine();
            } else {
                ThrowError(7);// Missing ID
            }
        }
        if (unit.key == "ID") {
            ThrowError(26);
            return;
        }
        if (unit.value == ";" && unit.key == "EOP" || *errorType == 2) {
            if (*errorType == 2 && unit.value != ";")
                errorType++;
            else
                ReadLine();
        } else {
            ThrowError(2); // Missing EOP
        }
    } else {
        ThrowError(6); // Missing ID
    }
}

//<const> → <id>:=<integer>
void Const() {
    if (unit.key == "ID" || *errorType == 3) {
        if (unit.key != "ID" && *errorType == 3)
            errorType++;
        else
            ReadLine();

        if ((unit.key == "AOP" && unit.value == ":=") || *errorType == 4) {
            if (!(unit.key == "AOP" && unit.value == ":=") && *errorType == 4)
                errorType++;
            else
                ReadLine();

            if (unit.key == "INT" || *errorType == 5) {
                if (unit.key != "INT" && *errorType == 5)
                    errorType++;
                else
                    ReadLine();
            } else {
                ThrowError(5);
            }
        } else {
            ThrowError(4);
        }
    } else {
        ThrowError(3);
    }
}

//<condecl> → const <const>{,<const>};
void Condecl() {
    Const();

    if (error) return;
    while ((unit.key == "SOP" && unit.value == ",")) {
        ReadLine();
        Const();

        if (error) return;
    }

    if ((unit.key == "EOP" && unit.value == ";") || *errorType == 2) {
        if (!(unit.key == "EOP" && unit.value == ";") && *errorType == 2)
            errorType++;
        else
            ReadLine();
    } else {
        ThrowError(2);
    }
}

//<block> → [<condecl>][<vardecl>][<proc>]<body>
void Block() {
    if (unit.value == "const" && unit.key == "RESERVED" && !error) {
        ReadLine();
        Condecl();

        if (error) return;
    } else if (unit.key == "ID" && *errorType != 8 && !error) {
        ThrowError(8); // Cannot resolve type
//        scary model
        while (unit.key != "EOP" && unit.key != "RESERVED") {
            ReadLine();
        }
        ReadLine();
    } else if (*errorType == 8 && !error) {
        errorType++;
        while (unit.key != "EOP" && unit.key != "RESERVED") {
            ReadLine();
        }
        ReadLine();
    }

    if (unit.value == "var" && unit.key == "RESERVED" && !error) {
        ReadLine();
        Vardecl();

        if (error) return;
    } else if (unit.key == "ID" && *errorType != 8 && !error) {
        ThrowError(8); // Cannot resolve type
        while (unit.key != "EOP" && unit.key != "RESERVED") {
            ReadLine();
        }
        ReadLine();
    } else if (*errorType == 8 && !error) {
        errorType++;
        while (unit.key != "EOP" && unit.key != "RESERVED") {
            ReadLine();
        }
        ReadLine();
    }

    if (unit.value == "procedure" && unit.key == "RESERVED" && !error) {
        ReadLine();
        Proc();

        if (error) return;
    } else if (unit.key == "ID" && *errorType != 8 && !error) {
        ThrowError(8); // Cannot resolve type
        while (unit.key != "EOP" && unit.key != "RESERVED") {
            ReadLine();
        }
        ReadLine();
    } else if (*errorType == 8 && !error) {
        errorType++;
        while (unit.key != "EOP" && unit.key != "RESERVED") {
            ReadLine();
        }
        ReadLine();
    }

    if (!error) {
        Body();

        if (error) return;
    }
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
