#include "market_data_pipeline/market_data_order_book.h"
#include "common_types/constants.h"
#include "common_types/enums.h"
#include "common_types/error_codes.h"
#include "common_types/type_aliases.h"
#include "common_utilities/time_utils.h"

using trading::constants::MaxPrice;
using trading::types::Price;
using trading::types::Quantity;
using trading::types::SymbolId;

MarketDataOrderBook::MarketDataOrderBook(SymbolId symbol_id)
    : symbol_id_(symbol_id), best_bid_price_(0), best_bid_quantity_(0),
      best_ask_price_(MaxPrice), best_ask_quantity_(0) {}

/* Core Methods */
void MarketDataOrderBook::processOrderUpdate(const OrderUpdate &update) {
  if (update.side == Side::Buy) {
    if (update.type == UpdateType::New) {
      // new buy order to process
      bids_[update.price] += update.quantity;
      if (update.price > best_bid_price_) {
        // new order is new best bid
        best_bid_price_ = update.price;
        best_bid_quantity_ = update.quantity;
      } else if (update.price == best_bid_price_) {
        // new order is same price as best bid
        best_bid_quantity_ += update.quantity;
      }
    } else {
      bids_[update.price] -= update.quantity;
      if (bids_[update.price] <= 0) {
        bids_.erase(update.price);
        if (best_bid_price_ == update.price) {
          // best bid was just cancelled completely
          best_bid_price_ = bids_.begin()->first;
          best_bid_quantity_ = bids_.begin()->second;
        }
      } else if (best_bid_price_ == update.price) {
        // best bid reduced quantity
        best_bid_quantity_ -= update.quantity;
      }
    }
  } else {
    if (update.type == UpdateType::New) {
      // new sell order to process
      asks_[update.price] += update.quantity;
      if (update.price < best_ask_price_) {
        // new sell is best offer
        best_ask_price_ = update.price;
        best_ask_quantity_ = update.quantity;
      } else if (update.price == best_ask_price_) {
        // new sell increase best offer quantity
        best_ask_quantity_ += update.quantity;
      }
    } else {
      asks_[update.price] -= update.quantity;
      if (asks_[update.price] <= 0) {
        asks_.erase(update.price);
        if (best_ask_price_ == update.price) {
          // best offer was just cancelled completely
          best_ask_price_ = asks_.begin()->first;
          best_ask_quantity_ = asks_.begin()->second;
        }
      } else if (best_ask_price_ == update.price) {
        // best offer reduced quantity
        best_ask_quantity_ -= update.quantity;
      }
    }
  }
}

void MarketDataOrderBook::processTrade(const Trade &trade) {
  if (trade.aggressor_side == Side::Buy) {
    asks_[trade.price] -= trade.quantity;
    // update best offer data if need be
    if (asks_[trade.price] <= 0) {
      asks_.erase(trade.price);
      if (best_ask_price_ == trade.price) {
        // best offer was just cancelled completely
        best_ask_price_ = asks_.begin()->first;
        best_ask_quantity_ = asks_.begin()->second;
      }
    } else if (best_ask_price_ == trade.price) {
      // best offer reduced quantity
      best_ask_quantity_ -= trade.quantity;
    }
  } else {
    bids_[trade.price] -= trade.quantity;
    if (bids_[trade.price] <= 0) {
      bids_.erase(trade.price);
      if (best_bid_price_ == trade.price) {
        // best bid was just filled completely
        best_bid_price_ = bids_.begin()->first;
        best_bid_quantity_ = bids_.begin()->second;
      }
    } else if (best_bid_price_ == trade.price) {
      // best bid reduced quantity
      best_bid_quantity_ -= trade.quantity;
    }
  }
}

/* L1 Methods */
Price MarketDataOrderBook::getBestBidPrice() const { return best_bid_price_; }

Price MarketDataOrderBook::getBestAskPrice() const { return best_ask_price_; }

Quantity MarketDataOrderBook::getBestBidQuantity() const {
  return best_bid_quantity_;
}

Quantity MarketDataOrderBook::getBestAskQuantity() const {
  return best_ask_quantity_;
}

Price MarketDataOrderBook::getSpread() const {
  return best_ask_price_ - best_bid_price_;
}

Price MarketDataOrderBook::getMidPrice() const {
  return (best_ask_price_ + best_bid_price_) / 2.0;
}

Price MarketDataOrderBook::getMicroPrice() const {
  return (best_ask_price_ * best_bid_quantity_ +
          best_bid_price_ * best_ask_quantity_) /
         (best_ask_quantity_ + best_bid_quantity_);
}
