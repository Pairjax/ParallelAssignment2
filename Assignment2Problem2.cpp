#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <chrono>
#include <algorithm>
#include <thread>
#include <mutex>
#include <queue>

/*
 * The solution asumes the third strategy
 * is the best option. Doing this places each
 * guest in a FIFO queue, where each guest then
 * views the vase one at a time, before notifying
 * the next guest by removing itself from the
 * queue.
 *
 * Each thread has an associated ID, where they
 * wait for their thread's ID to appear.
 */

#define NUM_GUESTS 30

std::queue<int> guestQueue;

void run_guest_thread(int threadId) {
  guestQueue.push(threadId);

  while(true) {
    if (guestQueue.front() == threadId) {
      std::cout << "Guest " << threadId + 1 << " has viewed the vase!" << std::endl;
      guestQueue.pop();
      return;
    }
  }
}

int main() {
  std::thread guestThreads[NUM_GUESTS];

  for (int i = 0; i < NUM_GUESTS; i++) {
    guestThreads[i] = std::thread(run_guest_thread, i);
  }

  for (auto& thread : guestThreads) {
    thread.join();
  }

  return 0;
}