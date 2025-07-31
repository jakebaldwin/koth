#include <gtest/gtest.h>

#include "symbol_table/symbol_table.h"

class SymbolTableTest : public ::testing ::Test {
 protected:
  void SetUp() override { symbol_table_ = std::make_unique<SymbolTable>(); }

  void TearDown() override { symbol_table_.reset(); }

  std::unique_ptr<SymbolTable> symbol_table_;
};

TEST_F(SymbolTableTest, AddSymbolAndRetrieve) {
  std::string test_symbol = "AAPL";

  symbol_table_->AddSymbol(test_symbol);

  uint32_t retrieved_id = symbol_table_->GetSymbolIdSafe(test_symbol);
  EXPECT_NE(retrieved_id, SymbolTable::kInvalidSymbolId);
  EXPECT_EQ(retrieved_id, 1);

  const std::string& retrieved_symbol =
      symbol_table_->GetSymbolString(retrieved_id);

  EXPECT_EQ(retrieved_symbol, test_symbol);

  EXPECT_EQ(symbol_table_->GetSymbolCount(), 1);
  EXPECT_FALSE(symbol_table_->IsEmpty());
}

TEST_F(SymbolTableTest, GetInvalidSymbol) {
  uint32_t result = symbol_table_->GetSymbolIdSafe("NONEXISTENT");
  EXPECT_EQ(result, SymbolTable::kInvalidSymbolId);
}

TEST_F(SymbolTableTest, AddDuplicateSymbol) {
  std::string test_symbol = "AAPL";

  symbol_table_->AddSymbol(test_symbol);
  EXPECT_EQ(symbol_table_->GetSymbolCount(), 1);
  symbol_table_->AddSymbol(test_symbol);
  EXPECT_EQ(symbol_table_->GetSymbolCount(), 1);
}

TEST_F(SymbolTableTest, EmptySymbolTable) {
  EXPECT_TRUE(symbol_table_->IsEmpty());
  EXPECT_EQ(symbol_table_->GetSymbolCount(), 0);
}
