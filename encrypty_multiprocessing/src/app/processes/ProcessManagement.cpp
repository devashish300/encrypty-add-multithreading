#include <iostream>
#include "ProcessManagement.hpp"
#include <cstring>
#include "../encryptDecrypt/Cryption.hpp"
#include <atomic>
#include <thread>

ProcessManagement::ProcessManagement(int workerCount) {
    frontIndex = 0;
    rearIndex = 0;
    currentSize = 0;
    stopRequested.store(false);

    unsigned int hc = std::thread::hardware_concurrency();
    if (workerCount <= 0) {
        workerCount = hc > 0 ? static_cast<int>(hc) : 4;
    }
    workers.reserve(static_cast<size_t>(workerCount));

    for (int i = 0; i < workerCount; ++i) {
        workers.emplace_back(&ProcessManagement::workerLoop, this);
    }
}

ProcessManagement::~ProcessManagement() {
    stopRequested.store(true);
    for (size_t i = 0; i < workers.size(); ++i) {
        itemsSemaphore.release();
    }
    for (auto &t : workers) {
        if (t.joinable()) t.join();
    }
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    if (stopRequested.load()) return false;
    emptySlotsSemaphore.acquire();
    queueMutex.acquire();
    tasks[rearIndex] = task->toString();
    rearIndex = (rearIndex + 1) % QUEUE_CAPACITY;
    ++currentSize;
    queueMutex.release();
    itemsSemaphore.release();
    return true;
}

void ProcessManagement::workerLoop() {
    for (;;) {
        itemsSemaphore.acquire();
        if (stopRequested.load()) break;
        queueMutex.acquire();
        std::string taskStr = tasks[frontIndex];
        frontIndex = (frontIndex + 1) % QUEUE_CAPACITY;
        --currentSize;
        queueMutex.release();
        emptySlotsSemaphore.release();
        executeCryption(taskStr);
    }
}