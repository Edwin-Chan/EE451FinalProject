#include <unordered_map>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sys/stat.h>
#include "mpi.h"
using namespace std;

long long get_file_size(string file_name) {
    struct stat stat_buf;
    int rc = stat(file_name.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

std::vector<int> encoding(ifstream& ifile, long long len) 
{ 
    // std::cout << "Encoding\n"; 
    std::unordered_map<std::string, int> table; 
    for (int i = 0; i <= 255; i++) { 
        std::string ch = ""; 
        ch += char(i); 
        table[ch] = i; 
    } 
  
    std::string p = "", c = ""; 
    p += ifile.get(); 
    int code = 256; 
    std::vector<int> output_code; 
    // std::cout << "String\tOutput_Code\tAddition\n"; 
    unsigned long long cnt = 1;
    char ch;
    while (!ifile.fail()) { 
        ch = ifile.get();

        if (!ifile.eof()) 
            c += ch; 
        if (table.find(p + c) != table.end()) { 
            p = p + c; 
        } 
        else { 
            // std::cout << p << "\t" << table[p] << "\t\t" 
            //      << p + c << "\t" << code << std::endl; 
            output_code.push_back(table[p]); 
            cnt++;
            table[p + c] = code; 
            code++; 
            p = c; 
        } 
        c = ""; 
        if (len > 0 && cnt > len) {
            break;
        }
    } 
    // std::cout << p << "\t" << table[p] << std::endl; 
    output_code.push_back(table[p]); 
    return output_code; 
} 
  
std::vector<string> decoding(std::vector<int> op) 
{ 
    std::cout << "Decoding\n"; 
    std::unordered_map<int, std::string> table; 
    std::vector<string> output_ori ;
    for (int i = 0; i <= 255; i++) { 
        std::string ch = ""; 
        ch += char(i); 
        table[i] = ch; 
    } 
    int old = op[0], n; 
    std::string s = table[old]; 
    std::string c = ""; 
    c += s[0]; 
    //std::cout << s; 
    output_ori.push_back(s) ;
    int count = 256; 
    for (int i = 0; i < op.size() - 1; i++) { 
        n = op[i + 1]; 
        if (table.find(n) == table.end()) { 
            s = table[old]; 
            s = s + c; 
        } 
        else { 
            s = table[n]; 
        } 
        output_ori.push_back(s);
        c = ""; 
        c += s[0]; 
        table[count] = table[old] + c; 
        count++; 
        old = n; 
    }
    return output_ori ;
} 
int main(int argc, char** argv) 
{ 
    string input_file_name = "data/EnglishBible.txt";
    string output_file_name = "MPIEncoded.out";
    int len = get_file_size(input_file_name);
    if (len == -1) {
        cerr << "Cannot open input file" << endl;
        return -1;
    }

    int size, rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int signal=1;

    // std::string s = "WYS*WYGWYS*WYSWYSG"; 
    
    int l = len/2 ;

    if(rank  == 0){
        ifstream ifile(input_file_name);
        std::vector<int> output_code0 = encoding(ifile, l); 
        // std::vector<string> outp =decoding(output_code0); 
        // std::cout <<"the decodeing result is \n";
        // for(int i=0; i<outp.size();i++){
        // cout<<outp[i];
        // }
        ofstream ofile(output_file_name);
        for (unsigned long long i = 0; i < output_code0.size(); i++) {
            ofile << output_code0[i] << endl;
        }
        ofile.close();
        MPI_Send( &signal, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

    }
    if(rank  == 1){
        ifstream ifile(input_file_name);
        ifile.seekg(l);
        std::vector<int> output_code1 = encoding(ifile, -1); 
        // std::vector<string> outp =decoding(output_code1); 
        MPI_Recv( &signal, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        ofstream ofile;
        ofile.open(output_file_name, std::ios::out | std::ios::app);
        for (unsigned long long i = 0; i < output_code1.size(); i++) {
            ofile << output_code1[i] << endl;
        }
        ofile.close();
        // for(int i=0; i<outp.size();i++){
        // cout<<outp[i];
        // }
    }
    cout<<endl;
    MPI_Finalize();
} 