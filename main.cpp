#include <chrono>
#include <iomanip>
#include <ios>
#include <iostream>
using std::cout;
using std::endl;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const int MAX_TRIES = 100000000;
unsigned long int lastVerifiedNumber = -1;
const int UPDATE_INTERVAL = 100000000;

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

int main() {
  const unsigned long int END = 5000000000;
  // const unsigned long int END = 500000000;
  cout << "Calculating from 1 to " << END << "...\n" << endl;

  auto start_time = high_resolution_clock::now();

  for (unsigned long int i = 1; i <= END; i++) {
    if (!isMagicNumber(i)) {
      cout << i << " is not a magic number." << endl;
      return 1;
    } else if (i % UPDATE_INTERVAL == 0) {
      cout << i << " is a magic number! (" << std::fixed << std::setprecision(1)
           << static_cast<float>(i) / END * 100 << "\% done)" << endl;
    }
    lastVerifiedNumber = i;
  }

  auto end_time = high_resolution_clock::now();
  auto ms = duration_cast<milliseconds>(end_time - start_time);

  cout << "Completed in " << ms.count() / 1000.0 << " seconds" << endl;
  cout << "All numbers from 1 to " << END << " are magic!" << endl;
  return 0;
}
