//
// Created by pieter on 24-3-17.
//
#include <string>
#include <hpdf.h>
#include "threadpool.h"

void Worker::operator()() {
    function<void()> task;
    while (true)
    {
        unique_lock<mutex> locker(pool.queue_mutex);
        pool.cond.wait(locker, [this](){ return !pool.tasks.empty() || pool.stop;});
        if(pool.stop) return;
        task = pool.tasks.front();
        pool.tasks.pop_front();
        locker.unlock();
        task();
    }
}

ThreadPool::ThreadPool(size_t threads) : stop(false){
    for(size_t i = 0; i < threads; i++)
    {
        workers.push_back(thread(Worker(*this)));
    }
}

ThreadPool::~ThreadPool() {
    stop = true;
    cond.notify_all();
    for(auto &thread: workers)
    {
        thread.join();
    }
}

template<class F>
void ThreadPool::enqueue(F f) {
    unique_lock<mutex> lock(queue_mutex);
    tasks.push_back(function<void()>(f));
    lock.unlock();
    cond.notify_one();
}