# priority_queue
A fast implementation of a priority queue mostly compatible with std

## Key differenced
### Initialization from an initialization list or a container
One can pass initial values as an initalization list:
```C++
priorityQueue<int> pq({8, 2, 6, 4, 7, 3});
```
or copy elements from any other container
```C++
std::vector<int> xs({8, 2, 6, 4, 7, 3});
priorityQueue<int> pq(xs);
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
The expected output is `7 6 5 4 3 2`

## Acknowledgements
Many ideas of this implementation were inspired by a [Malte Skarupke article](https://probablydance.com/2020/08/31/on-modern-hardware-the-min-max-heap-beats-a-binary-heap/)
