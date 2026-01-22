#pragma once

#include <vector>

template <typename T, class Compare = std::less<T>, int K = 4>
class priorityQueue : public std::vector<T> {
public:
    const T& top() const {
        return this->front();
    }
    T top() {
        return this->front();
    }
    void push(const T& value) {
        this->push_back(value);
        move_up(this->size() - 1);
    }
    void pop() {
        this->front() = this->back();
        this->pop_back();
        move_down(0);
    }
    // Replaces the top element with value:
    void push_pop(const T& value) {
        this->front() = value;
        move_down(0);
    }
    template< class... Args >
    void emplace( Args&&... args ) {
        this->emplace_back(std::forward<Args>(args)...);
        move_up(this->size() - 1);
    }
    template<typename Container>
    void push_range(const Container& c) {
        this->insert(this->end(), c.begin(), c.end());
        init();
    }
    priorityQueue() : std::vector<T>() {}
    // construct a heap from given elements in O(n):
    priorityQueue(std::initializer_list<int> values) : std::vector<T>(values) {
        init();
    }
    template<typename Container>
    priorityQueue(const Container& c) : std::vector<T>(c.begin(), c.end()) {
        init();
    }
    // Turn existing data into a heap
    void init() {
        for(int i = this->size()/K; i>=0; i--) {
            move_down(i);
        }
    }
private:
    Compare cmp;
    inline void move_up(size_t i) {
        while(i>0) {
            size_t parent = (i-1)/K;
            if( !cmp(this->data()[parent], this->data()[i]) ){
                break;
            }
            std::swap(this->data()[i], this->data()[parent]);
            i = parent;
        }
    }
    inline void move_down(size_t i) {
        size_t n = this->size();
        auto data = this->data();
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
                auto data = this->data();
                size_t a = largest_child<4>(firstChild);
                size_t b = largest_child(firstChild + 4, n);
                return cmp(data[a], data[b]) ? b : a;
        }
    }
    template<int N>
    inline size_t largest_child(size_t firstChild) {
        auto data = this->data();
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
