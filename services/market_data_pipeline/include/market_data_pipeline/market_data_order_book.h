#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <deque>
#include <map>

#include "common_types/market_data_types.h"

/// market_data_order_book.h
/// this class is the Market Data Order Book
/// It serves as the market-tracking order book, storing all of a single
/// symbols orders and trades. It does not do any matching, just tracking and
/// passes relevant output to the Trading Engine Core via the Pricing Engine

/// Future enhancements
/// L2 Data, L3 Data, Admin methods, callback functions?

class MarketDataOrderBook {
public:
  /// Constructor
  MarketDataOrderBook(trading::types::SymbolId symbol_id);

  /// Copy Constructor
  MarketDataOrderBook(const MarketDataOrderBook &other) = delete;

  /// Copy Assignment Operator
  MarketDataOrderBook &operator=(const MarketDataOrderBook &other) = delete;

  /// Move Constructor
  MarketDataOrderBook &operator=(MarketDataOrderBook &&other) noexcept = delete;

  /// Move Assignment Operator
  MarketDataOrderBook(MarketDataOrderBook &&other) noexcept = delete;

  /// Destructor
  ~MarketDataOrderBook() = default;

  /* Core functions - External Interface */
  void processOrderUpdate(const OrderUpdate &update);

  /// This method processes a trade, it assumes the orders are in the book
  void processTrade(const Trade &trade);

  /* L1 Methods */
  trading::types::Price getBestBidPrice() const;

  trading::types::Price getBestAskPrice() const;

  trading::types::Quantity getBestBidQuantity() const;

  trading::types::Quantity getBestAskQuantity() const;

  trading::types::Price getSpread() const;

  trading::types::Price getMidPrice() const;

  trading::types::Price getMicroPrice() const;

  /** TODO - later on. Think about using
   * callbacks. callbacks are when the caller can
   * register a function to be invoked by the
   * order manager when an event happens
   *    - order added
   *    - order cancelled
   *    - order matched / filled
   *    - best bid / offer changed
   *    - new trade occurred
   */
  // void RegisterOrderEventCallback(/* callback
  // function */); void
  // RegisterTradeEventCallback(/* callback
  // function */);

  /* Delta methods */
  /** TODO - implement delta methods to our cached
   * data, so instead of returning an entire book
   * snapshot we can just return what changed
   */
private:
  trading::types::SymbolId symbol_id_;

  /* Core order storage */
  std::map<trading::types::Price, trading::types::Quantity,
           std::greater<trading::types::Price>>
      bids_;
  std::map<trading::types::Price, trading::types::Quantity> asks_;

  trading::types::Price best_bid_price_;
  trading::types::Quantity best_bid_quantity_;

  trading::types::Price best_ask_price_;
  trading::types::Quantity best_ask_quantity_;

  // consider using ring buffer
  std::deque<Trade> recent_trades_;
};

#endif
