# priority_queue
A fast implementation of a priority queue mostly compatible with std

## Key differences
### Customizable arity of the queue
The queue is implemented as a binary heap or a general k-ary heap. This can be configured as a template parameter:
```C++
priorityQueue<int, std::less<int>, 2> pq2;
priorityQueue<int, std::less<int>, 4> pq4;
priorityQueue<int, std::less<int>, 8> pq8;
```
The default arity is 4 and the code is mostly optimized to run binary and quaternary heaps.
### Initialization from an initialization list or a container
You can pass initial values as an initalization list:
```C++
priorityQueue<int> pq({8, 2, 6, 4, 7, 3});
```
or copy elements from any other container
```C++
std::vector<int> xs({8, 2, 6, 4, 7, 3});
priorityQueue<int> pq(xs);
```
The creation time for the new queue is O(n).
### Fixing the internal structure of the queue
If you know what you are doing and mess with the elements directly, a call to `init()` will fix the internal heap:
```C++    
priorityQueue<int> pq({8, 2, 6, 4, 7, 3});
for(auto& x:pq){
  if(x%2==0){
    x = 0; // this changes the top element 8 and breakes the heap
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
priorityQueue<int> pq({8, 2, 6, 4, 7, 3});
pq.push_pop(5); // same as pq.pop(); pq.push(5);
// now the maximum element 8 is gone from the queue, but 5 is in it's place (maintaining the proper structure)
while(!pq.empty()){
  std::cout<<pq.top()<<" ";
  pq.pop();
}
std::cout<<std::endl;
```
The output is `7 6 5 4 3 2`
### Pushing multiple elements
Similar to priority_queue from standard C++ library, it is possible to push many elements to the queue:
```C++
priorityQueue<int> pq({8, 2, 6, 4, 7, 3});
std::vector<int> xs({5, 1, 11});
pq.push_range(xs);
```
Note that pushing m elements in a queue with n elements takes O(m+n) time. If m is small consider doing `pq.push(..)` for each element instead.
## Acknowledgements
Many ideas of this implementation were inspired by a [Malte Skarupke article](https://probablydance.com/2020/08/31/on-modern-hardware-the-min-max-heap-beats-a-binary-heap/)
