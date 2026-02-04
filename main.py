import threading
from time import perf_counter

MAX_VAL = 5000000000
NUM_THREADS = 10
STEP = int(MAX_VAL / NUM_THREADS)

lock = threading.RLock()
cache = {}

# notes from first attempt at multithreading with cache:
# swap size hit 22 gb (my laptop hit full storage because of the swap size...) before macos killed it


def is_magic_number(x):
    if x == 0:
        return False
    elif x == 1:
        return True

    with lock:
        if i in cache:
            return True

    if x % 2:
        # number is odd
        return is_magic_number(x * 3 + 1)
    else:
        # number is even
        return is_magic_number(x / 2)


def calculate_numbers(start, end):
    print(f"Starting thread from {start} to {end}")
    start_perf = perf_counter()
    for i in range(start, end + 1):
        is_magic = is_magic_number(i)

        if not is_magic:
            # TODO
            raise Exception(f"Number {i} is not magic")
        else:
            with lock:
                cache[i] = 0
    delta = perf_counter() - start_perf
    print(f"Calculation from {start} to {end} took {delta:.6f} seconds")


threads = []
for i in range(NUM_THREADS):
    start = (STEP * i) + 1
    end = STEP * (i + 1)
    threads.append(
        threading.Thread(target=calculate_numbers, args=(start, end), daemon=True)
    )

for thread in threads:
    thread.start()

print("Calculating...")
start = perf_counter()
for thread in threads:
    thread.join()
end = perf_counter()

print(f"\n\nTotal calculation took {end - start:.6f} seconds")
print(f"Length of cache: {len(cache)} (is equal to max num? {len(cache) == MAX_VAL})")
