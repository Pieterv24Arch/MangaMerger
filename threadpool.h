//
// Created by pieter on 24-3-17.
//

#ifndef MANGAMERGER_THREADPOOL_H
#define MANGAMERGER_THREADPOOL_H

#include <thread>
#include <vector>
#include <deque>
#include <condition_variable>
#include <mutex>

using namespace std;

class ThreadPool;

class Worker {
public:
    Worker(ThreadPool &s): pool(s){}
    void operator()();
private:
    ThreadPool &pool;
};

class ThreadPool
{
public:
    ThreadPool(size_t threads);
    template<class F>
    void enqueue(F f);
    ~ThreadPool();
private:
    friend class Worker;

    vector<thread> workers;
    deque<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable cond;
    bool stop;
};


#endif //MANGAMERGER_THREADPOOL_H
