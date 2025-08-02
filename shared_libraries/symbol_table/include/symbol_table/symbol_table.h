#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include "common_types/type_aliases.h"

class SymbolTable {
 public:
  SymbolTable() : next_available_id_(1) {};
  ~SymbolTable() = default;

  // Returns the symbol ID for the given symbol string without validation
  // REQUIRES: symbol must exist in the table (else undefined behavior)
  SymbolId GetSymbolIdUnsafe(const std::string& symbol) const;

  // Returns the symbol ID for the given symbol string with validation
  // Returns 0 if symbol is not found
  SymbolId GetSymbolIdSafe(const std::string& symbol) const;

  // Returns the symbol string for the given id
  // Returns empty string if ID not found
  const std::string& GetSymbolString(SymbolId id) const;

  void LoadFromFile(const std::string& config_file);
  void AddSymbol(const std::string& symbol);

  size_t GetSymbolCount() const { return string_to_id_.size(); }
  bool IsEmpty() const { return string_to_id_.empty(); }

  static constexpr uint32_t kInvalidSymbolId = 0;

 private:
  std::unordered_map<std::string, SymbolId> string_to_id_;
  std::unordered_map<SymbolId, std::string> id_to_string_;

  uint32_t next_available_id_;
};

#endif
