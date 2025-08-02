#ifndef TRADE_HISTORY_H
#define TRADE_HISTORY_H

#include <vector>

#include "common_types/constants.h"
#include "common_types/trade.h"

using namespace trading::constants;

class TradeHistory {
 public:
  TradeHistory();
  ~TradeHistory() = default;

  void RecordTrade(const Trade& trade);

  const Trade& GetLastTrade() const;

  const std::vector<Trade> GetRecentTrades(int max_trades) const;

  size_t GetTradeCount() const;

 private:
  std::vector<Trade> trades_;
  size_t max_trades_ = kMaxInMemoryTrades;
};

#endif
