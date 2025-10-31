//
// Created by tetram26 on 26/10/2025.
//

#ifndef THREADUTILS_H
#define THREADUTILS_H

#ifdef _WIN32
#include <windows.h>
typedef HANDLE ThreadHandle;
#else
#include <pthread.h>
typedef pthread_t ThreadHandle;
#endif
typedef void* (*ThreadFunction)(void*);

class Thread {
    public:
        Thread();

        bool createThread(ThreadFunction, void*);
        void joinThread() const;
        void sleep(double);

    private:
        ThreadHandle handle;

};
#endif //THREADUTILS_H