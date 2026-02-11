#include <chrono>
#include <iomanip>
#include <ios>
#include <iostream>
#include <numeric>
using std::cout;
using std::endl;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const int MAX_TRIES = 100000000;
const int UPDATE_INTERVAL = 1000000000;
const unsigned long int END = 5000000000;
const int NUM_TESTS = 20;

unsigned long int lastVerifiedNumber = -1;

bool isMagicNumber(unsigned long x) {
  if (x <= 0) {
    return false;
  }

  if (x == 1) {
    return true;
  }

  int tries = 0;
  while (x != 1) {
    if (x & 1) {
      // number is odd, multiply by 3 and add 1
      x = x * 3 + 1;
    } else {
      // number is even, divide by 2
      x = x >> 1;
    }

    if (x <= lastVerifiedNumber) {
      // all numbers before the last verified number have been validated as
      // "magic"
      return true;
    }

    tries++;
    if (tries >= MAX_TRIES) {
      cout << "Hit max try limit (" << MAX_TRIES << ")" << endl;
      return false;
    }
  }

  return true;
}

bool calculateAllNumbers() {
  for (unsigned long int i = 1; i <= END; i++) {
    if (!isMagicNumber(i)) {
      cout << i << " is not a magic number." << endl;
      return false;
    }
    lastVerifiedNumber = i;
  }
  return true;
}

int main() {
  float times[NUM_TESTS];

  for (int i = 0; i < NUM_TESTS; i++) {
    lastVerifiedNumber = -1;

    cout << "\nRunning test #" << i + 1 << "..." << endl;
    auto start_time = high_resolution_clock::now();
    bool result = calculateAllNumbers();
    auto end_time = high_resolution_clock::now();
    auto ms = duration_cast<milliseconds>(end_time - start_time);

    float seconds = ms.count() / 1000.0;
    cout << "Completed in " << seconds
         << " seconds. Result (0=failed, 1=succeeded): " << result << endl;

    times[i] = seconds;
  }

  float sum = std::accumulate(std::begin(times), std::end(times), 0);
  float average = sum / NUM_TESTS;

  cout << "Average execution time: " << average << endl;

  // Average execution time: 20.05

  return 0;
}
