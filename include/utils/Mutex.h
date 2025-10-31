//
// Created by tetram26 on 24/10/2025.
// Mutex, since C++98 doesn't have any of those multitheading thingy

#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>

class Mutex {
public:
    Mutex();

    ~Mutex();

    void lock();

    void unlock();

private:
#ifdef _WIN32
    CRITICAL_SECTION m_cs;
#else
    pthread_mutex_t m_mutex;
#endif
};
#endif //MUTEX_H
