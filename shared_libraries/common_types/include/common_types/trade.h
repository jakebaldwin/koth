#ifndef TRADE_H
#define TRADE_H

#include <cstdint>

#include "common_types/enums.h"
#include "common_types/type_aliases.h"

struct Trade {
  TradeId trade_id;
  Price price;
  Quantity quantity;
  Timestamp timestamp;
  OrderId buy_order_id;
  OrderId sell_order_id;
  Side aggressor_side;
  SymbolId symbol;
}

#endif
