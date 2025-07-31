#include "symbol_table/symbol_table.h"

#include <iostream>
#include <stdexcept>

uint32_t SymbolTable::GetSymbolIdUnsafe(const std::string& symbol) const {
  return string_to_id_.at(symbol);
}

uint32_t SymbolTable::GetSymbolIdSafe(const std::string& symbol) const {
  auto it = string_to_id_.find(symbol);

  if (it == string_to_id_.end()) {
    // TODO - log error
    return kInvalidSymbolId;
  }
  return it->second;
}

const std::string& SymbolTable::GetSymbolString(uint32_t id) const {
  auto it = id_to_string_.find(id);

  if (it == id_to_string_.end()) {
    static const std::string empty_string;
    return empty_string;
  }

  return it->second;
}

void SymbolTable::LoadFromFile(const std::string& config_file) {
  std::cout << "config file: " << config_file << std::endl;
  // TODO - implement this method
}

void SymbolTable::AddSymbol(const std::string& symbol) {
  auto it = string_to_id_.find(symbol);

  if (it == string_to_id_.end()) {
    id_to_string_[next_available_id_] = symbol;
    string_to_id_[symbol] = next_available_id_++;
  }
}
