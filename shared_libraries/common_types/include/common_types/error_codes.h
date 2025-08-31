#ifndef COMMON_TYPES_ERROR_CODES_H
#define COMMON_TYPES_ERROR_CODES_H

#include <cstdint>

namespace trading {

enum class ResultCode : uint8_t {

  // Success
  Success = 0,

  // Generic System Errors (1-99)
  SystemError = 1,
  InvalidParameter = 2,
  ResourceExhausted = 3,
  Timeout = 4,
  NotFound = 5,
  AlreadyExists = 6,
  PermissionDenied = 7,

  // Memory / Resource Errors (100-199)
  MemoryPoolExhausted = 100,
  OrderPoolExhausted = 101,
  TradePoolExhausted = 102,
  BufferFull = 103,
  QueueFull = 104,

  // Order Validation Errors (200-299)
  InvalidPrice = 200,
  InvalidQuantity = 201,
  InvalidSide = 202,
  InvalidOrderType = 203,
  PriceOutOfRange = 204,
  QuantityToSmall = 205,
  OrderNotFound = 206,
  OrderAlreadyFilled = 207,

};

} // namespace trading

#endif
