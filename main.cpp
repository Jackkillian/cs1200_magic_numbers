#include <atomic>
#include <chrono>
#include <iomanip>
#include <ios>
#include <iostream>
#include <thread>
#include <unordered_map>
using std::atomic;
using std::atomic_flag;
using std::cout;
using std::endl;
using std::memory_order_acquire;
using std::memory_order_release;
using std::thread;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const int MAX_TRIES = 100000000;

atomic<bool> FOUND_NON_MAGIC(false);
atomic_flag flag = ATOMIC_FLAG_INIT;

bool isMagicNumber(unsigned long int x) {
  if (x <= 0) {
    return false;
  }

  if (x == 1) {
    return true;
  }

  int tries = 0;
  unsigned long int original = x;
  while (x != 1) {
    if (x & 1) {
      // number is odd
      x = x * 3 + 1;
    } else {
      // number is even
      x = x >> 1;
    }

    tries++;
    if (tries >= MAX_TRIES) {
      cout << "Found non-magic number (or hit limit): " << original << endl;
      FOUND_NON_MAGIC.store(true);
      return false;
    }
  }
  return true;
}

void workerFunc(int threadNumber, unsigned long startOffset,
                unsigned long step) {
  for (unsigned long int i = 0; i <= step; i++) {
    unsigned long number = i + startOffset;
    if (!isMagicNumber(number)) {
      cout << number << " is not a magic number." << endl;
      return;
    } else if (number % 10000000 == 0) {
      cout << number << " is a magic number! (" << std::fixed
           << std::setprecision(1) << static_cast<float>(i) / step * 100
           << "\% done on thread #" << threadNumber << ")" << endl;
    }
  }
}

int main() {
  const unsigned long int END = 5000000000;
  // const unsigned long int END = 500000000;
  const int NUM_THREADS = 10;
  const unsigned long int STEP = END / NUM_THREADS;
  thread threads[NUM_THREADS];

  cout << "Calculated number of threads: " << NUM_THREADS << endl;
  cout << "Calculated steps: " << STEP << endl;
  cout << "Calculating from 1 to " << END << "...\n" << endl;

  auto start_time = high_resolution_clock::now();
  for (int i = 0; i < NUM_THREADS; i++) {
    unsigned long int startVal = STEP * i + 1;
    cout << "Starting thread #" << i + 1 << " for [" << startVal << ", "
         << startVal + STEP - 1 << "]" << endl;
    threads[i] = thread(workerFunc, i + 1, startVal, STEP);
  }
  cout << endl;

  for (int i = 0; i < NUM_THREADS; i++) {
    threads[i].join();
  }

  auto end_time = high_resolution_clock::now();
  auto ms = duration_cast<milliseconds>(end_time - start_time);

  cout << "Are all numbers magic? (1=True, 0=False): " << !FOUND_NON_MAGIC
       << endl;
  cout << "Completed in " << ms.count() / 1000.0 << " seconds" << endl;
  cout << "All numbers from 1 to " << END << " are magic!" << endl;
  return 0;
}
