#include <iostream>
#include <chrono>
#include <queue>
#include <algorithm>
#include <cmath>
#include <functional>

#include "../priority_queue.hpp"

unsigned int seed = 123456789;

int rand(){
    seed = 1103515245 * seed + 12345;
    return seed>>1;
}

int main(int argc, char** argv){
    int n = argc>1 ? atoi(argv[1]) : 1000000;
    std::vector<int> xs;
    for(int i=0; i<n; i++){
        xs.push_back(rand());
    }
    priority_queue<int, std::less<int>, 4, false> q;
    //std::priority_queue<int> q;
    auto start = std::chrono::high_resolution_clock::now();
    for(size_t i=0; i<n; i++){
        q.push(xs[i]);
    }
    auto end_push = std::chrono::high_resolution_clock::now();
    uint64_t t1 = std::chrono::duration_cast<std::chrono::microseconds>(end_push - start).count();
    while(q.size()>0){
        q.pop();
    }
    auto end_pop = std::chrono::high_resolution_clock::now();
    uint64_t t2 = std::chrono::duration_cast<std::chrono::microseconds>(end_pop - end_push).count();

    std::cout<<"Push took "<<t1<<"us\n";
    std::cout<<"Pop took  "<<t2<<"us\n";
}

