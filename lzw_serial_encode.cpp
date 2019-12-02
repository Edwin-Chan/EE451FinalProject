#include <unordered_map>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <sys/stat.h>

#define PRINT 0

using namespace std;

int compressed_file_size;

long long get_file_size(string file_name) {
    struct stat stat_buf;
    int rc = stat(file_name.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

std::vector<int> encoding(ifstream& ifile, ofstream& ofile) 
{ 
    std::cout << "Encoding\n"; 
    std::unordered_map<std::string, int> table; 
    for (int i = 0; i <= 255; i++) { 
        std::string ch = ""; 
        ch += char(i); 
        table[ch] = i; 
    } 
  
    std::string p = "", c = ""; 
    // p += s1[0];
    p += (char)ifile.get();
    int code = 256; 
    std::vector<int> output_code; 
    #if PRINT
    std::cout << "String\tOutput_Code\tAddition\n"; 
    #endif
    char ch;
    unsigned long long cnt = 0;
    while (!ifile.fail()) { 
        // if (i != s1.length() - 1) 
        //     c += s1[i + 1]; 
        ch = ifile.get();
        if (!ifile.eof()) {
            c += ch;
        }
        if (table.find(p + c) != table.end()) { 
            p = p + c; 
        } 
        else { 
            #if PRINT
            std::cout << p << "\t" << table[p] << "\t\t" 
                 << p + c << "\t" << code << std::endl; 
            #endif
            output_code.push_back(table[p]); 
            ofile << table[p] << endl;
            cnt++;
            table[p + c] = code; 
            code++; 
            p = c; 
        } 
        c = ""; 
    } 
    #if PRINT
    std::cout << p << "\t" << table[p] << std::endl; 
    #endif
    output_code.push_back(table[p]); 
    ofile << table[p] << endl;

    // Analysis
    int num_bits = (int)ceil(log2(code));
    std::cout << "Largest code assigned: " << code << endl;
    std::cout << "Number of bits to store each code: " << num_bits << endl;
    std::cout << "Number of output codes: " << cnt << endl;
    std::cout << "Estimated best-case compressed size: " << cnt / 8 * num_bits << " bytes" << endl;
    compressed_file_size = cnt / 8 * num_bits;
    return output_code; 
} 

int main(int argc, char** argv) 
{ 
    if (argc < 2 || argc > 3) {
        cerr << "Usage: ./lzw_serial_encode filename [output]" << endl;
        return 1;
    }

    string output_file = argc == 3 ? argv[2] : "output_code.txt";

    ifstream ifile(argv[1]);
    if (ifile.fail()) {
        cerr << "Cannot open file" << endl;
        return 1;
    }
    ofstream ofile(argv[2]);

    struct timespec start, stop;
    double time = 0;
    if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
    std::vector<int> output_code = encoding(ifile, ofile); 
    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}       
    time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
    std::cout << "Encoding time = " << time << " sec " <<std::endl; 
    int file_size = get_file_size(argv[1]);
    std::cout << "Input file size = " << file_size << endl;
    std::cout << "Estimated Compression Rate = " << (double)file_size / compressed_file_size << endl;
    
    #if PRINT
    std::cout << "Output Codes are: "; 
    for (size_t i = 0; i < output_code.size(); i++) { 
        std::cout << output_code[i] << " "; 
    } 
    
    std::cout << std::endl; 
    #endif
    
} 