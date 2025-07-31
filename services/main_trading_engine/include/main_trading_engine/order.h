#ifndef ORDER_H
#define ORDER_H

enum OrderType {
  BUY,
  SELL
}

struct Order {
  uint64_t order_id_;
  uint64_t price_;
  uint64_t quantity_;
  uint64_t timestamp;
  Side side_;
  OrderType type_;
}

#endif
