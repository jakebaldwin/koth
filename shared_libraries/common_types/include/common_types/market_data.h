#ifndef COMMON_TYPES_MARKET_DATA_H
#define COMMON_TYPES_MARKET_DATA_H

#include <cstdint>
#include <vector>

#include "common_types/enums.h"
#include "common_types/type_aliases.h"

struct PriceLevel {
  Quantity quantity;
  Quantity total_quantity;
  int order_count;
};

struct BookSnapshot {
  std::vector<PriceLevel> bids;
  std::vector<PriceLevel> asks;
  Timestamp timestamp;
  SequenceNumber sequence_number;
};

struct OrderPosition {
  Price price;
  Side side;
  int position;
};
