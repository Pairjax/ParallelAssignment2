#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <chrono>
#include <algorithm>
#include <thread>
#include <queue>

#define NUM_GUESTS 100
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
 * until a flag is raised for them to go (int holding guest #).
 * Then, after they are done with their visit, they calculate
 * the next thread's starting point. We also ignore
 * any guests other than the eater that have already seen the
 * lack of cupcake to save computational time.
 */

bool eaterHasSeenNCupcakes = false; // flag for Eater's completion.
bool hasCupcake = true;
bool hasVisited [NUM_GUESTS]; // test to confirm accuracy
bool hasSeenNoCupcake [NUM_GUESTS]; // test to confirm accuracy
int currentGuest;
int numEaterVisitors = 0;

void run_guest_thread(int guestIndex) {
  srand(time(0)); // Refresh seed else generate the same number

  bool firstTimeWithoutCupcake = true;

  // Run thread perpetually until success condition
  while (true) {
    while (currentGuest != guestIndex) {
      std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    if (guestIndex == EATER_GUEST) { // Eater guest
      if (hasCupcake) {
        hasCupcake = false;
        numEaterVisitors += 1;
      }

      if (numEaterVisitors >= NUM_GUESTS) {
        // Flag for exit condition
        eaterHasSeenNCupcakes = true;

        hasVisited[guestIndex] = true;
        return;
      }
    } else { // Regular guest
      if (!hasCupcake) {
        hasSeenNoCupcake[guestIndex] = true;
        hasCupcake = true;

        // Get new guest & notify them
        int newGuest = rand() % NUM_GUESTS;

        while (hasSeenNoCupcake[newGuest]) {
          newGuest = rand() % NUM_GUESTS;
        }

        currentGuest = newGuest;

        hasVisited[guestIndex] = true;
        return;
      }
    }

    // Get new guest & notify them
    int newGuest = rand() % NUM_GUESTS;

    while (hasSeenNoCupcake[newGuest]) {
      newGuest = rand() % NUM_GUESTS;
    }

    currentGuest = newGuest;
  }
}

int main() {
  std::fill_n(hasVisited, NUM_GUESTS, false);
  std::fill_n(hasSeenNoCupcake, NUM_GUESTS, false);
  std::thread guestThreads[NUM_GUESTS];
  auto begin_execution = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < NUM_GUESTS; i++) {
    guestThreads[i] = std::thread(run_guest_thread, i);
  }

  // Notify first guest they should visit
  int newGuest = rand() % NUM_GUESTS;
  currentGuest = newGuest;

  for (auto& thread : guestThreads) {
    thread.join();
  }

  auto stop_execution = std::chrono::high_resolution_clock::now();
  auto execution_time = std::chrono::duration_cast<std::chrono::microseconds>(
      stop_execution - begin_execution);

  std::cout << "Time Took: " << execution_time.count() << std::endl;

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