#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include "Task.hpp"
#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include <semaphore>    
#include <iostream>  

class ProcessManagement
{
public:
    ProcessManagement(int workerCount = 0);
    ~ProcessManagement();

    bool submitToQueue(std::unique_ptr<Task> task);

private:
    static constexpr int QUEUE_CAPACITY = 1000;

    std::string tasks[QUEUE_CAPACITY];
    int frontIndex;
    int rearIndex;
    int currentSize;

    std::counting_semaphore<> itemsSemaphore{0};
    std::counting_semaphore<> emptySlotsSemaphore{QUEUE_CAPACITY};
    std::binary_semaphore queueMutex{1};

    std::vector<std::thread> workers;
    std::atomic<bool> stopRequested;

    void workerLoop();
};

#endif