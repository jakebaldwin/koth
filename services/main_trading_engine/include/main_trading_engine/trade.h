#ifndef TRADE_H
#define TRADE_H

#include "main_trading_engine/order.h"

struct Trade {
  std::pair<Order, Order> match;
}

#endif
