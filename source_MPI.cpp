
#include <unordered_map>
#include <vector>
#include <map>
#include <iostream>
#include "mpi.h"
using namespace std;
std::vector<int> encoding(std::string s1) 
{ 
    std::cout << "Encoding\n"; 
    std::unordered_map<std::string, int> table; 
    for (int i = 0; i <= 255; i++) { 
        std::string ch = ""; 
        ch += char(i); 
        table[ch] = i; 
    } 
  
    std::string p = "", c = ""; 
    p += s1[0]; 
    int code = 256; 
    std::vector<int> output_code; 
    std::cout << "String\tOutput_Code\tAddition\n"; 
    for (int i = 0; i < s1.length(); i++) { 
        if (i != s1.length() - 1) 
            c += s1[i + 1]; 
        if (table.find(p + c) != table.end()) { 
            p = p + c; 
        } 
        else { 
            std::cout << p << "\t" << table[p] << "\t\t" 
                 << p + c << "\t" << code << std::endl; 
            output_code.push_back(table[p]); 
            table[p + c] = code; 
            code++; 
            p = c; 
        } 
        c = ""; 
    } 
    std::cout << p << "\t" << table[p] << std::endl; 
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
    int size, rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int signal=1;

    std::string s = "WYS*WYGWYS*WYSWYSG"; 
    int len = s.size();
    int l = len/2 ;

    if(rank  == 0){
        std::string s0= s.substr(0,l);
        std::vector<int> output_code0 = encoding(s0); 
        std::vector<string> outp =decoding(output_code0); 
        std::cout <<"the decodeing result is \n";
        for(int i=0; i<outp.size();i++){
        cout<<outp[i];
        }
        MPI_Send( &signal, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

    }
    if(rank  == 1){
        std::string s1= s.substr(l);
        std::vector<int> output_code1 = encoding(s1); 
        std::vector<string> outp =decoding(output_code1); 
        MPI_Recv( &signal, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i=0; i<outp.size();i++){
        cout<<outp[i];
        }
    }
    cout<<endl;
    MPI_Finalize();
} 