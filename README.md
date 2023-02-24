# Assignment2
This is a series of journeys through a friendly minoutar's maze! In order to compile this homework, I reccomend using clang to compile.
- Compile problem 1 with: clang Assignment2Problem1.cpp -o problem1.exe
- Run problem 1 with: ./problem1.exe
- Compile problem 2 with: clang Assignment2Problem2.cpp -o problem12exe
- Run problem 2 with: ./problem2.exe


## Problem 1
The solution assumes that one guest is the "eater" guest. This guest always eats the cupcake. Every other guest will ask for the cupcake to be replenished, but only the first time it is missing. Thus, when the "eater" has seen N replenished cupcakes, they can assume everyone has visited.

In order to achieve this, each thread loops infinitely until a flag is raised for them to go (int holding guest #). Then, after they are done with their visit, they calculate the next thread's starting point. We also ignore any guests other than the eater that have already seen the lack of cupcake to save computational time.

## Problem 2
The solution asumes the third strategy is the best option. Doing this places each guest in a FIFO queue, where each guest then views the vase one at a time, before notifying the next guest by removing itself from the queue. Each thread has an associated ID, where they wait for their thread's ID to appear.

I chose this solution because it requires the least amount of waiting, since there can never be a deadlock where two guests can demand to visit it at the same time, and it minimizes the amount of time debating the order of operations - it's simply first come first serve.
