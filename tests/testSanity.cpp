#include <iostream>
#include <chrono>
#include <queue>

#include "../priority_queue.hpp"

unsigned int seed = 123456789;

int rand(){
    seed = 1103515245 * seed + 12345;
    return seed>>1;
}

int main(){
    std::vector<int> xs;
    for(int i=0; i<10000000; i++){
        xs.push_back(rand());
    }
    std::priority_queue<int> q;
    priorityQueue<int, std::less<int>, 2> q2;
    priorityQueue<int, std::less<int>, 4> q4;
    priorityQueue<int, std::less<int>, 8> q8;
    for(auto x : xs){
        q.push(x);
        q2.push(x);
        q4.push(x);
        q8.push(x);
        if(q.top() != q2.top()){
            std::cout << "Push. Mismatch between std::priority_queue and priorityQueue with arity 2" << std::endl;
            return 1;
        }
        if(q.top() != q4.top()){
            std::cout << "Push. Mismatch between std::priority_queue and priorityQueue with arity 4" << std::endl;
            return 1;
        }
        if(q.top() != q8.top()){
            std::cout << "Push. Mismatch between std::priority_queue and priorityQueue with arity 8" << std::endl;
            return 1;
        }
    }

    while(q.size()>0){
        if(q.top() != q2.top()){
            std::cout << "Pop. Mismatch between std::priority_queue and priorityQueue with arity 2" << std::endl;
            return 1;
        }
        if(q.top() != q4.top()){
            std::cout << "Pop. Mismatch between std::priority_queue and priorityQueue with arity 4" << std::endl;
            return 1;
        }
        if(q.top() != q8.top()){
            std::cout << "Pop. Mismatch between std::priority_queue and priorityQueue with arity 8" << std::endl;
            return 1;
        }
        q.pop();
        q2.pop();
        q4.pop();
        q8.pop();
    }
}
