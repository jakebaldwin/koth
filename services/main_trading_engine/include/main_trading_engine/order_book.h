#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <map>
#include <queue>
#include <unordered_map>
#include <vector>

#include "common_types/market_data.h"
#include "common_types/order.h"
#include "common_types/trade.h"
#include "memory_pool/order_pool.h"

// order_book.h

class OrderBook {
public:
  /// Constructor
  OrderBook(trading::types::SymbolId symbol_id);

  /// Copy Constructor
  OrderBook(const OrderBook &other) = delete;

  /// Copy Assignment Operator
  OrderBook &operator=(const OrderBook &other) = delete;

  /// Move Constructor
  OrderBook &operator=(OrderBook &&other) noexcept = delete;

  /// Move Assignment Operator
  OrderBook(OrderBook &&other) noexcept = delete;

  /// Destructor
  ~OrderBook() = default;

  /* Core functions - External Interface */
  std::vector<Trade> placeOrder(const Order &order);

  void cancelOrder(trading::types::OrderId order_id);

  void modifyOrder(trading::types::OrderId order_id,
                   const Order &modified_order);

  /* L1 Methods */
  const Order &getBestBid() const;

  const Order &getBestOffer() const;

  trading::types::Price getSpread() const;

  trading::types::Price getMidPrice() const;

  /* vol_bid * price_ask + vol_ask * price_bid all
    divided by the volume_bid + volume_ask. */
  trading::types::Price getMicroPrice() const;

  /* L2 Methods */
  std::vector<PriceLevel> getMarketDepthBid(int max_depth) const;

  std::vector<PriceLevel> getMarketDepthAsk(int max_depth) const;

  BookSnapshot getBookSnapshot(int max_depth) const;

  /* L3 Methods */
  L3BookSnapshot getL3BookSnapshot() const;

  /* Query and inspection methods */
  const Order &getOrderStatus(trading::types::OrderId order_id) const;

  trading::types::Quantity getTotalVolume() const;

  int getOrderCount() const;

  int getOrderPosition(trading::types::OrderId order_id) const;

  /* Admin methods */
  void clear();

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
  void match() const;
  void updatePriceLevels() const;
  void recomputeBBO() const;
  void buildL3Cache();

  trading::types::SymbolId symbol_id_;

  /* Core order storage */
  OrderPool order_pool_;
  std::map<trading::types::Price, std::queue<Order *>, std::greater<uint64_t>>
      bids_;
  std::map<trading::types::Price, std::queue<Order *>> asks_;

  /* Fast Lookup */
  std::unordered_map<trading::types::OrderId, OrderPosition> order_lookup_;

  /* Cached Data */
  Order *best_bid_;
  Order *best_ask_;

  // L2 cache
  std::vector<PriceLevel> cached_l2_bids_;
  std::vector<PriceLevel> cached_l2_asks_;
  bool l2_cache_dirty_;

  // L3 cache
  L3BookSnapshot cached_l3_;
  bool l3_cache_dirty_;
};

#endif
