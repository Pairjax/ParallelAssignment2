#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <chrono>
#include <algorithm>
#include <thread>
#include <mutex>
#include <queue>

#define NUM_GUESTS 10

std::queue<int> guestQueue;

void run_guest_thread() {

}

int main() {
  std::thread guestThreads[NUM_GUESTS];

  for (int i = 0; i < NUM_GUESTS; i++) {
    guestThreads[i] = std::thread(run_guest_thread);
  }
  
  for (auto& thread : guestThreads) {
    thread.join();
  }

  return 0;
}