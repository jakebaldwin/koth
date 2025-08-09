#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <vector>

template <typename T> class MemoryPool {
public:
  MemoryPool(size_t pool_size);

  T *GetObject();
  void ReleaseObject(T *object);
  size_t AvailableCount() const;
  size_t TotalCount() const;

private:
  std::vector<T> storage_;
  std::vector<T *> available_;
};

#endif
