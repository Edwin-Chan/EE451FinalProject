# EE 451 Final Project: Parallelized LZW Algorithm

## Group Members:
- Fook Yen Edwin Chan (fookyenc@usc.edu)
- Yizhou Sheng (yizhoush@usc.edu) 
- Mengwei Yang (mengweiy@usc.edu)

## Instructions for Executing:
- The `pthread` library and standard C++ libraries are required.
- To compile the executables, simply use `make` or `make all`. To remove the executables, use `make clean`.
- Instructions for running each executable is included below.
- To run the full test suite and acquire experiment outcomes, simply run `sh run.sh`, which contains a script that autpmatically runs all three algorithms on all available test cases. Statistics would be store in the `log` directory.


## File Structure
- `lzw_serial_encode`: Performs serial LZW compression on a file. Usage: `./lzw_serial_encode input_filename output_filename`
- `lzw_standard_encode`: Performs LZW compression with standard input-division parallel technique. Currently uses the MPI interface. May change to use the PThread model. Usage: `./lzw_standard_encode input_filename output_filename` 
- `lzw_parallel_encode`: Performs LZW compression with the data layout suggested in the 2005 Klein&Wiseman paper. Usage: `./lzw_parallel_encode num_blocks input_filename output_filename`
- `lzw_serial_decode`, `lzw_standard_decode`, `lzw_parallel_decode`: Performs decoding for the corresponding compression algorithm. Usage: `./[decode_exec] input_filename output_filename`
- `data`: Folder that contains the test data. Currently contains a sample input with a single sentence, and a text version of the English Bible.
- `log`: Statistics of execution, if run with `run.sh`.
- `data-plot`: Complete statistics of the experiment in Excel format, with plots of typical runtime and compression rates.
- `source.cpp`: initial source code for LZW compression and decompression. Used as a reference.
- `source_MPI.cpp`: initial souce code for standard parallelization of LZW. Use as a reference.
- `Makefile`, `.gitignore`: control files for the repo and compilation. 
- `run.sh`: Bash script to run all test cases.
- `input_gen.cpp`: Code to generate formatted input for experiment. Not required for any algorithm and not automatically compiled. Used as a reference.
- `paper.pdf`: Our project paper on this project.

-----------
## Project Briefing
### Problem Statement:
Compression of data is a basic technique in system design and communication. It aims at reducing the size of data without losing or only losing a small portion of the information stored in the data and allows (at least most) of the original data to be retrieved through a corresponding decompression. Many algorithms come into existence since the beginning of modern computer systems. One of them is the Lempel–Ziv–Welch (LZW) algorithm. Coming around in the 1970s, it soon becomes a dominant compression algorithm in Unix systems and still plays a vital role in compression today for file formats such as GIF, TIFF, and PDF. The algorithm is easy to implement and has high performance (½ of the original size for English text files), but parallelizing the algorithm is more complicated due to its nature. Throughout the years there are multiple attempts on parallelizing the algorithm using different ideas, platforms, and layouts. We will investigate two to three different approaches to parallelizing the LZW algorithm, compare their performance against the serial baseline, and analyze their performance with regards to their design as well as identify potential improvements.

### Contribution/Hypothesis
The standard serial LZW algorithm and two or three parallel LZW algorithms will be implemented. Then we will run them over several datasets and compare the performance of each algorithm against the baseline and each other. By comparing the performance of Parallel LZW algorithms, we will be able to find the most efficient version of LZW. By analyzing the techniques and ideas of these algorithms, we can all observe how different data layouts and code structures can affect the performance of a parallel algorithm on a physical machine.

### Process
1. Firstly, a serial version of LZW will be implemented as a baseline.
2. We will then pick two to three existing Parallel LZW algorithms and implement them. Currently, two potential choices are: 
- a direct parallelization of the original serial algorithm (https://pdfs.semanticscholar.org/be7c/b6d0d3935337fd633dc9b3667bf28904c891.pdf)
- A more complicated parallelization that exploits a certain data layout to achieve higher efficiency (http://u.cs.biu.ac.il/~wiseman/dam2005.pdf).

Other algorithms exist and exploit different platforms (MPI, CUDA, etc.) After a closer look at these algorithms, we will decide if these algorithms are feasible and create a rubric to compare programs utilizing different platforms. 

### Criteria
1. We will first create a generic dataset, run the program over the dataset, and compute the average speedup of the parallel algorithms against the serial version. A decompression algorithm may also be implemented to ensure correctness.

2. We will then create a dataset of different characteristics and sizes to see how the performance is affected for each of the algorithms. We will provide possible explanations for such differences in performance, and propose/implement improvements on the algorithms. 

3. We will also observe how the performance change over the different parameters (number of threads, hardware configuration, etc.)

### Platform
The two algorithms given above all use shared memory programming models. We will pick either OpenMP or PThreads based on the exact structure of the code. 

## Evaluation 
Since LSW works on any binary file, our database will contain explicitly created text files to simulate different nominal and edge cases, and will also contain large text files/picture files to verify scalability. The main focus lies in profiling the performance of algorithms, identifying the better algorithms, and provide possible explanations of program behavior and performance. The result would consist of the implementation of the algorithms and a report of profiling and analysis. 

