#include "Queue.h"
#include <iostream>

#include "Map.h"

// Constructor to initialize an empty queue
Queue::Queue() {
    // TODO: Your code here
    front = rear = -1;

}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    // Implement circular structure 
    // Add the province

    if (front == -1 && rear == -1) {
        front = rear = 0;
    }
    else if ((rear + 1) % MAX_SIZE == front) {
        std::cerr << "Queue is full" << std::endl;
        return;
    }
    else {
        rear = (rear + 1) % MAX_SIZE;
    }

    data[rear] = province;
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    // TODO: Your code here

    if (front == -1) {
        std::cerr << "Queue is empty" << std::endl;
        return -1;
    }

    int retVal = data[front];

    if (front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % MAX_SIZE;
    }

    return retVal;
}

// Returns the front province without removing it
int Queue::peek() const {
    // TODO: Your code here
    if (front == -1) {
        std::cout << "Queue is empty!" << std::endl;
        return -1;
    }
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here
    return front == -1;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here
    if (front == -1 && rear == -1) {
        front = rear = 0;
    } else if ((rear + 1) % MAX_SIZE == front) {
        std::cerr << "Queue overflow" << std::endl;
        return;
    } else {
        front--;
        if (front<0) {
            front = MAX_SIZE + front;
        }
    }

    data[front] = province;
}