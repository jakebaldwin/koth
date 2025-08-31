#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <vector>

#include "common_types/type_aliases.h"

// memory_pool.h

/// Generic class for pre-allocated storage of structs, which handles lookups
/// and exchange of pointers. Optimized for hot path, it does no checks

template <typename T> class MemoryPool {
public:
  MemoryPool(trading::types::Count pool_size);

  /// Grab an available pointer
  /// This method is a hot path and does no validation that there
  /// are pointers available. It will be up to system monitors and generous
  /// pre-allocation to determine if the memory pools are becoming exhausted
  /// \return object pointer
  T *getObject();

  /// Return a pointer to the pool so it can be used again elsewhere
  /// This method does not care if the underlying object has content still,
  /// as it assumes the next user will overwrite.
  /// \pre the pointer was allocated by the same instances getObject method
  void releaseObject(T *object);

  trading::types::Count availableCount() const;
  trading::types::Count totalCount() const;

private:
  std::vector<T> storage_;
  std::vector<T *> available_;
};

#endif
