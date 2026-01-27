# priority_queue
A fast implementation of a priority queue mostly compatible with `std::priority_queue`.

## Usage
In most aspects the queue is analogous to `std::priority_queue` from the C++ standard library.
```C++
priority_queue<int> pq;
pq.push(5);
pq.push(3);
pq.push(8);
pq.push(1);
pq.push(7);
while(!pq.empty()){
  std::cout<<pq.top()<<" ";
  pq.pop();
}
std::cout<<std::endl;
```
Output:
```
8 7 5 3 1
```
Of course if you want the minimal element to be on top:
```C++
priority_queue<int, std::greater<int>> pq;
...
```
Output:
```
1 3 5 7 8
```

## Key differences
### Customizable arity of the queue
The queue is implemented as a binary heap or a general k-ary heap. This can be configured as a template parameter:
```C++
priority_queue<int, std::less<int>, 2> pq2;
priority_queue<int, std::less<int>, 4> pq4;
priority_queue<int, std::less<int>, 8> pq8;
```
The default arity is 4 and the code is mostly optimized to run binary and quaternary heaps.

### Initialization from an initializater list or a container
You can pass initial values as an initalization list:
```C++
priority_queue<int> pq({8, 2, 6, 4, 7, 3});
```
or copy elements from any other container
```C++
std::vector<int> xs({8, 2, 6, 4, 7, 3});
priority_queue<int> pq(xs);
```
The creation time for the new queue is **O(n)**.

### Repairing the internal structure of the queue
If you know what you are doing and mess with the elements directly, a call to `init()` will fix the internal representation of the heap:
```C++
priority_queue<int> pq({8, 2, 6, 4, 7, 3});
for(auto& x:pq){
  if(x%2==0){
    x = 0; // this changes the top element 8 and breaks the heap
  }
}
// at this point the heap is broken:
std::cout<<"Wrong top: "<<pq.top()<<"\n"; // WRONG OUTPUT!!!
pq.init();
std::cout<<"Correct top: "<<pq.top()<<"\n"; // CORRECT OUTPUT!!!
```
Output:
```
Wrong top: 0
Correct top: 7
```

### Replacing the top element faster
A method `push_pop(<new element>)` is essentially analogous to calling `pop()` and `push(<new element>)` but is significantly faster:
```C++
priority_queue<int> pq({8, 2, 6, 4, 7, 3});
pq.push_pop(5); // same as pq.pop(); pq.push(5);
// now the maximum element 8 is gone from the queue, but 5 is in its place (maintaining the proper structure)
while(!pq.empty()){
  std::cout<<pq.top()<<" ";
  pq.pop();
}
std::cout<<std::endl;
```
The output is `7 6 5 4 3 2`

### Pushing multiple elements
Similar to `priority_queue` from standard C++ library, it is possible to push many elements to the queue:
```C++
priority_queue<int> pq({8, 2, 6, 4, 7, 3});
std::vector<int> xs({5, 1, 11});
pq.push_range(xs);
```
Note that pushing m elements in a queue with n elements takes **O(m+n)** time. If m is small consider doing `pq.push(..)` for each element instead.

### Maintaining access to elements and updating them inside the queue
It may be desirable to update element priorities within the queue
(for example in Dijkstra's algorithm).
This can be done with a special version of the queue (albeit with some overhead):
```C++
priority_queue<int, std::less<int>, 4, true> pq; // the last template parameter enables element handles
```
Which then allows to get handles to elements when pushing them:
```C++
auto h = pq.push(5); // h is of type priority_queue_handle*
```
With the handle `h` you can update the priority of the element:
```C++
pq.update(h, 3); // change the priority of the element with handle h to 3
```
Or you can change the element directly and then fix the heap:
```C++
pq[h] = 7; // change the priority of the element with handle h to 7
pq.fix(h); // fix the heap after changing the element directly
```
Here is a complete example:
```C++
priority_queue<int, std::less<int>, 4, true> pq({5, 3, 8, 1, 2, 7, 4, 6});
priority_queue_handle* h = pq.push(11);
std::cout<<"top = "<<pq.top()<<"\n";
// change the priority of element with handle h to 0
pq.update(h, 0);
std::cout<<"top = "<<pq.top()<<"\n";
// change the priority of element with handle h to 5 (manual fix)
pq[h] = 5;
pq.fix(h);
while(!pq.empty()){
   std::cout<<pq.top()<<" ";
   pq.pop();
}
std::cout<<"\n";
```
Output:
```
top = 11
top = 0
8 7 6 5 5 4 3 2 1
```

## Performance
The following tests were performed using `GCC 15.2.0` compiled with `g++ -O2`, on an Intel Core Ultra 7 155U CPU.

The following graph shows time in nanoseconds per one push operation depending on the size of the heap:
<img width="1918" height="1137" alt="push" src="https://github.com/user-attachments/assets/4e0d9ff8-e973-49c6-80be-fa284ff03426" />

The following graph shows time in nanoseconds per one pop operation depending on the size of the heap:
<img width="1918" height="1137" alt="pop" src="https://github.com/user-attachments/assets/54a07cf7-97b3-480c-989b-d552efe14cfe" />

The quaternary heap (the default) shows best performance.

## Acknowledgements
Many ideas of this implementation were inspired by a [Malte Skarupke article](https://probablydance.com/2020/08/31/on-modern-hardware-the-min-max-heap-beats-a-binary-heap/)
