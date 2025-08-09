#ifndef COMMON_TYPES_ORDER_H
#define COMMON_TYPES_ORDER_H

#include "common_types/enums.h"
#include "common_types/type_aliases.h"

struct Order {
  trading::types::OrderId order_id;
  trading::types::Price price;
  trading::types::Quantity quantity;
  trading::types::Timestamp timestamp;
  Side side;
  OrderType type;
  OrderStatus status;
};

#endif
