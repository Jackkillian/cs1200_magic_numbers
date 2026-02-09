#include <chrono>
#include <iostream>
#include <unordered_map>
using std::cout;
using std::endl;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const unsigned long int CACHE_LIMIT = 100000000;
std::unordered_map<unsigned long int, bool> cache;

bool isMagicNumber(unsigned long int x) {
  if (x == 1) {
    return true;
  }

  if (cache.count(x) != 0) {
    return true;
  }

  unsigned long int original = x;
  while (x != 1) {
    if (x & 1) {
      // number is odd
      x = x * 3 + 1;
    } else {
      // number is even
      x = x >> 1;
    }

    if (cache.count(x) != 0 && original <= CACHE_LIMIT) {
      cache[original] = true;
      return true;
    }
  }
  if (original <= CACHE_LIMIT) {
    cache[original] = true;
  }
  return true;
}

int main() {
  unsigned long int end = 5000000000;
  // unsigned long int end = 50000000;
  //  unsigned long int end = 5000000000000000000;

  // cout << std::numeric_limits<unsigned long int>::max() << endl;

  // bool v = isMagicNumber(end);
  // cout << "Is " << end << " magic? " << v << endl;
  // return 0;

  auto start_time = high_resolution_clock::now();
  for (unsigned long int i = 1; i <= end; i++) {
    if (!isMagicNumber(i)) {
      cout << i << " is not a magic number." << endl;
      return 1;
      // }
    } else if (i % 10000000 == 0) {
      cout << i << " is a magic number!\n";
    }
  }
  // cout << endl;
  auto end_time = high_resolution_clock::now();
  auto ms = duration_cast<milliseconds>(end_time - start_time);

  cout << "Completed in " << ms.count() / 1000.0 << " seconds" << endl;
  cout << "All numbers from 1 to " << end << " are magic!" << endl;
  return 0;
}
