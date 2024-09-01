# Ring Buffers

Ring buffer is a queue data structure which has it's tail connected to the head i.e. ringBuffer[n + 1] = ringBuffer[0];
It is used in systems where there is a lot of data that is coming and we want to control the data to the services because the service cannot handle such large influx of data. For example:
1. Multiple sensors sending weather data. We can use ring buffer to store the data and return the data in batches every 30 seconds.
2. Stock exchange where the orders coming in can be stored in the ring buffer.

## Use Cases:

### Audio Processing:

Digital Audio Workstation (DAW): In a DAW like Ableton Live or Logic Pro, a ring buffer might be used to store incoming audio from a microphone or instrument, allowing for real-time processing and effects like reverb or delay to be applied before the audio is sent to the speakers or saved to a file.
Voice Assistants (e.g., Alexa, Google Assistant): Ring buffers could be used to store a short history of the user's voice input, enabling features like "hotword" detection and allowing the assistant to process and understand the full context of the user's query.

### Network Communication:

Web Browsers: When you stream a video on YouTube or Netflix, your browser uses a ring buffer to store chunks of the video data as it's downloaded, allowing for smooth playback even if your internet connection is a bit slow or fluctuates.
Online Gaming: In fast-paced online games, ring buffers are used to store incoming and outgoing network packets, ensuring smooth gameplay by minimizing lag and packet loss.
### Embedded Systems:

Smart Thermostat: A smart thermostat might use a ring buffer to store temperature readings from its sensors, enabling it to calculate trends and adjust the heating or cooling system accordingly.
Automotive Control Systems: In a modern car, ring buffers could be used to store data from various sensors (e.g., engine RPM, wheel speed, tire pressure) for real-time monitoring and control by the car's electronic control unit (ECU).
### Other Examples:

Keyboard Input Buffer: Your computer's keyboard driver likely uses a ring buffer to store keystrokes as you type, ensuring that no keystrokes are lost even if you type faster than the system can process them.
Industrial Automation: In a factory setting, a ring buffer might be used to store data from sensors on a production line, allowing for real-time monitoring and control of the manufacturing process.



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
