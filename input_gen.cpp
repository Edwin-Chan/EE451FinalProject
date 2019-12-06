#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

int main() {
    ofstream ofile;
    ofile.open("strong_pattern.txt");
    for (int i = 0; i < 1024; i++) {
        ofile << "a";
    }
    ofile.close();

    ofile.open("weak_pattern1.txt");
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j <= i; j++) {
            ofile << char('a'+j);
        }
    }
    ofile.close();

    ofile.open("weak_pattern2.txt");
    for (int i = 0; i < 1024; i++) {
        for (char c = 'a'; c <= 'z'; c++) {
            ofile << c;
        }
    }
    ofile.close();

    mt19937 rand_gen;
    rand_gen.seed(time(0));

    ofile.open("random_text.txt");
    for (int i = 0; i < 4096; i++) {
        ofile << char('a' + rand_gen()%26);
    }
    ofile.close();
}