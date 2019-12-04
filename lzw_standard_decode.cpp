#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>
#include <pthread.h>
#define DEBUG 1

using std::endl;
using std::cerr;
using std::cout;

int num_thread;

pthread_attr_t* thread_attr;
pthread_t* threads;

std::vector<long long> codes;

struct thread_args_t {
    int id;
    long long start;
    long long end;
    std::unordered_map<long long, std::string>* table;
    long long code_begin;
    std::vector<std::string> output; 
};

thread_args_t* thread_args;

long long get_file_size(std::string file_name) {
    struct stat stat_buf;
    int rc = stat(file_name.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}


void* decode(void* args) 
{ 

    std::unordered_map<int, std::string> table; 
    for (int i = 0; i <= 255; ++i) { 
        std::string ch = ""; 
        ch += char(i); 
        table[i] = ch; 
    } 

	thread_args_t& curr_arg = *((thread_args_t*)args);

	long long old;
    long long n;
    old = codes[curr_arg.start];
    std::string s = table[old];
    std::string c;
    c += s[0];
    curr_arg.output.push_back(s);
    int count = 256;


    for (int i = curr_arg.start+1; i <= curr_arg.end; ++i) {
        n = codes[i];
        if (table.find(n) == table.end()) {
            s = table[old];
            s = s + c;
        } else {
            s = table[n];
        }

        curr_arg.output.push_back(s);
        c = "";
        c += s[0];
        table[count] = table[old] + c; 
        count++; 
        old = n;

    }

    return NULL;
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        cerr << "usage: ./lzw_parallel_decode encoded_file extracted_file" << endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    long long input_file_size = get_file_size(input_file);
    if (input_file_size < 0) {
        cerr << "Error: cannot read input file or input file is corrupted" << endl;
        return 1;
    }

    std::ifstream ifile(input_file);
    long long buf;
    ifile >> buf;
    if (ifile.fail() || buf != -2) {
        cerr << "Error: input file is corrupted" << endl;
        ifile.close();
        return 1;
    }

    ifile >> num_thread;

    threads = new pthread_t[num_thread];
    thread_args = new thread_args_t[num_thread];

    int curr_block = 0;
    int code_cnt = 0;
    int block_size = -1;

    while (ifile >> buf) {
        if (buf == -1) { // new block
            thread_args[curr_block].start = code_cnt;
            ifile >> block_size; // The number of elements to come
            for (int i = 0; i < block_size; i++) {
                ifile >> buf;
                codes.push_back(buf);
                code_cnt++;
            }
            thread_args[curr_block].end = code_cnt-1;
            curr_block++;
        } else { // corruption
            cerr << "Warning: corruption" << endl;
            return 2;
        }
        
    }

    for (int i = 0; i < num_thread; i++) {
        thread_args[i].id = i+1;
        // the start point is defined in the file
    }

    thread_attr = new pthread_attr_t;
    pthread_attr_init(thread_attr);
    pthread_attr_setdetachstate(thread_attr, PTHREAD_CREATE_JOINABLE);


    for(int i = 0; i < num_thread; ++i)
        {
            pthread_create(&threads[i], thread_attr, decode, (void*)&thread_args[i]);
        }

    for(int i = 0; i < num_thread; ++i)
        {
            pthread_join(threads[i], NULL);
        }

    std::ofstream ofile(output_file);
    for (int i = 0; i < num_thread; i++) {
        for (size_t j = 0; j < thread_args[i].output.size(); j++) {
            ofile << thread_args[i].output[j];
        }
    }

    delete thread_args[0].table;
    delete [] threads;
    delete [] thread_args;
    delete thread_attr;

    return 0;
}