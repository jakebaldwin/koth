# Order Book Manager Research

## What is an Order Book Manager?
An Order Book the systems live snapshot of all current buy and sell orders.
Commonly updated in real time, in memory, with replica A and B holding the same
data, so if one fails, the other can failover to the other.

An order book is a store of buy and sell orders for a specific asset, financial
instrument, security, usually organized by time price priority.

Example: you are looking to buy a ps5 from gamestop. Assume Gamestop will sell to
the highest bidder. If more than one person is willing to pay the highest bid,
game stop will sell to whoever offered that price first.

## L1 Order Book
L1 refers to the highest level of an order book, the best bid or ask for the 
underlying financial asset. L1 Best Bid Offer (BBO) is the cheapest price someone
will sell an asset for, or the highest they are willing to pay for the asset.

## L2 Order Book
L2 (MBP) Market By Price. This expands beyond L1 to show other prices people 
are buying and selling the asset for, seemingly sorted in order of best are at
the top of the lists.

## L3 of the Order Book
L3 (MBO) Market By Order. This level focuses on the queue priority of the elements.
Time Price Priority -> FIFO -> First in First Out.

Prorata -> Even first in queue can be fufilled later, if someone later in the queue
comes in with higher volume.

### Notes
- Order Books can support multiple order types.
- A market order is a message - i am willing to buy or sell immediately for the best price.
- A Limit Order however says - i am interested in buying or selling at this particular price.
- At a given time t, the bid price is defined to be the best available buying limit order,
    while the ask price is the best available selling limit order. Their difference
    makes the 'quoted spread'.
- A security will never have a unique price, but we can compute the MidPrice to 
    be the midpoint between bid and ask. However when volumes come into play, 
    it makes more sense to average them out, vol_bid * price_ask + vol_ask * price_bid all 
    divided by the volume_bid + volume_ask.


### Questions as a product of these research
- What order types should this support?

### Implementation Details
- Might be worth aliasing data types to other things - 'Price - int'
- 'Quantity - int'
- 'Order ID - int' etc

### Claude Generated questions to go and answer.
1. Data Structure & Performance:

What data structure gives O(1) order insertion/deletion? (Hint: price-level maps + order queues)
How do you handle price-time priority efficiently?
Should you use separate bid/ask trees or unified structure?
How do you quickly find best bid/offer after updates?

2. Memory Management:

How do you pre-allocate order book memory to avoid malloc/free in hot path?
What's the maximum depth you need to store? (Top 10 levels? Full book?)
How do you handle order book snapshots for recovery?

3. Update Processing:

How do you apply incremental updates (add/modify/cancel orders)?
What happens when you receive out-of-order messages?
How do you detect and handle corrupted market data?
Do you need to validate order book integrity in real-time?

4. Market Data Levels:

Will you implement all L1/L2/L3, or focus on L2 for market making?
How do you efficiently calculate mid-price, spread, and depth?
Do you need historical order book states for backtesting?

5. Multi-Symbol Coordination:

How many symbols will you track simultaneously? (10? 100? 1000?)
Should each symbol have its own thread or shared processing?
How do you handle symbol-specific configurations (tick sizes, lot sizes)?

6. Integration Points:

How does Pricing Engine subscribe to order book changes?
What events do you publish? (BBO change, depth change, trade?)
How do you handle multiple consumers reading the same book?

Research Suggestions:
Technical Papers:

Look up "ITCH protocol" (NASDAQ's market data format)
Research "Price-Time Priority Matching" algorithms
Study "Memory-Mapped Order Books" implementations

Practical Questions:

How do real exchanges handle order book updates?
What's the typical message rate for a liquid stock? (1000+ updates/second)
How do you benchmark order book performance?

Requirements Categories to Define:

Performance: Update latency, lookup speed, memory usage
Capacity: Max orders per book, max symbols, max depth
Reliability: Data validation, error handling, recovery
API: What methods other components need

Want to dive deeper into any of these areas? The data structure choice is probably the most critical decision for your implementation.RetryClaude can make mistakes. Please double-check responses. Sonnet 4
