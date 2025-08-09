#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <vector>

#include "common_types/type_aliases.h"

// memory_pool.h

/// Generic class for pre-allocated storage of structs, which handles lookups
/// and exchange of pointers

/// TODO - look into making sure this will be threadsafe
///      - also does this need to be per-symbol
///      - also does this T type need a clear method type thing for deallocation
template <typename T> class MemoryPool {
public:
  MemoryPool(trading::types::Count pool_size);

  /// Grab an available pointer
  /// \return object pointer that has null values everywhere
  T *getObject();

  /// Return a pointer
  /// \pre the pointer was allocated by the same instances getObject method
  void releaseObject(T *object);

  trading::types::Count availableCount() const;
  trading::types::Count totalCount() const;

private:
  std::vector<T> storage_;
  std::vector<T *> available_;
};

#endif
