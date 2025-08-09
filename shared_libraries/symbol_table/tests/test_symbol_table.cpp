#include "common_types/type_aliases.h"
#include "symbol_table/symbol_table.h"

#include <gtest/gtest.h>

using namespace ::trading::types;

class SymbolTableTest : public ::testing ::Test {
public:
  void SetUp() override { symbol_table_ = std::make_unique<SymbolTable>(); }

  void TearDown() override { symbol_table_.reset(); }

  std::unique_ptr<SymbolTable> symbol_table_;
};

TEST_F(SymbolTableTest, AddSymbolAndRetrieve) {
  std::string test_symbol = "AAPL";

  symbol_table_->addSymbol(test_symbol);

  std::optional<SymbolId> retrieved_id =
      symbol_table_->getSymbolIdSafe(test_symbol);
  EXPECT_NE(retrieved_id, std::nullopt);
  EXPECT_EQ(retrieved_id, 1);

  const std::string &retrieved_symbol =
      symbol_table_->getSymbolString(*retrieved_id);

  EXPECT_EQ(retrieved_symbol, test_symbol);

  EXPECT_EQ(symbol_table_->getSymbolCount(), 1);
  EXPECT_FALSE(symbol_table_->isEmpty());
}

TEST_F(SymbolTableTest, GetInvalidSymbol) {
  std::optional<SymbolId> result =
      symbol_table_->getSymbolIdSafe("NONEXISTENT");
  EXPECT_EQ(result, std::nullopt);
}

TEST_F(SymbolTableTest, AddDuplicateSymbol) {
  std::string test_symbol = "AAPL";

  symbol_table_->addSymbol(test_symbol);
  EXPECT_EQ(symbol_table_->getSymbolCount(), 1);
  symbol_table_->addSymbol(test_symbol);
  EXPECT_EQ(symbol_table_->getSymbolCount(), 1);
}

TEST_F(SymbolTableTest, EmptySymbolTable) {
  EXPECT_TRUE(symbol_table_->isEmpty());
  EXPECT_EQ(symbol_table_->getSymbolCount(), 0);
}
