#ifndef TRADE_H
#define TRADE_H

#include "common_types/enums.h"
#include "common_types/type_aliases.h"

struct Trade {
  trading::types::TradeId trade_id;
  trading::types::Price price;
  trading::types::Quantity quantity;
  trading::types::Timestamp timestamp;
  trading::types::OrderId buy_order_id;
  trading::types::OrderId sell_order_id;
  Side aggressor_side;
  trading::types::SymbolId symbol;
};

#endif
