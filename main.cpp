#include <iostream>
#include <string>
#include <vector>

using namespace std;

int LA(string file);
int GA(string file);

string file = "../test/";

int main () {
    cout << "Please input the name of the file that you want to compile and interpret:";
    string name;
    cin >> name;
    string current_file = file + name;
    cout << "current file:" << current_file.substr(8) << endl;
    LA(current_file);

    GA(current_file);

    return 0;
}