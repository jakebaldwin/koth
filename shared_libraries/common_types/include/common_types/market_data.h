#ifndef COMMON_TYPES_MARKET_DATA_H
#define COMMON_TYPES_MARKET_DATA_H

#include <vector>

#include "common_types/enums.h"
#include "common_types/type_aliases.h"

struct PriceLevel {
  trading::types::Quantity quantity;
  trading::types::Quantity total_quantity;
  int order_count;
};

struct BookSnapshot {
  std::vector<PriceLevel> bids;
  std::vector<PriceLevel> asks;
  trading::types::Timestamp timestamp;
  trading::types::SequenceNumber sequence_number;
};

struct OrderPosition {
  trading::types::Price price;
  Side side;
  int position;
};

#endif
