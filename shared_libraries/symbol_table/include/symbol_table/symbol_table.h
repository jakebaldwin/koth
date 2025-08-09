#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "common_types/type_aliases.h"

#include <cstdint>
#include <string>
#include <unordered_map>

// symbol_table.h

/// High performance symbol table lookup class. This stores string symbol IDS
/// as uint32_t for faster lookup as opposed to string comparison. It provides
/// lock free lookup operations
class SymbolTable {
public:
  /// Default Constructor
  SymbolTable() {};

  /// Copy Constructor
  SymbolTable(const SymbolTable &other) = delete;

  /// Copy Assignment Operator
  SymbolTable &operator=(const SymbolTable &other) = delete;

  /// Move Constructor
  SymbolTable &operator=(SymbolTable &&other) noexcept = delete;

  /// Move Assignment Operator
  SymbolTable(SymbolTable &&other) noexcept = delete;

  /// Destructor
  ~SymbolTable() = default;

  /// Returns the symbol ID for the given symbol string without bounds checking
  /// \param symbol The symbol ID string to lookup
  /// \return The numeric uint32_t id for the string, or undefined if DNE
  /// \pre the symbolid string must exist in the table
  trading::types::SymbolId getSymbolIdUnsafe(const std::string &symbol) const;

  /// Returns the symbol ID for the given symbol string with validation
  /// \param symbol The symbol ID string to lookup
  /// \return The numeric uint32_t id for the string
  /// \post Return is either valid ID or invalid ID symbol
  std::optional<trading::types::SymbolId>
  getSymbolIdSafe(const std::string &symbol) const;

  /// Returns the symbol string for the given symbol ID with validation
  /// \param id the symbol ID to lookup
  /// \return the string symbol id for the given numeric id
  /// \post Return is either valid symbolID or empty string
  const std::string &getSymbolString(trading::types::SymbolId id) const;

  /// Populates the symbol id lookup from a configuration file
  /// \param config_file relative path to config file containing symbols
  /// \post symbol id maps are populated with unique ids for all in file
  void loadFromFile(const std::string &config_file);

  /// Insert a single symbol into table
  /// \param symbol string symbol id to insert into map
  /// \post symbol is in map with unique id
  void addSymbol(const std::string &symbol);

  /// Returns size of table
  size_t getSymbolCount() const { return string_to_id_.size(); }

  bool isEmpty() const { return string_to_id_.empty(); }

private:
  std::unordered_map<std::string, trading::types::SymbolId> string_to_id_;
  std::unordered_map<trading::types::SymbolId, std::string> id_to_string_;

  trading::types::SymbolId next_available_id_ = 1;
};

#endif
