#ifndef TRADE_HISTORY_H
#define TRADE_HISTORY_H

#include <vector>

#include "common_types/trade.h"
#include "common_types/type_aliases.h"

// trade_history.h

/// Object to store trades created by the order book
class TradeHistory {
public:
  /// Default Constructor
  TradeHistory() = default;

  /// Copy Constructor
  TradeHistory(const TradeHistory &other) = delete;

  /// Copy Assignment Operator
  TradeHistory &operator=(const TradeHistory &other) = delete;

  /// Move Constructor
  TradeHistory &operator=(TradeHistory &&other) = delete;

  /// Move Assignment Operator
  TradeHistory(TradeHistory &&other) = delete;

  /// Destructor
  ~TradeHistory() = default;

  /// Adds a trade object to the store of trades
  /// \post the trade is recorded in the object
  void recordTrade(const Trade &trade);

  /// Returns the last trade if one exists
  /// \param trade to store
  /// \throws Exception if trade list is empty
  /// \pre trade list is not empty
  const Trade &getLastTrade() const;

  /// Returns all last trades up to max_trades size
  /// \param max_trades to return
  /// \return vector of up to max_trades Trade objects ordered as they were
  /// added
  const std::vector<Trade> getRecentTrades(int max_trades) const;

  trading::types::Count getTradeCount() const;

private:
  std::vector<Trade> trades_;
};

#endif
