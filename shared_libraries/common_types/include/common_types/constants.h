#ifndef COMMON_TYPES_CONSTANTS_H
#define COMMON_TYPES_CONSTANTS_H

#include <cstddef>
#include <cstdint>

namespace trading::constants {

/* Trade History Limits */
constexpr size_t MaxInMemoryTrades = 10000;
constexpr size_t MaxTradesPerSymbol = 1000;

/* Order Book Limits */
constexpr size_t MaxOrdersPerPriceLevel = 500;
constexpr size_t MaxPriceLevels = 1000;

/* Performance Tuning*/
constexpr size_t OrderPoolSize = 50000;

constexpr uint64_t MaxPrice = 99999999;

} // namespace trading::constants

#endif
