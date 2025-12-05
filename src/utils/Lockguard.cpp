#include "utils/LockGuard.h"
using namespace utils;
LockGuard::LockGuard(Mutex &mut) : mtx(mut) {
    mtx.lock();
}

LockGuard::~LockGuard() {
    mtx.unlock();
}
