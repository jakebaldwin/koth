
#include "memory_pool/memory_pool.h"

using namespace trading::types;

template <typename T> MemoryPool<T>::MemoryPool(Count pool_size) {
  storage_.reserve(pool_size);

  for (int i = 0; i < storage_.size(); i++) {
    available_[i] = &storage_[i];
  }
}

template <typename T> T *MemoryPool<T>::getObject() {
  T *res = available_.back();
  available_.pop_back();
  return res;
}

template <typename T> void MemoryPool<T>::releaseObject(T *object) {
  available_.push_back(object);
}

template <typename T> Count MemoryPool<T>::availableCount() const {
  return available_.size();
}

template <typename T> Count MemoryPool<T>::totalCount() const {
  return storage_.size();
}
