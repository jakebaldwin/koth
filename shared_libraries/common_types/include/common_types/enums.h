#ifndef COMMON_TYPES_ENUMS_H
#define COMMON_TYPES_ENUMS_H

enum class Side { Buy, Sell };
enum class OrderType { Limit, Market, Stop, IOC };
enum class OrderStatus { New, Partial, Filled, Cancelled };

#endif
