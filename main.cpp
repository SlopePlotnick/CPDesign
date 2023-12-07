#include <iostream>
#include <string>
#include <vector>

using namespace std;

int LA(string file);
int GA();

vector<string> filelist{"right1"};


int main () {
    for (auto file : filelist) {
        LA(file);

        GA();
    }

    return 0;
}