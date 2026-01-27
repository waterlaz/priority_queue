/* Copyright (c) 2026 Evgeniy Vodolazskiy (waterlaz)  */

#pragma once

#include <vector>

struct Handle {
    size_t index;
};

template <typename T, bool useHandles>
class HeapNode;

template <typename T>
class HeapNode<T, false> {
public:
    T value;
    HeapNode& operator=(const T& other) {
        value = other;
        return *this;
    }
    HeapNode(const T& val) : value(val) {}
    HeapNode() = default;
};

template <typename T>
class HeapNode<T, true> {
public:
    T value;
    Handle* handle;
    HeapNode& operator=(const T& other) {
        value = other;
        return *this;
    }
    HeapNode(const T& val) : value(val) {
        handle = new Handle();
    }
    HeapNode() {
        handle = new Handle();
    }
};


template <typename T, class Compare = std::less<T>, int K = 4, bool useHandles = false>
class priority_queue {
public:
    const T& top() const {
        return data.front().value;
    }
    T& top() {
        return data.front().value;
    }
    T& operator[](Handle* h) {
        return data[h->index].value;
    }
    Handle* top_handle() {
        if constexpr (useHandles) {
            return data.front().handle;
        } else {
            return nullptr;
        }
    }
    template<bool B = useHandles>
    std::enable_if_t<B, Handle*>
    push(const T& value) {
        data.push_back(value);
        Handle* h = data.back().handle;
        h->index = data.size() - 1;
        move_up(data.size() - 1);
        return h;
    }
    template<bool B = useHandles>
    std::enable_if_t<!B>
    push(const T& value) {
        data.emplace_back(value);
        move_up(data.size() - 1);
    }
    void pop() {
        if constexpr (useHandles) {
            delete data.front().handle;
        }
        data.front() = data.back();
        data.pop_back();
        if(data.empty()) {
            return;
        }
        if constexpr (useHandles) {
            data.front().handle->index = 0;
        }
        move_down(0);
    }
    // Replaces the top element with value:
    void push_pop(const T& value) {
        data.front() = value;
        move_down(0);
    }
    template<bool B = useHandles, class... Args >
    std::enable_if_t<!B>
    emplace( Args&&... args ) {
        data.emplace_back(std::forward<Args>(args)...);
        move_up(data.size() - 1);
    }
    template<bool B = useHandles, class... Args >
    std::enable_if_t<B, Handle*>
    emplace( Args&&... args ) {
        data.emplace_back(std::forward<Args>(args)...);
        Handle* h = data.back().handle;
        h->index = data.size() - 1;
        move_up(data.size() - 1);
        return h;
    }
    template<typename Container>
    void push_range(const Container& c) {
        size_t oldSize = data.size();
        data.insert(data.end(), c.begin(), c.end());
        if constexpr (useHandles) {
            for(size_t i=oldSize; i<data.size(); i++) {
                    data[i].handle->index = i;
                }
        }
        init();
    }
    size_t size() const {
        return data.size();
    }
    bool empty() const {
        return data.empty();
    }
    priority_queue() {}
    // construct a heap from given elements in O(n):
    priority_queue(std::initializer_list<T> c) : data(c.begin(), c.end()) {
        init();
    }
    template<typename Container>
    priority_queue(const Container& c) : data(c.begin(), c.end()) {
        init();
    }
    // Turn existing data into a heap
    void init() {
        if constexpr (useHandles) {
            for(size_t i=0; i<data.size(); i++) {
                data[i].handle->index = i;
            }
        }
        for(int i = data.size()/K; i>=0; i--) {
            move_down(i);
        }
    }
private:
    std::vector<HeapNode<T, useHandles>> data;
    Compare cmp;

    inline void swap_nodes(size_t i, size_t j) {
        std::swap(data[i], data[j]);
        if constexpr (useHandles) {
            data[i].handle->index = i;
            data[j].handle->index = j;
        }
    }

    inline void move_up(size_t i) {
        while(i>0) {
            size_t parent = (i-1)/K;
            if( !cmp(data[parent].value, data[i].value) ){
                break;
            }
            swap_nodes(i, parent);
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
            if(!cmp(data[i].value, data[largest].value)){
                break;
            }
            swap_nodes(i, largest);
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
                return cmp(data[a].value, data[b].value) ? b : a;
        }
    }
    template<int N>
    inline size_t largest_child(size_t firstChild) {
        if constexpr (N == 1) {
            return firstChild;
        } else if constexpr (N == 2) {
            return firstChild + !!cmp(data[firstChild].value, data[firstChild + 1].value);
        } else {
            size_t a = largest_child< N/2 >(firstChild);
            size_t b = largest_child< N - N/2 >(firstChild + N/2);
            return cmp(data[a].value, data[b].value) ? b : a;
        }
    }
};

