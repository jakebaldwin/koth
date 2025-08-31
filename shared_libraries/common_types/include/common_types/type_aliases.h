#ifndef COMMON_TYPES_TYPE_ALIASES_H
#define COMMON_TYPES_TYPE_ALIASES_H

#include <atomic>
#include <cstddef>
#include <cstdint>

namespace trading::types {

/* Financial Types */
using Price = uint64_t;    // Price in smallest units (e.g. cents)
using Quantity = uint64_t; // Number of shares/contracts
using OrderId = uint64_t;  // Unique order identifier
using TradeId = uint64_t;  // Unique trade identifier
using SymbolId = uint64_t; // Symbol identifier
using ClientId = uint64_t; // Client / trader identifier

/* Time Types */
using Timestamp = uint64_t;      // Nanoseconds since epoch
using SequenceNumber = uint64_t; // For message ordering

/* Volume/Size Types */
using Volume = uint64_t; // Total Volume
using Count = size_t;    // Number of orders, trades, etc.

} // namespace trading::types

#endif
