#ifndef COMMON_TYPES_CONSTANTS_H
#define COMMON_TYPES_CONSTANTS_H

#include <cstdint>

namespace trading {
namespace constants {

/* Trade History Limits */
constexpr size_t kMaxInMemoryTrades = 10000;
constexpr size_t kMaxTradesPerSymbol = 1000;

/* Order Book Limits */
constexpr size_t kMaxOrdersPerPriceLevel = 500;
constexpr size_t kMaxPriceLevels = 1000;

/* Performance Tuning*/
constexpr size_t kOrderPoolSize = 50000;

}  // namespace constants
}  // namespace trading

#endif
