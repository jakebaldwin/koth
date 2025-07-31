# Koth Requirements Document

## Table of Contents
1. [Requirements](#performance-requirements)
2. [Risk Limits and Safety Requirements](#risk-limits-and-safety-requirements)
3. [System Avaialability Targets](#system-availability-targets)
4. [Memory / CPU Constraints](#memory-cpu-constraints)
5. [Service Specific Requirements](#service-specific-requirements)
5.1 [Order Book Requirements](#order-book-requirements)

## Performance Requirements

## Risk Limits and Safety Requirements

## System Availability Targets

## Memory / CPU Constraints

## Service Specific Requirements


# Order Book Requirements (3-6 Month Solo Dev Scope)

#### OB-001: Performance Requirements
* **OB-001.1**: Order insertion must complete in < 500 nanoseconds (99th percentile)
* **OB-001.2**: Best bid/offer lookup must be O(1) constant time
* **OB-001.3**: Order cancellation must complete in < 250 nanoseconds (99th percentile)
* **OB-001.4**: Memory allocation during trading hours: minimal (use object pools)
* **OB-001.5**: L1 data updates must be published within 100 nanoseconds of book change
* **OB-001.6**: L2 snapshots must be generated in < 10 microseconds

#### OB-002: Capacity Requirements
* **OB-002.1**: Support minimum 10,000 active orders per symbol
* **OB-002.2**: Handle 10,000+ order updates per second per symbol
* **OB-002.3**: Track order book depth to 100 price levels per side
* **OB-002.4**: Support concurrent access from 4 threads maximum (2 read, 2 write)
* **OB-002.5**: Handle up to 100 symbols simultaneously
* **OB-002.6**: Support burst traffic of 50K orders/second for 30 seconds

#### OB-003: Functional Requirements
##### Core Matching Engine
* **OB-003.1**: Maintain strict price-time priority for order matching
* **OB-003.2**: Support order types: Limit, Market, IOC (Immediate or Cancel)
* **OB-003.3**: Handle partial fills with remaining quantity management
* **OB-003.4**: Support basic order modification (quantity reduction only)
* **OB-003.5**: Validate order integrity (positive quantities, valid prices, tick size)

##### Market Data Generation
* **OB-003.6**: Generate L1 data (BBO, last trade, total volume)
* **OB-003.7**: Generate L2 data (price-level aggregated depth, top 20 levels)
* **OB-003.8**: Publish trade execution data with basic fill details
* **OB-003.9**: Calculate simple market statistics (spread, mid-price)

##### Order Lifecycle Management
* **OB-003.10**: Support order states: New, Partial, Filled, Cancelled, Rejected
* **OB-003.11**: Implement FIFO queues with millisecond timestamp resolution
* **OB-003.12**: Handle basic order expiration (session-based GTC only)

#### OB-004: Data Requirements
##### Order Data Model
* **OB-004.1**: Store order ID (64-bit), symbol, price, quantity, timestamp, side
* **OB-004.2**: Track order type and time-in-force flags
* **OB-004.3**: Maintain simple order modification history
* **OB-004.4**: Store client ID for order attribution

##### Market Data Model
* **OB-004.5**: Support price precision up to 4 decimal places with fixed tick size
* **OB-004.6**: Maintain bid/ask depth with quantity aggregation by price level
* **OB-004.7**: Store recent trade history (last 1000 trades per symbol)
* **OB-004.8**: Track basic OHLCV data for current session

##### Recovery & Persistence
* **OB-004.9**: Implement simple checkpoint mechanism (file-based snapshots)
* **OB-004.10**: Maintain basic audit log for order events
* **OB-004.11**: Support cold restart from checkpoint files

#### OB-005: Interface Requirements
##### Core API
* **OB-005.1**: Provide C++ API for order operations (Add, Cancel, Modify)
* **OB-005.2**: Support single-order operations only (no bulk)
* **OB-005.3**: Implement callback system for order book events
* **OB-005.4**: Provide synchronous operation mode

##### Market Data Interfaces
* **OB-005.5**: Publish L1 data feeds via callbacks (BBO, trades)
* **OB-005.6**: Publish L2 market depth via callbacks (top 20 levels)
* **OB-005.7**: Support market data snapshot requests
* **OB-005.8**: Provide basic calculated fields (mid-price, spread)

##### Query Interface
* **OB-005.9**: Support order lookup by ID or symbol
* **OB-005.10**: Provide current order book state queries
* **OB-005.11**: Enable basic trade history queries (current session)

#### OB-006: Threading & Concurrency Requirements
* **OB-006.1**: Use standard mutexes for thread safety (reader-writer locks)
* **OB-006.2**: Implement atomic operations for performance counters
* **OB-006.3**: Support single-threaded mode for development/testing
* **OB-006.4**: Use thread-safe STL containers where appropriate

#### OB-007: Error Handling & Resilience
* **OB-007.1**: Implement input validation with standard error codes
* **OB-007.2**: Provide basic logging with configurable levels (INFO, WARN, ERROR)
* **OB-007.3**: Handle memory exhaustion with graceful order rejection
* **OB-007.4**: Support basic integrity checks on startup

#### OB-008: Configuration & Monitoring
* **OB-008.1**: Support configuration via JSON/YAML files
* **OB-008.2**: Provide basic performance metrics (orders/sec, latency percentiles)
* **OB-008.3**: Implement simple health status reporting
* **OB-008.4**: Support symbol-specific tick size configuration

#### OB-009: Testing Requirements
* **OB-009.1**: Implement unit tests with 80%+ code coverage
* **OB-009.2**: Support basic load testing with configurable scenarios
* **OB-009.3**: Provide order book state validation tools
* **OB-009.4**: Include performance benchmark suite

#### Implementation Roadmap

##### Month 1-2: Core Foundation
**Week 1-2: Data Structures**
- Order structure and memory pools
- Price-level containers (std::map initially, optimize later)
- Basic symbol management

**Week 3-4: Basic Matching**
- Limit order insertion and matching
- Simple FIFO price-time priority
- Order cancellation

**Week 5-6: Market Data L1**
- BBO calculation and callbacks
- Trade event generation
- Basic validation and error handling

**Week 7-8: Testing & Refinement**
- Unit test suite development
- Basic performance testing
- Bug fixes and optimization

##### Month 3-4: Enhanced Features
**Week 9-10: Market & IOC Orders**
- Market order implementation
- IOC order type support
- Enhanced order validation

**Week 11-12: L2 Market Data**
- Depth aggregation by price level
- L2 snapshot generation
- Market statistics (spread, mid-price)

**Week 13-14: Order Modification**
- Quantity reduction support
- Order state management
- Modification event handling

**Week 15-16: Multi-Symbol Support**
- Symbol container and management
- Per-symbol configuration
- Resource isolation

##### Month 5-6: Production Readiness
**Week 17-18: Persistence**
- File-based checkpointing
- Restart/recovery mechanism
- Basic audit logging

**Week 19-20: Performance Optimization**
- Memory pool optimization
- Lock contention reduction
- Latency measurement and tuning

**Week 21-22: Monitoring & Config**
- JSON configuration system
- Performance metrics collection
- Health check implementation

**Week 23-24: Final Testing & Documentation**
- Load testing and stress testing
- Performance benchmarking
- API documentation and examples

#### Key Simplifications Made

**Removed Complex Features:**
- L3 order-by-order tracking (significant complexity)
- Advanced order types (Stop, Stop-Limit, Iceberg)
- Hot-standby replication
- FIX protocol compliance
- NUMA optimization
- Lock-free data structures (use standard locks initially)

**Reduced Scope:**
- Single exchange simulation (no cross-venue)
- Session-based only (no overnight persistence)
- Simplified threading model
- Basic recovery (no real-time failover)
- Standard STL containers (optimize later if needed)

**Maintained Core Value:**
- Sub-microsecond performance for critical operations
- L1/L2 market data generation
- Production-quality error handling
- Comprehensive testing
- Clean, extensible architecture

