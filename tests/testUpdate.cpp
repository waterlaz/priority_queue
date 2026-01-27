#include <iostream>
#include <chrono>
#include <queue>
#include <algorithm>
#include <cmath>
#include <functional>

#include "../priority_queue.hpp"

int main(){
    std::vector<int> data = {5, 1, 3, 7, 2, 8, 9, 6};
    priority_queue<int, std::less<int>, 4, true> pq(data);
    pq.push(10);
    auto h4 = pq.push(4);
    auto h0 = pq.push(0);
    bool hasDropped = false;
    while(!pq.empty()){
        std::cout << pq.top() << " ";
        if(!hasDropped){
            if(pq[h4] != 4){
                std::cout << "\nError: 4 was dropped from the queue!" << std::endl;
                return 1;
            }
        }
        if(pq.top() == 4){
            pq.update(h0, 2);
            hasDropped = true;
        }
        pq.pop();
    }
    std::cout << std::endl;
}

