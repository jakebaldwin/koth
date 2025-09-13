## Prerequisites

First, lets get out the basic knowledge about multithreading, to establish a base of knowledge and derive the need for lock free programming. 
### Mutex's and Spinlocks

A 'mutex' or 'mutual exclusion lock' is a synchronization primitive that ensures only one thread can access an important section of the code at once. Think of group talk-therapy sessions where the hippie leader introduces a 'talking totem', where only the person holding the talking totem can talk. Similar idea. 

A 'spinlock' is a type of mutex that uses busy-waiting to acquire access to a shared resource, that is, when a thread tries to acquire a spinlock, it spins, or keeps checking if it is available instead of being put to sleep by the OS.

Spinlocks can be advantageous over mutex for short critical sections and low contention scenarios as the overhead of spinning is less than the cost of a thread context switch to a blocked thread.

One common pitfall that should be avoided with mutex's is deadlocking. Thread 1 aquires mutex 1, then tries to acquire mutex 2. Thread 2 at the same time acquires mutex 2 and tries to acquire mutex 1. All execution will halt.
### Semaphores
A semaphore is a synchronization primitive that maintains an internal counter which represents the number of available resources. There are two common types, a binary semaphore which is 0 or 1, essentially a mutex, or a counting semaphore which can go to an arbitrary n.

Threads can perform two operations on a semaphore:
- Wait or P/Decrement. Signifies a resource is being acquired.
- Signal or V/Increment. Signifies that a resource has been released and can unblock a waiting thread.

While a mutex is primarily a locking mechanism for mutual exclusion, ensuring that only one thread can access a block, a semaphore is a signaling mechanism commonly used for task coordination and synchronization, managing access to a pool of resources or coordinating events between threads. A mutex has the concept of ownership, a semaphore does not have ownership. 

