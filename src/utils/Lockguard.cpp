#include "utils/LockGuard.h"

LockGuard::LockGuard(Mutex &mut) : mtx(mut) {
    mtx.lock();
}

LockGuard::~LockGuard() {
    mtx.unlock();
}
