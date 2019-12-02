#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cmath>
#include <sys/stat.h>

#include <pthread.h>

#define PRINT 0

using std::cout;
using std::cerr;
using std::endl;

int num_thread;

pthread_attr_t* thread_attr;
pthread_t* threads;

struct thread_args_t {
    int id;
    std::string filename;
    long long start;
    long long block_size;
    std::unordered_map<std::string, long long>* table;
    long long code_begin;
    std::vector<long long> output_code;
};

thread_args_t* thread_args;

void* encode(void* args) {
    thread_args_t* curr_arg = (thread_args_t*)args;
    std::ifstream ifile(curr_arg->filename);
    ifile.seekg(curr_arg->start);

    std::string p = "", c = ""; 
    // p += s1[0];
    p += (char)ifile.get();
    int code = curr_arg->code_begin; 
    char ch;
    unsigned long long cnt = 1;
    while (!ifile.fail()) { 
        // if (i != s1.length() - 1) 
        //     c += s1[i + 1]; 
        ch = ifile.get();
        if (!ifile.eof()) {
            c += ch;
        }
        if (curr_arg->table->find(p + c) != curr_arg->table->end()) { 
            p = p + c; 
        } 
        else { 
            #if PRINT
            std::cout << p << "\t" << table[p] << "\t\t" 
                 << p + c << "\t" << code << std::endl; 
            #endif
            curr_arg->output_code.push_back(curr_arg->table->at(p)); 
            cnt++;
            curr_arg->table->insert(std::make_pair(p+c, code)); 
            code++; 
            p = c; 
        } 
        c = ""; 

        if (curr_arg->block_size > 0 && cnt > curr_arg->block_size) {
            break;
        }
    } 
    #if PRINT
    std::cout << p << "\t" << table[p] << std::endl; 
    #endif
    curr_arg->output_code.push_back(curr_arg->table->at(p)); 
    
    // start new threads
    if (curr_arg->id * 2 < num_thread) {
        int next_id = curr_arg->id * 2;
        thread_args[next_id].code_begin = code;
        thread_args[next_id].table = curr_arg->table;
        // pthread_create(&threads[next_id], thread_attr, encode, (void*)&thread_args[next_id]);
    }

    if (curr_arg->id * 2 + 1 < num_thread) {
        int next_id = curr_arg->id * 2 + 1;
        thread_args[next_id].code_begin = code;
        thread_args[next_id].table = new std::unordered_map<std::string, long long>(*(curr_arg->table));
        pthread_create(&threads[next_id], thread_attr, encode, (void*)&thread_args[next_id]);
    }

    if (curr_arg->id * 2 < num_thread) {
        // pthread_join(threads[curr_arg->id*2], NULL);
        encode((void*)&thread_args[curr_arg->id * 2]);
    }

    if (curr_arg->id * 2 + 1 < num_thread) {
        pthread_join(threads[curr_arg->id*2+1], NULL);
    }

    return NULL;
}

long long get_file_size(std::string file_name) {
    struct stat stat_buf;
    int rc = stat(file_name.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        cerr << "usage: ./lzw_parallel_encode num_thread input_file output_file" << endl;
        return 1;
    }

    num_thread = atoi(argv[1]);
    if (num_thread < 1 || num_thread > 8) {
        cerr << "Unsupported number of threads" << endl;
        return 1;
    }
    std::string input_file_name = argv[2];
    std::string output_file_name = argv[3];

    long long input_file_size = get_file_size(input_file_name);
    if (input_file_size == -1) {
        cerr << "Error reading input file" << endl;
        return 1;
    } 

    threads = new pthread_t[num_thread];
    thread_args = new thread_args_t[num_thread];
    long long block_size = input_file_size / num_thread;
    for (int i = 0; i < num_thread; i++) {
        thread_args[i].id = i+1;
        thread_args[i].filename = input_file_name;
        thread_args[i].start = i*block_size;
        thread_args[i].block_size = i == num_thread-1 ? -1 : block_size; // -1 indicates reading until the end
    }

    struct timespec start, stop;
    double time = 0;
    if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}

    thread_args[0].table = new std::unordered_map<std::string, long long>;
    for (int i = 0; i <= 255; i++) { 
        std::string ch = ""; 
        ch += char(i);
        thread_args[0].table->insert(std::make_pair(ch, i)); 
    } 

    thread_args[0].code_begin = 256;

    thread_attr = new pthread_attr_t;
    pthread_attr_init(thread_attr);
    pthread_attr_setdetachstate(thread_attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], thread_attr, encode, (void*)&thread_args[0]);
    pthread_join(threads[0], NULL);

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}       
    time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
    std::cout << "Encoding time = " << time << " sec " <<std::endl;

    // Gather output codes
    std::ofstream ofile(output_file_name);
    for (int i = 0; i < num_thread; i++) {
        for (size_t j = 0; j < thread_args[i].output_code.size(); j++) {
            ofile << thread_args[i].output_code[j] << endl;
        }
    }

    return 0;
}