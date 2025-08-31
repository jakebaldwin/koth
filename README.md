# KOTH (King of the Hill) - Ultra-Low Latency Trading Engine

## Overview
KOTH is a high-performance algorithmic trading system designed for market making
and arbitrage strategies. Built in C++ with sub-10 microsecond latency targets,
it demonstrates enterprise-grade trading infrastructure suitable for institutional
environments.

## System Goals
- Ultra-low latency order execution (<10us order-to-market)
- Real-time market making with dynamic spread calculation
- Cross-venue arbitrage detection and execution
- Comprehensive risk management with emergency controls

## High Level Architecture
- Market data ingestion via FIX protocol
- Lock-free data structures for zero-copy processing
- Real-time risk engine with kill switch capabilities
- Memory-mapped storage for consistent performance

## Key Technologies
- Design and Research Phase

## Documentation
- [System Design](docs/system_design.md) - Detailed Architecture
- [Requirements](docs/requirements.md) - Performance and Functional Specs
- [Database Schema](docs/database/) - Data models and migrations

## Architecture Diagram
![Architecture Diagram](https://jakebaldwin.github.io/koth/docs/system_design.html)
