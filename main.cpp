#include <chrono>
#include <iostream>
using std::cout;
using std::endl;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const int MAX_TRIES = 100000000;
const unsigned long int END = 5000000000;
unsigned long int lastVerifiedNumber = -1;

bool isMagicNumber(unsigned long x) {
  if (x == 1) {
    return true;
  }

  while (x != 1) {
    if (x & 1) {
      // number is odd, multiply by 3 and add 1
      // also, immediately divide by two: we know the number has to be even now,
      // so we can perform the next step without doing all of the comparisons
      // again
      x = (x * 3 + 1) >> 1;
    } else {
      // number is even, divide by 2
      x = x >> 1;
    }

    if (x <= lastVerifiedNumber) {
      // all numbers before the last verified number have been validated as
      // "magic"
      return true;
    }
  }

  return true;
}

bool calculateAllNumbers(unsigned long int endVal) {
  for (unsigned long int i = 1; i <= endVal; i++) {
    if (!isMagicNumber(i)) {
      cout << i << " is not a magic number." << endl;
      return false;
    } else if ((i & 0xFFFFFFF) == 0) {
      //  trigger only in large intervals
      cout << "Update: " << i << " is magic." << endl;
    }
    lastVerifiedNumber = i;
  }
  return true;
}

int main() {
  cout << "Verifying numbers from 1 to " << END << "..." << endl;
  auto start_time = high_resolution_clock::now();
  bool result = calculateAllNumbers(END);
  auto end_time = high_resolution_clock::now();
  auto ms = duration_cast<milliseconds>(end_time - start_time);

  double seconds = ms.count() / static_cast<double>(1000);
  long long milli = ms.count();
  cout << "Completed in " << milli
       << " milliseconds. Result: " << (result ? "Success" : "Failed") << " ("
       << seconds << " seconds)" << endl;

  return 0;
}
