# ideas: cache of numbers that have already been verified to be "magic"


from time import perf_counter


def is_magic_number(x):
    if x == 0:
        return False
    elif x == 1:
        return True
    elif x % 2:
        # number is odd
        return is_magic_number(x * 3 + 1)
    else:
        # number is even
        return is_magic_number(x / 2)


start = perf_counter()
for i in range(1, 5000000000):
    # start2 = perf_counter()
    is_magic = is_magic_number(i)
    # end2 = perf_counter()
    # print(f"Calculation took {end2 - start2:.6f} seconds (num {i})")
    # print(f"{i}: {is_magic}")
    if not is_magic:
        raise Exception(f"Not magic: '{i}'")
end = perf_counter()
delta = end - start
print("All numbers are magic")
print(f"Calculation took {delta:.6f} seconds")
