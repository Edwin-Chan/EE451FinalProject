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
    thread_args_t& curr_arg = *((thread_args_t*)args);
    long long old;
    long long n;
    old = codes[curr_arg.start];
    std::string s = curr_arg.table->at(old);
    std::string c;
    c += s[0];
    curr_arg.output.push_back(s);
    int count = curr_arg.code_begin;

    for (int i = curr_arg.start+1; i <= curr_arg.end; i++) {
        n = codes[i];
        if (curr_arg.table->find(n) == curr_arg.table->end()) {
            s = curr_arg.table->at(old);
            s = s + c;
        } else {
            s = curr_arg.table->at(n);
        }

        curr_arg.output.push_back(s);
        c = "";
        c += s[0];
        curr_arg.table->insert(std::make_pair(count++, curr_arg.table->at(old)+c));
        old = n;

    }

    // start new threads
    if (curr_arg.id*2 <= num_thread) {
        int next_id = curr_arg.id*2 - 1;
        thread_args[next_id].code_begin = count;
        thread_args[next_id].table = curr_arg.table;
    }

    if (curr_arg.id*2+1 <= num_thread) {
        int next_id = curr_arg.id*2;
        thread_args[next_id].code_begin = count;
        thread_args[next_id].table = new std::unordered_map<long long, std::string>(*(curr_arg.table));
        pthread_create(&threads[next_id], thread_attr, decode, (void*)&thread_args[next_id]);
    }

    if (curr_arg.id*2 <= num_thread) {
        decode((void*)&thread_args[curr_arg.id*2-1]);
    }

    if (curr_arg.id*2+1 <= num_thread) {
        pthread_join(threads[curr_arg.id*2], NULL);
        delete thread_args[curr_arg.id*2].table;
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

    struct timespec start, stop;
    double time = 0;
    
    if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
    
    for (int i = 0; i < num_thread; i++) {
        thread_args[i].id = i+1;
        // the start point is defined in the file
    }

    thread_args[0].table = new std::unordered_map<long long, std::string>;

    for (int i = 0; i <= 255; i++) {
        std::string ch;
        ch += char(i);
        thread_args[0].table->insert(std::make_pair(i, ch));
    }

    thread_args[0].code_begin = 256;

   

    thread_attr = new pthread_attr_t;
    pthread_attr_init(thread_attr);
    pthread_attr_setdetachstate(thread_attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], thread_attr, decode, (void*)&thread_args[0]);
    pthread_join(threads[0], NULL);

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}  
    time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;        
    std::cout << "Decoding time = " << time << " sec " <<std::endl; 

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