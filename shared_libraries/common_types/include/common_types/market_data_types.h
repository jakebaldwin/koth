#ifndef COMMON_TYPES_ORDER_TYPES_H
#define COMMON_TYPES_ORDER_TYPES_H

#include <list>

#include "common_types/enums.h"
#include "common_types/error_codes.h"
#include "common_types/type_aliases.h"

enum UpdateType { New, Cancel };

struct OrderUpdate {
  Side side;
  trading::types::Price price;
  trading::types::Quantity quantity;
  UpdateType type;
};

struct Trade {
  trading::types::Price price;
  trading::types::Quantity quantity;
  Side aggressor_side;
};

#endif
