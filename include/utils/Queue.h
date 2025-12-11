//
// Created by tetram26 on 05/12/2025.
//

#ifndef CARPET_SRV_QUEUE_H
#define CARPET_SRV_QUEUE_H
#include <iostream>
#include "utils/Mutex.h"
#include "packet/Packet.h"
namespace utils {
    template <typename T> class Queue {
    public :
        Queue(int inc_dec_size);

        Queue(const Queue&);
        Queue& operator=(const Queue&);

        void add(T);

        void remove();

        bool estVide() const;

        ~Queue();
        const T operator[](int) const;
        T operator[](int);

        void display(std::ostream &);

        int size() const;

        void addAll(const Queue<T> &);

    private:
        T * queue;
        int head;
        int tail;
        int tab_size;
        int computed_size;
        int inc_dec_size;
        mutable Mutex queueMutex;
    };
}
#endif //CARPET_SRV_QUEUE_H
