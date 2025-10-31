//
// Created by tetram26 on 26/10/2025.
//
#include <iostream>
#include "utils/TimeUtils.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
using namespace std;


// THIS WAS A PAIN. also I don't know for windaube?
double TimeUtils::getTimeMillis() {
#ifdef _WIN32
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double) counter.QuadPart * 1000.0 / (double) freq.QuadPart;
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (double) tv.tv_sec * 1000.0 + (double) tv.tv_usec / 1000.0;
#endif
}
