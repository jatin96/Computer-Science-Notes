# Ring Buffers

Ring buffer is a queue data structure which has it's tail connected to the head i.e. ringBuffer[n + 1] = ringBuffer[0];
It is used in systems where there is a lot of data that is coming and we want to control the data to the services because the service cannot handle such large influx of data. For example:
1. Multiple sensors sending weather data. We can use ring buffer to store the data and return the data in batches every 30 seconds.
2. Stock exchange where the orders coming in can be stored in the ring buffer.

Code:

```cpp
#include <iostream>
#include <vector>

template <typename T>
class RingBuffer {
public:
    RingBuffer(size_t capacity) : 
        buffer(capacity), 
        head(0), 
        tail(0), 
        size(0) 
    {}

    bool isEmpty() const {
        return size == 0;
    }

    bool isFull() const {
        return size == buffer.size();
    }

    void push(const T& item) {
        if (isFull()) {
            // Overwrite oldest element if full
            buffer[tail] = item;
            tail = (tail + 1) % buffer.size();
            head = (head + 1) % buffer.size(); // Move head to maintain size
        } else {
            buffer[tail] = item;
            tail = (tail + 1) % buffer.size();
            ++size;
        }
    }

    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Cannot pop from an empty RingBuffer");
        }

        T item = buffer[head];
        head = (head + 1) % buffer.size();
        --size;
        return item;
    }

private:
    std::vector<T> buffer;
    size_t head;
    size_t tail;
    size_t size;
};

int main() {
    RingBuffer<int> rb(5); // Capacity of 5

    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);
    rb.push(5); // Buffer is now full

    std::cout << rb.pop() << std::endl; // Output: 1
    std::cout << rb.pop() << std::endl; // Output: 2

    rb.push(6); // Overwrites 3

    std::cout << rb.pop() << std::endl; // Output: 4
    std::cout << rb.pop() << std::endl; // Output: 5
    std::cout << rb.pop() << std::endl; // Output: 6

    return 0;
}
```
