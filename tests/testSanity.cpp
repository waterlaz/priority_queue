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
    int n = 100000;
    std::vector<int> xs;
    for(int i=0; i<n; i++){
        xs.push_back(rand());
    }
    std::vector<int> ys({100, 1000, 10000, 50000, 99999});
    std::priority_queue<int> q;
    for(auto y : ys){
        q.push(y);
    }
    priority_queue<int, std::less<int>, 2> q2(ys);
    priority_queue<int, std::less<int>, 4> q4({100, 1000, 10000, 50000, 99999});
    priority_queue<int, std::less<int>, 8> q8({100, 10000, 99999});
    q8.push_range(std::vector<int>({1000, 50000}));
    priority_queue<int, std::less<int>, 2, true> q2_h(ys);
    priority_queue<int, std::less<int>, 4, true> q4_h({100, 1000, 10000, 50000, 99999});
    priority_queue<int, std::less<int>, 8, true> q8_h({100, 10000, 99999});
    q8_h.push_range(std::vector<int>({1000, 50000}));
    int i=0;
    for(auto x : xs){
        i++;
        q.push(x);
        if(i%7){
            q2.emplace(x);
            q4.emplace(x);
            q8.emplace(x);
            q2_h.emplace(x);
            q4_h.emplace(x);
            q8_h.emplace(x);
        } else {
            q2.push(x);
            q4.push(x);
            q8.push(x);
            q2_h.push(x);
            q4_h.push(x);
            q8_h.push(x);
        }
        if(q.top() != q2.top()){
            std::cout << "Push. Mismatch between std::priority_queue and priority_queue with arity 2" << std::endl;
            return 1;
        }
        if(q.top() != q4.top()){
            std::cout << "Push. Mismatch between std::priority_queue and priority_queue with arity 4" << std::endl;
            return 1;
        }
        if(q.top() != q8.top()){
            std::cout << "Push. Mismatch between std::priority_queue and priority_queue with arity 8" << std::endl;
            return 1;
        }
        if(q.top() != q2_h.top()){
            std::cout << "Push. Mismatch between std::priority_queue and priority_queue with arity 2 (handle)" << std::endl;
            return 1;
        }
        if(q.top() != q4_h.top()){
            std::cout << "Push. Mismatch between std::priority_queue and priority_queue with arity 4 (handle)" << std::endl;
            return 1;
        }
        if(q.top() != q8_h.top()){
            std::cout << "Push. Mismatch between std::priority_queue and priority_queue with arity 8 (handle)" << std::endl;
            return 1;
        }
    }

    q.push(123456);
    q2.push(123456);
    q4.push(123456);
    q8.push(123456);

    Handle* h2 = q2_h.push(123456);
    Handle* h4 = q4_h.push(123456);
    Handle* h8 = q8_h.push(123456);
    bool popped = false;

    while(q.size()>0){
        if(!popped){
            if(q2_h[h2] != 123456){
                std::cout << "Handle access mismatch for priority_queue with arity 2" << std::endl;
                return 1;
            }
            if(q4_h[h4] != 123456){
                std::cout << "Handle access mismatch for priority_queue with arity 4" << std::endl;
                return 1;
            }
            if(q8_h[h8] != 123456){
                std::cout << "Handle access mismatch for priority_queue with arity 8" << std::endl;
                return 1;
            }
            //std::cout<<q2_h[h2]<<" "<<q4_h[h4]<<" "<<q8_h[h8]<<std::endl;
        }
        if(q.top() == 123456){
            popped = true;
        }
        if(q.top() != q2.top()){
            std::cout << "Pop. Mismatch between std::priority_queue and priority_queue with arity 2" << std::endl;
            return 1;
        }
        if(q.top() != q4.top()){
            std::cout << "Pop. Mismatch between std::priority_queue and priority_queue with arity 4" << std::endl;
            return 1;
        }
        if(q.top() != q8.top()){
            std::cout << "Pop. Mismatch between std::priority_queue and priority_queue with arity 8" << std::endl;
            return 1;
        }
        if(q.top() != q2_h.top()){
            std::cout << "Pop. Mismatch between std::priority_queue and priority_queue with arity 2 (handle)" << std::endl;
            return 1;
        }
        if(q.top() != q4_h.top()){
            std::cout << "Pop. Mismatch between std::priority_queue and priority_queue with arity 4 (handle)" << std::endl;
            return 1;
        }
        if(q.top() != q8_h.top()){
            std::cout << "Pop. Mismatch between std::priority_queue and priority_queue with arity 8 (handle)" << std::endl;
            return 1;
        }
        if(q2_h[q2_h.top_handle()] != q2_h.top()){
            std::cout << "Top handle access mismatch for priority_queue with arity 2" << std::endl;
            return 1;
        }
        if(q4_h[q4_h.top_handle()] != q4_h.top()){
            std::cout << "Top handle access mismatch for priority_queue with arity 4" << std::endl;
            return 1;
        }
        if(q8_h[q8_h.top_handle()] != q8_h.top()){
            std::cout << "Top handle access mismatch for priority_queue with arity 8" << std::endl;
            return 1;
        }
        q.pop();
        q2.pop();
        q4.pop();
        q8.pop();
        q2_h.pop();
        q4_h.pop();
        q8_h.pop();
    }
}
