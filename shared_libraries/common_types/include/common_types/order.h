#ifndef COMMON_TYPES_ORDER_H
#define COMMON_TYPES_ORDER_H

#include <cstdint>

#include "common_types/enums.h"
#include "common_types/market_data.h"
#include "common_types/type_aliases.h"

struct Order {
  OrderId order_id;
  Price price;
  Quantity quantity;
  Timestamp timestamp;
  Side side;
  OrderType type;
  OrderStatus status;
};

#endif
