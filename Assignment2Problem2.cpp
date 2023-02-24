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

#define NUM_GUESTS 100

std::queue<int> guestQueue;

void run_guest_thread(int threadId) {
  guestQueue.push(threadId);

  while(true) {
    if (guestQueue.front() == threadId) {
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

  auto begin_execution = std::chrono::high_resolution_clock::now();

  for (auto& thread : guestThreads) {
    thread.join();
  }

  auto stop_execution = std::chrono::high_resolution_clock::now();
  auto execution_time = std::chrono::duration_cast<std::chrono::microseconds>(
      stop_execution - begin_execution);

  std::cout << "All guests have viewed the vase in " << execution_time.count() << " microseconds!" << std::endl;

  return 0;
}