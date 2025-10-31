//
// Created by tetram26 on 24/10/2025.
//
#include "utils/Mutex.h"

Mutex::Mutex() {
#ifdef _WIN32
    InitializeCriticalSection(&this->m_cs);
#else
    pthread_mutex_init(&this->m_mutex, NULL);
#endif
}

Mutex::~Mutex() {
#ifdef _WIN32
    DeleteCriticalSection(&this->m_cs);
#else
    pthread_mutex_destroy(&this->m_mutex);
#endif
}

void Mutex::lock() {
#ifdef _WIN32
    EnterCriticalSection(&this->m_cs);
#else
    pthread_mutex_lock(&this->m_mutex);
#endif
}

void Mutex::unlock() {
#ifdef _WIN32
    LeaveCriticalSection(&this->m_cs);
#else
    pthread_mutex_unlock(&this->m_mutex);
#endif
}
