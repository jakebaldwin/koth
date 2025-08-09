// Compile test for common_types headers
// This ensures all headers compile correctly and have no missing dependencies

#include "common_types/constants.h"
#include "common_types/enums.h"
#include "common_types/market_data.h"
#include "common_types/order.h"
#include "common_types/trade.h"
#include "common_types/type_aliases.h"

#include <iostream>

int main() {
  // Simple compilation test - just instantiate some basic types
  std::cout << "Common types headers compiled successfully!" << "\n";

  // You can add basic validation here if needed:
  // Order order{};
  // Trade trade{};
  // MarketData market_data{};

  return 0;
}
