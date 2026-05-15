//
// Created by tetram26 on 05/12/2025.
//
#include "utils/Queue.h"
#include <iostream>
using namespace std;

namespace utils {
template<class T>
Queue<T>::Queue(int inc_dec_size)
  : queue(NULL)
  , computed_size(0)
  , head(0)
  , tail(0)
  , tab_size(0)
  , inc_dec_size(inc_dec_size)
{
}

template<class T>
Queue<T>::~Queue()
{
  if (this->queue != NULL) {
    delete[] this->queue;
    this->queue = NULL;
  }
}

template<class T>
Queue<T>::Queue(const Queue& queue)
  : head(0)
  , tail(queue.computed_size % queue.tab_size)
  , tab_size(queue.tab_size)
  , computed_size(queue.computed_size)
  , inc_dec_size(queue.inc_dec_size)
{
  this->queue = new T[queue.tab_size];
  for (unsigned int i = 0; i < queue.computed_size; i++) {
    this->queue[i] = queue.queue[(i + queue.head) % queue.tab_size];
  }
}

template<class T>
Queue<T>&
Queue<T>::operator=(const Queue& queue)
{
  if (this->queue != NULL && this->tab_size != queue.tab_size) {
    delete[] this->queue;
    this->queue = new T[queue.tab_size];
  }

  if (queue.queue != NULL) {
    for (int i = 0; i < queue.computed_size; i++) {
      this->queue[i] = queue.queue[(i + queue.head) % queue.tab_size];
    }
  }
  this->inc_dec_size = queue.inc_dec_size;
  this->tab_size = queue.tab_size;
  this->tail = queue.computed_size;
  this->head = 0;
  this->computed_size = queue.computed_size;
  return *this;
}

template<class T>
void
Queue<T>::add(T caractere)
{

  lock_guard lock(this->queueMutex);
  // Si la taille du tableau n'est pas suffisante pour enfilé
  if (this->computed_size >= this->tab_size) {
    // On réalloue un tableau plus grand
    T* new_tab = new T[this->tab_size + this->inc_dec_size];
    // Puis on recopie tout dedans
    for (unsigned int i = 0; i < this->tab_size; i++) {
      new_tab[i] = this->queue[(i + this->head) % this->tab_size];
    }
    this->head = 0;
    this->tail = this->tab_size;
    this->tab_size += this->inc_dec_size;
    delete[] this->queue;
    this->queue = new_tab;
  }
  ++this->computed_size;
  this->queue[this->tail] = caractere;
  this->tail = (this->tail + 1) % this->tab_size;
}

template<class T>
void
Queue<T>::remove()
{
  lock_guard lock(this->queueMutex);
  if (this->computed_size != 0) {
    if (this->computed_size <= this->tab_size / 4 &&
        this->tab_size > this->inc_dec_size) {
      // Reduce the array
      T* new_tab = new T[this->tab_size - this->inc_dec_size];
      for (unsigned int i = 0; i < this->computed_size; i++) {
        new_tab[i] = this->queue[(i + this->head) % this->tab_size];
      }
      this->head = 0;
      this->tab_size -= this->inc_dec_size;
      this->tail = this->computed_size % this->tab_size;
      delete[] this->queue;
      this->queue = new_tab;
    } else {
      this->head = (this->head + 1) % this->tab_size;
    }
    --this->computed_size;
  }
}

template<class T>
bool
Queue<T>::isEmpty() const
{
  return this->size() == 0;
}

template<class T>
const T
Queue<T>::operator[](int index) const
{
  lock_guard lock(this->queueMutex);
  if (index < 0 || index >= this->computed_size) {
    throw invalid_argument("Invalid index");
  }
  return this->queue[(this->head + index) % this->tab_size];
}

template<class T>
T
Queue<T>::operator[](int index)
{
  lock_guard lock(this->queueMutex);
  if (index < 0 || index >= this->computed_size) {
    throw invalid_argument("Invalid index");
  }
  return this->queue[(this->head + index) % this->tab_size];
}

template<class T>
void
Queue<T>::display(std::ostream& outputStream)
{
  lock_guard lock(this->queueMutex);
  outputStream << "[";
  for (unsigned int i = 0; i < this->computed_size; i++) {
    if (this->computed_size - i == 1) {
      outputStream << (*this)[i];
    } else {
      outputStream << (*this)[i] << ",";
    }
  }
  outputStream << "]";
}

template<typename T>
int
Queue<T>::size() const
{
  lock_guard lock(this->queueMutex);
  return this->computed_size;
}

template<typename T>
void
Queue<T>::addAll(const Queue<T>& queue)
{
  if (this == &queue)
    return;
  const Queue<T>* first = (this < &queue) ? this : &queue;
  const Queue<T>* second = (this < &queue) ? &queue : this;

  lock_guard lockFirst(first->queueMutex);
  lock_guard lockSecond(second->queueMutex);
  for (unsigned int i = 0; i < queue.computed_size; i++) {
    if (this->computed_size >= this->tab_size) {
      T* new_tab = new T[this->tab_size + this->inc_dec_size];

      for (unsigned int j = 0; j < this->tab_size; j++)
        new_tab[j] = this->queue[(this->head + j) % this->tab_size];

      this->head = 0;
      this->tail = this->tab_size;
      this->tab_size += this->inc_dec_size;

      delete[] this->queue;
      this->queue = new_tab;
    }
    this->queue[this->tail] = queue.queue[(queue.head + i) % queue.tab_size];

    this->tail = (this->tail + 1) % this->tab_size;
    ++this->computed_size;
  }
}
}
