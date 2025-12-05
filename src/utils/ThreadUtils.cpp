//
// Created by tetram26 on 26/10/2025.
//

#include "utils/ThreadUtils.h"

#include <unistd.h>
using namespace utils;

Thread::Thread() : handle() {
}

bool Thread::createThread(ThreadFunction func, void *arg) {
#ifdef _WIN32
    *handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) func, arg, 0, 0);
    return *handle != 0;
#else
    return pthread_create(&handle, 0, func, arg) == 0;
#endif
}

void Thread::joinThread() const {
#ifdef _WIN32
    WaitForSingleObject(handle, INFINITE);
    CloseHandle(handle);
#else
    pthread_join(handle, 0);
#endif
}

void Thread::sleep(double time2sleep) {
#ifdef _WIN32
    Sleep(time2sleep);
#else
    usleep(time2sleep * 1000);
#endif
}
