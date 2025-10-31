//
// Created by tetram26 on 31/10/25.
// Since C++98 doesn't have lockguard thingy

#ifndef LOCKGUARD_H
#define LOCKGUARD_H

#include "Mutex.h"

class LockGuard {
    public:
        LockGuard(Mutex &);
        ~LockGuard();
    private:
        // NO COPY.
        LockGuard(const LockGuard &);
        LockGuard &operator=(const LockGuard &);

        Mutex &mtx;
};

#endif // LOCKGUARD_H