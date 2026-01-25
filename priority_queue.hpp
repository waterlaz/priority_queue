/* Copyright (c) 2026 Evgeniy Vodolazskiy (waterlaz)  */

#pragma once

#include <vector>

template <typename T, class Compare = std::less<T>, int K = 4>
class priorityQueue {
public:
    const T& top() const {
        return data.front();
    }
    T& top() {
        return data.front();
    }
    void push(const T& value) {
        data.push_back(value);
        move_up(data.size() - 1);
    }
    void pop() {
        data.front() = data.back();
        data.pop_back();
        move_down(0);
    }
    // Replaces the top element with value:
    void push_pop(const T& value) {
        data.front() = value;
        move_down(0);
    }
    template< class... Args >
    void emplace( Args&&... args ) {
        data.emplace_back(std::forward<Args>(args)...);
        move_up(data.size() - 1);
    }
    template<typename Container>
    void push_range(const Container& c) {
        data.insert(data.end(), c.begin(), c.end());
        init();
    }
    size_t size() const {
        return data.size();
    }
    bool empty() const {
        return data.empty();
    }
    priorityQueue() {}
    // construct a heap from given elements in O(n):
    priorityQueue(std::initializer_list<int> values) : data(values) {
        init();
    }
    template<typename Container>
    priorityQueue(const Container& c) : data(c.begin(), c.end()) {
        init();
    }
    // Turn existing data into a heap
    void init() {
        for(int i = data.size()/K; i>=0; i--) {
            move_down(i);
        }
    }
private:
    std::vector<T> data;
    Compare cmp;
    inline void move_up(size_t i) {
        while(i>0) {
            size_t parent = (i-1)/K;
            if( !cmp(data[parent], data[i]) ){
                break;
            }
            std::swap(data[i], data[parent]);
            i = parent;
        }
    }
    inline void move_down(size_t i) {
        size_t n = data.size();
        size_t noChildren = (n+K-2)/K;
        size_t limitedChildren = (n-1)/K;
        while(i < noChildren) {
            size_t largest;
            if(i < limitedChildren) [[likely]] {
                largest = largest_child<K>(K*i+1);
            } else {
                largest = largest_child(K*i+1, n);
            }
            if(!cmp(data[i], data[largest])){
                break;
            }
            std::swap(data[i], data[largest]);
            i = largest;
        }
    }
    inline size_t largest_child(size_t firstChild, size_t n) {
        if constexpr (K == 2) {
            return firstChild;
        }
        if constexpr (K <= 4) {
            switch(n-firstChild){
                case 1:
                    return firstChild;
                case 2:
                    return largest_child<2>(firstChild);
                default:
                    return largest_child<3>(firstChild);
            }
        }
        switch(n-firstChild){
            case 1:
                return firstChild;
            case 2:
                return largest_child<2>(firstChild);
            case 3:
                return largest_child<3>(firstChild);
            case 4:
                return largest_child<4>(firstChild);
            default:
                size_t a = largest_child<4>(firstChild);
                size_t b = largest_child(firstChild + 4, n);
                return cmp(data[a], data[b]) ? b : a;
        }
    }
    template<int N>
    inline size_t largest_child(size_t firstChild) {
        if constexpr (N == 1) {
            return firstChild;
        } else if constexpr (N == 2) {
            return firstChild + !!cmp(data[firstChild], data[firstChild + 1]);
        } else {
            size_t a = largest_child< N/2 >(firstChild);
            size_t b = largest_child< N - N/2 >(firstChild + N/2);
            return cmp(data[a], data[b]) ? b : a;
        }
    }
};
