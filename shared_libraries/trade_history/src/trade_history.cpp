
#include "trade_history/trade_history.h"

using namespace trading::types;

void TradeHistory::recordTrade(const Trade &trade) { trades_.push_back(trade); }

const Trade &TradeHistory::getLastTrade() const { return trades_.back(); }

const std::vector<Trade> TradeHistory::getRecentTrades(int max_trades) const {
  int size = trades_.size();
  if (max_trades <= size) {
    return std::vector<Trade>(trades_.end() - max_trades, trades_.end());
  }
  return std::vector<Trade>(trades_.end() - size, trades_.end());
}

Count TradeHistory::getTradeCount() const { return trades_.size(); }
