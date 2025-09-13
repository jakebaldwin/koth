#ifndef SPSC_RING_BUFFER_H
#define SPSC_RING_BUFFER_H

#include <array>
#include <mutex>

constexpr bool is_power_of_2(size_t n) {
  return (n > 0) && ((n & (n - 1)) == 0);
}

template <typename T, size_t N> class SpscRingBuffer {
  static_assert(is_power_of_2(N),
                "size of spsc ring buffer must be power of 2");

public:
  SpscRingBuffer();

  bool try_push(const T &object);

  bool try_pop(T &item);

private:
  std::mutex mutex_;
  std::array<T, N> buffer_;
  size_t producer_;
  size_t consumer_;
  size_t size_;
};

#endif
