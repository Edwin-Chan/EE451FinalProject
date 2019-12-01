#include <unordered_map>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

void decoding(ifstream& ifile, ofstream& ofile) 
{ 
    std::cout << "Decoding\n"; 
    std::unordered_map<int, std::string> table; 
    for (int i = 0; i <= 255; i++) { 
        std::string ch = ""; 
        ch += char(i); 
        table[i] = ch; 
    } 
    int old, n;
    ifile >> old; 
    std::string s = table[old]; 
    std::string c = ""; 
    c += s[0]; 
    ofile << s; 
    int count = 256; 
    // for (int i = 0; i < op.size() - 1; i++) { 
    //     n = op[i + 1]; 
    //     if (table.find(n) == table.end()) { 
    //         s = table[old]; 
    //         s = s + c; 
    //     } 
    //     else { 
    //         s = table[n]; 
    //     } 
    //     std::cout << s; 
    //     c = ""; 
    //     c += s[0]; 
    //     table[count] = table[old] + c; 
    //     count++; 
    //     old = n; 
    // }
    while (ifile >> n) {
        if (table.find(n) == table.end()) { 
            s = table[old]; 
            s = s + c; 
        } 
        else { 
            s = table[n]; 
        } 
        ofile << s; 
        c = ""; 
        c += s[0]; 
        table[count] = table[old] + c; 
        count++; 
        old = n; 
    }

    std::cout << "\nEND"<<std::endl;

} 

int main(int argc, char** argv) 
{ 
    if (argc < 2 || argc > 3) {
        cerr << "usage: ./law_serial_decode input_file [output_file]" << endl;
        return 1;
    }

    string output_file = argc == 3 ? argv[2] : "decode_serial.out";

    ifstream ifile(argv[1]);
    if (ifile.fail()) {
        cerr << "Cannot open input file" << endl;
        return 1;
    }

    ofstream ofile(argv[2]);

    struct timespec start, stop;
    double time = 0;
    
    if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");} 
    decoding(ifile, ofile); 
    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}  
    time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;        
    std::cout << "Decoding time = " << time << " sec " <<std::endl; 
} 