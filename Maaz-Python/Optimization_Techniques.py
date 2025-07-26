import time
import numpy as np


# Use Set or Dictionary whenever possible
def find_intersection_through_list(list_1, list_2):
    return (element for element in list_1 if element in list_2)


def find_intersection_thorugh_set(list_1, list_2):
    set_list = set(list_2)
    return (element for element in list_1 if element in set_list)


# Define lists to process
list_1 = range(10**11)
list_2 = range(5 * 10**10, 15 * 10**10)

begin = time.time()
intersection_unopt = list(find_intersection_through_list(list_1, list_2))
whole_time_unopt = time.time() - begin

begin = time.time()
intersection_opt = list(find_intersection_thorugh_set(list_1, list_2))
whole_time_opt = time.time() - begin

print(
    f"The time taken using lists: {whole_time_unopt:.4f} for intersection {len(intersection_unopt)}"
)
print(
    f"The time taken using sets:  {whole_time_opt:.4f} for intersection {len(intersection_opt)}"
)
# print(f"Sets being x{whole_time_opt / whole_time_unopt : .4f} times faster")
speedup = (
    np.round(whole_time_unopt / whole_time_opt, 4)
    if whole_time_opt != 0
    else float("inf")
)
print(f"Sets being x{speedup:.4f} times faster")


# Another
def find_common_elements_list(list1, list2):
    common = []
    for item in list1:  # Go through each item in the first list
        if item in list2:  # Check if it exists in the second list
            common.append(item)  # If yes, add it to our common list
    return common


# Test with reasonably large lists
large_list1 = list(range(10000))
large_list2 = list(range(5000, 15000))

start_time = time.time()
common_list = list(find_common_elements_list(large_list1, large_list2))
list_time = time.time() - start_time
print(f"List approach time: {list_time:.4f} seconds")


def find_common_elements_set(list1, list2):
    set2 = set(list2)  # Convert list to a set (one-time cost)
    return [item for item in list1 if item in set2]  # Check membership in set


start_time = time.time()
common_set = list(find_common_elements_set(large_list1, large_list2))
set_time = time.time() - start_time
print(f"Set approach time: {set_time:.4f} seconds")
# print(f"Improvement: {list_time / set_time:.2f}x faster")
