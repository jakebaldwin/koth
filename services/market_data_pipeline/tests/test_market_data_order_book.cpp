#include "common_types/type_aliases.h"
#include "market_data_pipeline/market_data_order_book.h"
#include "symbol_table/symbol_table.h"
#include <gtest/gtest.h>

using namespace ::trading::types;

class MarketDataOrderBookTest : public ::testing ::Test {
public:
  void SetUp() override {
    std::string aapl_symbol = "AAPL";

    symbol_table_ = std::make_unique<SymbolTable>();

    symbol_table_->addSymbol(aapl_symbol);

    SymbolId aapl_symbol_id = symbol_table_->getSymbolIdUnsafe(aapl_symbol);

    aapl_order_book_ = std::make_unique<MarketDataOrderBook>(aapl_symbol_id);

    populateOrderBook();
  }

  void populateOrderBook() {
    OrderUpdate new_aapl_buy_order = {Side::Buy, 21011, 100, UpdateType::New};
    OrderUpdate new_aapl_sell_order = {Side::Sell, 23511, 775, UpdateType::New};

    aapl_order_book_->processOrderUpdate(new_aapl_buy_order);
    aapl_order_book_->processOrderUpdate(new_aapl_sell_order);

    new_aapl_buy_order = {Side::Buy, 21575, 500, UpdateType::New};
    new_aapl_sell_order = {Side::Sell, 28011, 1000, UpdateType::New};
    aapl_order_book_->processOrderUpdate(new_aapl_buy_order);
    aapl_order_book_->processOrderUpdate(new_aapl_sell_order);

    new_aapl_buy_order = {Side::Buy, 21680, 445, UpdateType::New};
    new_aapl_sell_order = {Side::Sell, 24515, 98, UpdateType::New};
    aapl_order_book_->processOrderUpdate(new_aapl_buy_order);
    aapl_order_book_->processOrderUpdate(new_aapl_sell_order);

    new_aapl_buy_order = {Side::Buy, 22000, 10, UpdateType::New};
    aapl_order_book_->processOrderUpdate(new_aapl_buy_order);
  }

  void TearDown() override {
    symbol_table_.reset();
    aapl_order_book_.reset();
  }

  std::unique_ptr<MarketDataOrderBook> aapl_order_book_;
  std::unique_ptr<SymbolTable> symbol_table_;
  std::optional<SymbolId> aapl_symbol_id_;
};

/** At the start of these test methods the order book looks like the following
 * BUYS: [(210.11, 100), (215.75, 500), (216.80, 445), (220, 10),
 * ---------------------------------------------------------------------------
 * ASKS: (235.11, 775), (245.15, 98), (280.11, 1000)]
 */

/// Test that the populateOrderBook worked as expected
TEST_F(MarketDataOrderBookTest, PopulateOrderBookSuccess) {
  ASSERT_EQ(aapl_order_book_->getBestBidPrice(), 22000);
  ASSERT_EQ(aapl_order_book_->getBestBidQuantity(), 10);

  ASSERT_EQ(aapl_order_book_->getBestAskPrice(), 23511);
  ASSERT_EQ(aapl_order_book_->getBestAskQuantity(), 775);

  ASSERT_EQ(aapl_order_book_->getSpread(), 1511);
  ASSERT_EQ(aapl_order_book_->getMidPrice(), ((22000 + 23511) / 2));
  ASSERT_EQ(aapl_order_book_->getMicroPrice(),
            ((23511 * 10) + (22000 * 775)) / 785);
}

/// Manipulate the order book and test
TEST_F(MarketDataOrderBookTest, NominalOrdersAndTrades) {

  // Increase quantity of best buy
  OrderUpdate new_aapl_buy_order = {Side::Buy, 22000, 100, UpdateType::New};
  aapl_order_book_->processOrderUpdate(new_aapl_buy_order);

  ASSERT_EQ(aapl_order_book_->getBestBidPrice(), 22000);
  ASSERT_EQ(aapl_order_book_->getBestBidQuantity(), 110);

  // New trade someone offers to sell at best buy price
  Trade trade = {22000, 75, Side::Sell};

  aapl_order_book_->processTrade(trade);

  ASSERT_EQ(aapl_order_book_->getBestBidPrice(), 22000);
  ASSERT_EQ(aapl_order_book_->getBestBidQuantity(), 35);

  // New trades clears best buy and one
  trade = {22000, 35, Side::Sell};

  aapl_order_book_->processTrade(trade);

  ASSERT_EQ(aapl_order_book_->getBestBidPrice(), 21680);
  ASSERT_EQ(aapl_order_book_->getBestBidQuantity(), 445);

  OrderUpdate new_aapl_sell_order = {Side::Sell, 23470, 30, UpdateType::New};
  aapl_order_book_->processOrderUpdate(new_aapl_sell_order);

  ASSERT_EQ(aapl_order_book_->getBestAskPrice(), 23470);
  ASSERT_EQ(aapl_order_book_->getBestAskQuantity(), 30);

  ASSERT_EQ(aapl_order_book_->getSpread(), 1790);
  ASSERT_EQ(aapl_order_book_->getMidPrice(), ((23470 + 21680) / 2));
  ASSERT_EQ(aapl_order_book_->getMicroPrice(),
            ((23470 * 445) + (21680 * 30)) / 475);
}
