#include <iostream>
#include <string>
#include <vector>

using namespace std;

int LA(string file);
int GA(string file);

string file = "right11";

int main () {
//    cout << "-----------------------------------" << endl;
    cout << "current file:" << file << endl;
    LA(file);

    GA(file);

    return 0;
}