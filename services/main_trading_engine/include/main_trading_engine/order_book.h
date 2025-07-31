#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "main_training_engine/order.h"
#include "main_training_engine/trade.h"

class OrderBook {
 public:
  OrderBook();

  ~OrderBook() = default;

  /* Common functions */
  // TODO - does this need to return a code? status?
  void PlaceOrder(const Order& order);

  void CancelOrder(const uint64_t& order_id);

  void ModifyOrder(const uint64_t& order_id, const Order& modified_order);

  /* L1 Methods */
  const Order& GetBestBid() const;

  const Order& GetBestOffer() const;

  const Trade& GetLastTrade() const;

  // Should these return a const reference or just a uint64_t, because
  // the calling function might do math with these and want them to be
  // modifiable
  const uint64_t& GetSpread() const;

  const uint64_t& GetMidPrice() const;

  /* vol_bid * price_ask + vol_ask * price_bid all
    divided by the volume_bid + volume_ask. */
  const uint64_t GetMicroPrice() const;

  /* L2 Methods */
  // const ??? GetMarketDepth(??) const;

  // const ?? GetBookSnapshot() const;

  /* L3 Methods */
  // time price priority based methods

  /* Query and inspection methods */
  const Order& GetOrderStatus(uint64_t order_id) const;

  uint64_t GetTotalVolume() const;

  int GetOrderCount() const;

  // should this be multiple methods?
  // const ??? GetBookStatistics() const;

  /* Admin methods */
  const void Clear() const;

  const bool isEmpty() const;

  /* Event methods */
  // order added
  //
  // order cancelled
  //
  // order matched / filled
  //
  // best bid / offer changed
  //
  // new trade occurred

  const void Match() const;

  const void UpdatePriceLevels() const;

  const void RecomputeBBO() const;

 private:
  uint32_t symbol_id_;
};

#endif
