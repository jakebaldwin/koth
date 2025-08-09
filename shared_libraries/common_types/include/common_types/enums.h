#ifndef COMMON_TYPES_ENUMS_H
#define COMMON_TYPES_ENUMS_H

#include <cstdint>

enum class Side : uint8_t { Buy, Sell };
enum class OrderType : uint8_t { Limit, Market, Stop, IOC };
enum class OrderStatus : uint8_t { New, Partial, Filled, Cancelled };

#endif
