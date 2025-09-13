#include "spsc_ring_buffer/spsc_ring_buffer.h"

template <typename T, size_t N>
SpscRingBuffer() : size_(0), producer_(0), consumer_(0) {}

template <typename T, size_t N>
bool SpscRingBuffer<T, N>::try_push(const T &object) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (size_ < N) {
    buffer_[producer_] = object;
    producer_ = (producer_ + 1) & (N - 1);
    size_++;
    return true;
  }
  return false;
}

template <typename T, size_t N> bool SpscRingBuffer<T, N>::try_pop(T &object) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (size_ > 0) {
    object = buffer_[consumer_];
    consumer_ = (consumer_ + 1) & (N - 1);
    size_--;
    return true;
  }
  return false;
}
