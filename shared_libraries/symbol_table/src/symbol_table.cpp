#include "symbol_table/symbol_table.h"

#include <iostream>

using namespace trading::types;

SymbolId SymbolTable::getSymbolIdUnsafe(const std::string &symbol) const {
  return string_to_id_.at(symbol);
}

std::optional<SymbolId>
SymbolTable::getSymbolIdSafe(const std::string &symbol) const {
  auto it = string_to_id_.find(symbol);
  return (it != string_to_id_.end()) ? std::optional{it->second} : std::nullopt;
}

const std::string &SymbolTable::getSymbolString(SymbolId id) const {
  auto it = id_to_string_.find(id);

  if (it == id_to_string_.end()) {
    static const std::string empty_string;
    return empty_string;
  }

  return it->second;
}

void SymbolTable::loadFromFile(const std::string &config_file) {
  std::cout << "config file: " << config_file << "\n";
  // TODO - implement this method
}

void SymbolTable::addSymbol(const std::string &symbol) {
  auto it = string_to_id_.find(symbol);

  if (it == string_to_id_.end()) {
    id_to_string_[next_available_id_] = symbol;
    string_to_id_[symbol] = next_available_id_++;
  }
}
