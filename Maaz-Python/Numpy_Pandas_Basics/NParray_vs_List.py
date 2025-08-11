import numpy as np
import os
import time

os.system("cls")
iterations = 10**6

# Comparison between `range` and `np.arange` Comprehension time consumptions

# `np.arange` Comprehension time
start_time = time.perf_counter()
_ = [i for i in np.arange(iterations)]
np_arange_comprehension_time = time.perf_counter() - start_time

# `range`` Comprehension time
start_time = time.perf_counter()
_ = [i for i in range(iterations)]
range_comprehension_time = time.perf_counter() - start_time

print("range Comprehension time =", range_comprehension_time)
print("np.arange Comprehension time =", np_arange_comprehension_time)
print(
    f"range Comprehension is {'faster' if range_comprehension_time < np_arange_comprehension_time else 'slower'} than np.arange Comprehension by {np_arange_comprehension_time / range_comprehension_time if range_comprehension_time < np_arange_comprehension_time else range_comprehension_time / np_arange_comprehension_time}x"
)

print("\n")
time.sleep(0.5)

# Comparison between `list` and `np.array` Comprehension time consumptions

# `list` Comprehension time
start_time = time.perf_counter()
_ = [i for i in range(iterations)]
list_comprehension_time = time.perf_counter() - start_time

# `np.array` Comprehension time
start_time = time.perf_counter()
_ = np.array(i for i in range(iterations))
np_array_comprehension_time = time.perf_counter() - start_time

print("list Comprehension time =", list_comprehension_time)
print("np.array Comprehension time =", np_array_comprehension_time)
print(
    f"list Comprehension is {'faster' if list_comprehension_time < np_array_comprehension_time else 'slower'} than np.array Comprehension by {np_array_comprehension_time / list_comprehension_time if list_comprehension_time < np_array_comprehension_time else list_comprehension_time / np_array_comprehension_time}x"
)

print("\n")
time.sleep(0.5)


# Comparison in cross-conversion time consumption
_ = range(iterations)

# `list` to `np.array` conversion time
start_time = time.perf_counter()
_ = np.array(_)
list_to_np_array_time = time.perf_counter() - start_time

# `np.array` to list conversion time
# start_time = time.perf_counter()
# _ = list(_)
# np_array_to_list_time = time.perf_counter() - start_time
start_time = time.perf_counter()
_ = _.tolist()
np_array_to_list_time = time.perf_counter() - start_time

print("list to np.array conversion time =", list_to_np_array_time)
print("np.array to list conversion time =", np_array_to_list_time)
print(
    f"list to np.array is {'faster' if list_to_np_array_time < np_array_to_list_time else 'slower'} than np.array to list by {np_array_to_list_time / list_to_np_array_time if list_to_np_array_time < np_array_to_list_time else list_to_np_array_time / np_array_to_list_time}x"
)
