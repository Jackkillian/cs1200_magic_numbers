#include <atomic>
#include <chrono>
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

void workerFunc(unsigned long start, unsigned long end) {
  for (unsigned long int i = start; i <= end; i++) {
    if (!isMagicNumber(i)) {
      cout << i << " is not a magic number." << endl;
      return;
    } else if (i % 10000000 == 0) {
      cout << i << " is a magic number! ("
           << static_cast<float>(i) / end * 100.0 << "\% done)" << endl;
    }
  }
}

int main() {
  const unsigned long int END = 5000000000;
  const int NUM_THREADS = 10;
  const unsigned long int STEP = END / NUM_THREADS;
  thread threads[NUM_THREADS];

  cout << "Calculated number of threads: " << NUM_THREADS << endl;
  cout << "Calculated steps: " << STEP << endl;
  cout << "Calculating from 1 to " << END << "...\n" << endl;

  auto start_time = high_resolution_clock::now();
  for (int i = 0; i < NUM_THREADS; i++) {
    unsigned long int startVal = STEP * i + 1;
    unsigned long int endVal = STEP * (i + 1);
    cout << "Starting thread #" << i << " for [" << startVal << ", " << endVal
         << "]" << endl;
    threads[i] = thread(workerFunc, startVal, endVal);
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
