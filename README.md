# EE451FinalProject

## Group Members:
- Fook Yen Edwin Chan (fookyenc@usc.edu)
- Yizhou Sheng (yizhoush@usc.edu) 
- Mengwei Yang (mengweiy@usc.edu)

## Problem Statement:
Compression of data is a basic technique in system design and communication. It aims at reducing the size of data without losing or only losing a small portion of the information stored in the data and allows (at least most) of the original data to be retrieved through a corresponding decompression. Many algorithms come into existence since the beginning of modern computer systems. One of them is the Lempel–Ziv–Welch (LZW) algorithm. Coming around in the 1970s, it soon becomes a dominant compression algorithm in Unix systems and still plays a vital role in compression today for file formats such as GIF, TIFF, and PDF. The algorithm is easy to implement and has high performance (½ of the original size for English text files), but parallelizing the algorithm is more complicated due to its nature. Throughout the years there are multiple attempts on parallelizing the algorithm using different ideas, platforms, and layouts. We will investigate two to three different approaches to parallelizing the LZW algorithm, compare their performance against the serial baseline, and analyze their performance with regards to their design as well as identify potential improvements.

## Contribution/Hypothesis
The standard serial LZW algorithm and two or three parallel LZW algorithms will be implemented. Then we will run them over several datasets and compare the performance of each algorithm against the baseline and each other. By comparing the performance of Parallel LZW algorithms, we will be able to find the most efficient version of LZW. By analyzing the techniques and ideas of these algorithms, we can all observe how different data layouts and code structures can affect the performance of a parallel algorithm on a physical machine.

## Description of Implementation

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

4. If time allows, we will compare the performance upgrade/downgrade when switching to a different platform. We will analyze how the algorithm exploits the power of their respective platforms.

### Platform
The two algorithms given above all use shared memory programming models. We will pick either OpenMP or PThreads based on the exact structure of the code. If applicable we can implement a similar/different algorithm on other platforms and compare the performance, while the majority of the work would be done with OpenMP or PThreads.


## Evaluation 
Since LSW works on any binary file, our database will contain explicitly created text files to simulate different nominal and edge cases, and will also contain large text files/picture files to verify scalability. The main focus lies in profiling the performance of algorithms, identifying the better algorithms, and provide possible explanations of program behavior and performance. The result would consist of the implementation of the algorithms and a report of profiling and analysis. 

