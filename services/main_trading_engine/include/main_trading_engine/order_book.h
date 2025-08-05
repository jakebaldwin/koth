#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <cstdint>
#include <functional>
#include <map>
#include <vector>

#include "common_types/enums.h"
#include "common_types/market_data.h"
#include "common_types/order.h"
#include "common_types/trade.h"
#include "common_types/type_aliases.h"
#include "memory_pool/order_pool.h"

class OrderBook {
 public:
  OrderBook() : l2_cache_dirty_(false), l3_cache_dirty_(false);

  ~OrderBook() = default;

  /* Core functions - External Interface */
  std::vector<Trade> PlaceOrder(const Order& order);

  void CancelOrder(OrderId order_id);

  void ModifyOrder(OrderId order_id, const Order& modified_order);

  /* L1 Methods */
  const Order& GetBestBid() const;

  const Order& GetBestOffer() const;

  Price GetSpread() const;

  Price GetMidPrice() const;

  /* vol_bid * price_ask + vol_ask * price_bid all
    divided by the volume_bid + volume_ask. */
  Price GetMicroPrice() const;

  /* L2 Methods */
  std::vector<PriceLevel> GetMarketDepthBid(int max_depth) const;

  std::vector<PriceLevel> GetMarketDepthAsk(int max_depth) const;

  BookSnapshot GetBookSnapshot(int max_depth) const;

  /* L3 Methods */
  L3BookSnapshot GetL3BookSnapshot() const;

  /* Query and inspection methods */
  const Order& GetOrderStatus(OrderId order_id) const;

  Quantity GetTotalVolume() const;

  int GetOrderCount() const;

  int GetOrderPosition(OrderId order_id) const;

  /* Admin methods */
  void Clear();

  bool isEmpty() const;

  /* Event methods */
  /** TODO - later on. Think about using callbacks.
   *         callbacks are when the caller can register a function
   *         to be invoked by the order manager when an event happens
   *    - order added
   *    - order cancelled
   *    - order matched / filled
   *    - best bid / offer changed
   *    - new trade occurred
   */
  // void RegisterOrderEventCallback(/* callback function */);
  // void RegisterTradeEventCallback(/* callback function */);

  /* Delta methods */
  /** TODO - implement delta methods to our cached data,
   *         so instead of returning an entire book snapshot
   *         we can just return what changed
   */
 private:
  /* Internal methods */
  void Match() const;
  void UpdatePriceLevels() const;
  void RecomputeBBO() const;
  void BuildL3Cache();

  SymbolId symbol_id_;

  /* Core order storage */
  OrderPool order_pool_;
  std::map<Price, std::queue<Order*>, std::greater<uint64_t>> bids_;
  std::map<Price, std::queue<Order*>> asks_;

  /* Fast Lookup */
  std::unordered_map<OrderId, OrderPosition> order_lookup_;

  /* Cached Data */
  Order* best_bid_;
  Order* best_ask_;

  // L2 cache
  std::vector<PriceLevel> cached_l2_bids_;
  std::vector<PriceLevel> cached_l2_asks_;
  bool l2_cache_dirty_;

  // L3 cache
  L3BookSnapshot cached_l3_;
  bool l3_cache_dirty_;
};

#endif
