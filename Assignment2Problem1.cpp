#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <chrono>
#include <algorithm>
#include <thread>
#include <mutex>

#define NUM_GUESTS 10
#define EATER_GUEST 0

/*
 * The solution assumes that one guest
 * is the "eater" guest. This guest always eats the
 * cupcake. Every other guest will ask for the cupcake
 * to be replenished, but only the first time it is
 * missing. Thus, when the "eater" has seen N replenished
 * cupcakes, they can assume everyone has visited.
 *
 * In order to achieve this, each thread loops infinitely
 * until a flag is raised for them to go (held in a
 * boolean array). Then, after they are done with their visit,
 * they calculate the next thread's starting point.
 */

bool eaterHasSeenNCupcakes = false; // flag for Eater's completion.
bool hasCupcake = true;
bool hasVisited [NUM_GUESTS]; // test to confirm accuracy
bool timeToVisit [NUM_GUESTS]; // test to confirm accuracy
int numEaterVisitors = 0;
std::mutex mtx;

void run_guest_thread(int guestIndex) {
  srand(time(0)); // Refresh seed else generate the same number

  bool firstTimeWithoutCupcake = true;

  // Run thread perpetually until success condition
  while (true) {
    // Waiting for next visit
    while (!timeToVisit[guestIndex]) {
      // Exit condition
      if (eaterHasSeenNCupcakes) {
        return;
      }

      continue;
    }

    timeToVisit[guestIndex] = false;
    hasVisited[guestIndex] = true;

    if (guestIndex == EATER_GUEST) { // Eater guest
      if (hasCupcake) {
        hasCupcake = false;
        numEaterVisitors += 1;
      }

      if (numEaterVisitors >= NUM_GUESTS) {
        // Flag for exit condition
        eaterHasSeenNCupcakes = true;
        return;
      }
    } else { // Regular guest
      if (!hasCupcake && firstTimeWithoutCupcake) {
        firstTimeWithoutCupcake = false;
        hasCupcake = true;
      }
    }

    // Get new guest & notify them
    int newGuest = rand() % NUM_GUESTS;
    timeToVisit[newGuest] = true;
  }
}

int main() {
  std::fill_n(hasVisited, NUM_GUESTS, false);
  std::fill_n(timeToVisit, NUM_GUESTS, false);
  std::thread guestThreads[NUM_GUESTS];

  for (int i = 0; i < NUM_GUESTS; i++) {
    guestThreads[i] = std::thread(run_guest_thread, i);
  }

  // Notify first guest they should visit
  int newGuest = rand() % NUM_GUESTS;
  timeToVisit[newGuest] = true;

  for (auto& thread : guestThreads) {
    thread.join();
  }

  if (std::all_of(
          std::begin(hasVisited),
          std::end(hasVisited),
          [](bool i) { return i; }
          )) {
    std::cout << "Success! All guests have visited.";
  } else {
    std::cout << "Failure! Not all guests have visited.";
  }

  return 0;
}