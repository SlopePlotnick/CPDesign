//
// Created by Plotnick Slope on 2023/12/5.
//

#ifndef COMPILER_GA_H
#define COMPILER_GA_H

#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;

const int N = 1e4;
string order[] = {"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "RED", "WRT"};

#define CONST 0
#define VAR 1
#define PROCEDURE 2

#define LIT 0
#define OPR 1
#define LOD 2
#define STO 3
#define CAL 4
#define INT 5
#define JMP 6
#define JPC 7
#define RED 8
#define WRT 9

class Unit {
public:
    std::string value;
    std::string key;
    int line;
    int column;

    void print();
};

/**
 * Print all properties of a unit
 */
void Unit::print() {
    std::cout << "-----------------" << std::endl;
    std::cout << "Value: " << value << std::endl;
    std::cout << "Key: " << key << std::endl;
    std::cout << "Position: [" << line << "," << column << "]" << std::endl;
    std::cout << "-----------------" << std::endl;
}

// 代码类
class code {
public:
    int f; // 指令码编号
    int l; // 层差
    int a; // 不同代码不同
};

// 符号表表项类
class symbol {
public:
    string name;
    int type; // 0: const 1: var 2: procedure
    int value;
    int level;
    int addr;
    int size;
    int num; // 变量在程序中出现的次数 便于内存优化
};

Unit unit, former_unit;
int errorStack[1000] = {-1};
int *errorType = errorStack;
bool error = false;
fstream gaSource;
fstream gaOutput;
vector<code> Pcode(1000); //指令序列
vector<symbol> SymTable(1000); //符号表
string line;
string::iterator itLine;
int tx = 0; // 符号表下标指针 符号表地址从1开始
int dx = 0; // 变量在运行栈中相对本过程基地址的偏移量
int cx = 0; // 指令下一地址
int lev = 0;
int dataStack[N] = {0}; // 数据栈
int T; // 栈顶寄存器top
int B; // 栈基址寄存器sp
int P; // 下条指令
int I; // 指令寄存器
int INT_pos; // 记录指令INT的位置

#endif //COMPILER_GA_H
