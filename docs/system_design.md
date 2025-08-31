
### Decisions I Make
- Error codes over exceptions. While less straighforward while debugging, 
exceptions add latency, which is the enemy here.
- OrderBookResult instead of returning individual components. I tried making it
as light as I could. If we need it even faster we can try and change the hot path
to return precisely whats needed.
