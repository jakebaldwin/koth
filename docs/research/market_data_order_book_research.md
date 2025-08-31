# Market Data Order Book Research

## What is an Order Book?
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
This system is not market making, so the order book does not need to be doing
any matching. It just needs to be thin and track the current state, as the data
comes in from the FIX gateway. Current implementation is just L1, track price 
levels and quantities of buys and asks, and keep BBO data in hot memory. As I
continue development on the rest of the system, I might determine I need more 
data from the market data order book, like L2 or even L3 data.

Considering that this will likely never run on specialty hardware with fiber 
cables, I might not be able to match the real HFT system microsecond speeds. 
However seeing where my L1 only method performs, as compared to a then L2 and
L3 would be interesting, and perhaps I could compare to some popular benchmarks.
:q
